---
requiere:
  - "[[Mutex]]"
  - "[[Sleep and wakeup]]"
habilita: []
creado: 2026-09-29
---
Con semáforos es muy facil romper todo:
- un `up` de más → dos hilos en la región crítica;
- dos `down` en el orden equivocado → deadlock;
- un `down` que te olvidás → race condition.
Slide: _"Debemos ser muy cuidadosos cuando usamos semáforos, un solo error y todo el sistema se detiene."_

El problema de fondo es que **la sincronización queda en manos del programador**, desparramada por todo el código. El **monitor** (Hoare 1974, Brinch Hansen 1975) la pone en manos del **lenguaje**: vos escribís el código y el compilador pone los locks.

#### Cómo funciona, parte 1: la mutual exclusion la pone el lenguaje
Slide: _"Un monitor es como una clase, pero solo se puede ejecutar un procedimiento a la vez."_

```
monitor Contador {
    int x = 0;

    procedure incrementar() {    // el compilador agrega: lock al entrar
        x = x + 1;
    }                            // unlock al salir

    procedure leer() { return x; }
}
```

- Los datos (`x`) **solo** se pueden tocar a través de los procedimientos del monitor.
- Si un hilo está adentro de **cualquier** procedimiento, los demás que quieran entrar **esperan dormidos** en la puerta.
- Nunca escribís `lock` ni `unlock`, así que no te los podés olvidar ni dar vuelta.

Slide: _"Son una implementación en el lenguaje de los mutex."_ Java los tiene (`synchronized`); C y C++ no.

#### Cómo funciona, parte 2: variables de condición
La mutual exclusion no alcanza. En producer-consumer, el productor entra al monitor, ve que el buffer está lleno, y tiene que **esperar**. Pero si espera **adentro** del monitor, nadie más puede entrar, y el consumidor nunca va a vaciar nada.

Para eso están las **variables de condición** `c`, con dos operaciones:

| Operación       | Qué hace                                                                                                                                                      |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`wait(c)`**   | **Suelta el monitor** y se duerme en la fila de `c`, las dos cosas juntas y de forma atómica. Cuando lo despiertan, vuelve a tomar el monitor antes de seguir |
| **`signal(c)`** | Despierta a **uno** de los que esperan en `c`. Si no hay nadie esperando, **no pasa nada**: la señal se pierde                                                |

Ese "suelta el monitor al dormirse" es la clave. Es lo que permite que otro entre y cambie las cosas.

#### Producer-consumer con monitor
Les puse nombres que dicen **qué se espera**: `wait(hay_lugar)` se lee "esperá hasta que haya lugar".

```c
monitor Buffer {
    condition hay_lugar;          // acá espera el productor si está lleno
    condition hay_items;          // acá espera el consumidor si está                                       //   vacío
    int count = 0;

    procedure insertar(item) {
        if (count == N)
            wait(hay_lugar);      // lleno → suelto el monitor y duermo
        insert_item(item);
        count = count + 1;
        if (count == 1)
            signal(hay_items);    // estaba vacío → despierto a un                                          // consumidor
    }

    procedure sacar() {
        if (count == 0)
            wait(hay_items);      // vacío → suelto el monitor y duermo
        item = remove_item();
        count = count - 1;
        if (count == N - 1)
            signal(hay_lugar);    // estaba lleno → despierto al productor
        return item;
    }
}

// ─────────────── Productor ───────────────
while (TRUE) {
    item = produce_item();
    Buffer.insertar(item);
}

// ─────────────── Consumidor ──────────────
while (TRUE) {
    item = Buffer.sacar();
    consume_item(item);
}
```

Es **casi idéntico** a la solución ingenua con `sleep`/`wakeup` que no funcionaba. La diferencia es que ahora todo pasa **adentro del monitor**: `count` está protegido, y el "mirar `count` y dormirse" no puede ser interrumpido por el otro. Te pregunto por qué en la pregunta 1.

#### ¿Quién sigue después de un `signal`? (slide)
Cuando A hace `signal` y despierta a B, **los dos** quieren estar adentro del monitor, y solo puede haber uno.

| Propuesta                   | Qué pasa                                                                             |
| --------------------------- | ------------------------------------------------------------------------------------ |
| **Hoare**                   | B corre **inmediatamente**; A espera                                                 |
| **Brinch Hansen**           | A tiene que **salir del monitor** inmediatamente (el `signal` es lo último que hace) |
| **Java** (y casi todos hoy) | A **sigue**; B se despierta pero espera a que A salga para volver a entrar           |

Con el modelo de Java, entre que despertaron a B y que B vuelve a entrar puede pasar cualquier cosa, así que B tiene que **volver a chequear** la condición.

>*Es la regla de oro con variables de condición:* **siempre `while`, nunca `if`**. Un despertar es solo "puede que ahora se cumpla": chequealo de nuevo.

#### Con qué se confunde

|                      | Semáforo                                | Variable de condición                                  |
| -------------------- | --------------------------------------- | ------------------------------------------------------ |
| ¿Guarda las señales? | **Sí**: `up` sin nadie esperando suma 1 | **No**: `signal` sin nadie esperando se pierde         |
| ¿Tiene un valor?     | Sí, un contador                         | No, solo una fila de dormidos                          |
| ¿Se usa sola?        | Sí                                      | **No**: siempre adentro de un monitor (o con un mutex) |

- **La variable de condición no es una condición.** Slide: _"Las variables de condición NO son condiciones. Así que nunca hagan `if (conditionVariable)`."_ No es un booleano: la condición real es `count == N`, y la variable de condición es **la fila donde esperás** a que eso cambie (*el await basicamente*).
- **En C no hay monitores**: se arman a mano con `pthread_mutex` (la mutual exclusion) más `pthread_cond_wait` / `pthread_cond_signal` (las variables de condición).