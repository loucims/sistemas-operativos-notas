---
requiere:
  - "[[Deadlock]]"
  - "[[Two-phase locking]]"
habilita: []
creado: 2026-09-30
---
#### Qué es 

> _"El bloqueo activo se produce cuando un hilo intenta continuamente una acción que falla todas las veces."_

En criollo: **todos corren, nadie avanza**. No hay nadie dormido. Los procesos están activos, gastan CPU, reintentan, pero el trabajo útil nunca progresa.

La analogía clásica: dos personas se cruzan en un pasillo. Las dos se corren a la derecha para dejar pasar, después las dos a la izquierda, después otra vez a la derecha… Se mueven todo el tiempo y nunca pasan.

#### Ejemplo 1: los filósofos "reparados" (U3_1, p.11)

Para evitar el deadlock, la slide propone: _"En lugar de tomar el tenedor (i+1) % N, compruebe primero si está disponible. Si no está disponible, espera un rato y vuelve a intentarlo."_
```c
void philosopher(int i) {
    while (TRUE) {
        think();
        take_fork(i);                        // tomo el izquierdo
        while (!disponible((i + 1) % N)) {   // ¿está libre el derecho?
            put_fork(i);                     // no: suelto el izquierdo
            sleep(1);                        // espero un rato
            take_fork(i);                    // y reintento
        }
        take_fork((i + 1) % N);
        eat();
        put_fork(i);
        put_fork((i + 1) % N);
    }
}
```

Si los 5 arrancan **sincronizados**:

|Paso|Todos los filósofos hacen…|Resultado|
|---|---|---|
|1|toman su izquierdo|cada uno tiene 1|
|2|miran el derecho|ocupado (lo tiene el vecino)|
|3|sueltan el izquierdo|todos con 0|
|4|duermen 1 segundo||
|5|toman su izquierdo|**igual que el paso 1** → se repite para siempre|

La slide lo resume así: _"Todos pueden correr, pero nadie avanza"_.

⚠️ Esa misma slide lo llama **"inanición"**. Técnicamente es un **livelock**: no hay uno solo que se queda afuera mientras los demás avanzan, sino que **nadie** avanza. Si en el parcial te lo preguntan, explicá la diferencia.

#### Ejemplo 2: el que viste recién
`Two-phase locking` con A y B intercalados: A toma r1, B toma r2, fallan los dos, sueltan, reintentan… Es el mismo patrón.

#### Por qué pasa
Fijate que en los dos ejemplos los procesos **rompieron _hold and wait_** (sueltan lo que tienen si no consiguen todo). Así esquivaron el deadlock, pero lo cambiaron por otro problema. Si todos reaccionan **igual y al mismo tiempo**, repiten el mismo baile para siempre. La causa es la **simetría**.

#### Cómo se resuelve

> _"Puede ser evitado repitiendo la acción después de una demora aleatoria."_

Si cada uno espera un tiempo **distinto**, alguno reintenta antes que los demás, consigue todo y avanza. El azar **rompe la simetría**.

Pero la p.12 avisa: _"Normalmente, eso funcionará bien. Pero algunas aplicaciones requieren una solución garantizada."_ Con mala suerte, los tiempos aleatorios pueden volver a coincidir. Es muy improbable, pero no imposible.

#### Deadlock vs livelock vs starvation ⭐

|                        | **Deadlock**              | **Livelock**                   | **`Starvation`**                     |
| ---------------------- | ------------------------- | ------------------------------ | ------------------------------------ |
| Estado de los procesos | **Bloqueados** (dormidos) | **Corriendo** (activos)        | Los demás corren; uno espera         |
| ¿Usan CPU?             | No                        | **Sí, y la desperdician**      | Los demás sí                         |
| ¿Quién avanza?         | Nadie del conjunto        | **Nadie**                      | **Todos menos uno**                  |
| ¿Se destraba solo?     | **Nunca**                 | Quizás, por azar o por timing  | Quizás, si deja de haber competencia |
| ¿Se ve en el grafo?    | Sí: **ciclo**             | No: sueltan todo, no hay ciclo | No                                   |
