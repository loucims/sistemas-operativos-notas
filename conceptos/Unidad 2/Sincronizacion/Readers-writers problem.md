---
requiere:
  - "[[Semaphore]]"
  - "[[Mutex]]"
habilita:
  - "[[Starvation]]"
creado: 2026-09-29
---
#### Qué problema es (slide U3_1, "Lectores y escritores")
Una **base de datos** compartida, con dos tipos de procesos:

- **Lectores**: solo leen.
- **Escritores**: modifican.

Las reglas:

| Situación                      | ¿Permitido? | Por qué                                            |
| ------------------------------ | ----------- | -------------------------------------------------- |
| Varios **lectores** a la vez   | ✅ Sí        | Leer no cambia nada, así que no hay race condition |
| **Lector + escritor** a la vez | ❌ No        | El lector podría leer datos a medio escribir       |
| **Dos escritores** a la vez    | ❌ No        | Se pisan (race condition clásica)                  |
Con un simple `Mutex` sobre toda la base funcionaría, pero sería un desperdicio: los lectores harían fila de a uno aunque podrían leer todos juntos. El desafío es **dejar entrar a todos los lectores juntos**, pero a los escritores **solos**.

#### La idea (slide "Bosquejo de la solución")

- **El primer lector que llega** toma la llave de la base **en nombre de todos los lectores**.
- Los lectores siguientes solo se **cuentan**: entran sin pedir la llave.
- **El último lector que se va** devuelve la llave.
- **Un escritor** tiene que tomar la llave él solo, así que espera a que no quede ningún lector.

#### El código (Tanenbaum; es el `readers_writers1.c` de la cátedra)

Le cambio los nombres a los semáforos para que se lea mejor. Entre paréntesis van los nombres originales, que son los que vas a ver en el parcial.

```c
semaphore llave_rc   = 1;     // (mutex) protege al contador rc
semaphore llave_base = 1;     // (db)    acceso a la base de datos
int rc = 0;                   // cuántos lectores hay adentro

// ──────────────── Lector ────────────────
while (TRUE) {
    down(&llave_rc);
    rc = rc + 1;
    if (rc == 1)                  // soy el PRIMER lector
        down(&llave_base);        //   → tomo la base por todos los                                         //     lectores
    up(&llave_rc);

    read_data_base();             // leo (junto con otros lectores)

    down(&llave_rc);
    rc = rc - 1;
    if (rc == 0)                  // soy el ÚLTIMO lector
        up(&llave_base);          //   → libero la base
    up(&llave_rc);

    use_data_read();
}

// ─────────────── Escritor ───────────────
while (TRUE) {
    think_up_data();
    down(&llave_base);            // necesito la base para mí solo
    write_data_base();
    up(&llave_base);
}
```

Hay **dos** llaves, y cada una protege una cosa distinta:

|Semáforo|Qué protege|Quién lo usa|
|---|---|---|
|`llave_rc` (mutex)|El contador `rc`: sumar y restar es región crítica, como el `x++`|Solo los lectores, un instante|
|`llave_base` (db)|La base de datos entera|El **primer y el último** lector (en nombre de todos) y **cada** escritor|
#### Con qué se confunde
- **No es producer-consumer**: acá nadie espera a que "haya algo". El problema es **cuántos pueden estar adentro a la vez, según el tipo**.
- **`rc` necesita su propio mutex**: sumarlo y restarlo es el `x++` de siempre. Sin `llave_rc`, dos lectores podrían pensar los dos que son "el primero" (o "el último").

