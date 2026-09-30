---
requiere:
  - "[[Semaphore]]"
  - "[[Test-and-set]]"
habilita:
  - "[[Monitor]]"
creado: 2026-09-29
---
#### Qué problema resuelve
El uso más común de un semáforo, por lejos, es el **binario**: una sola ficha, para `Mutual exclusion`. Como es tan común, existe una versión simplificada dedicada solo a eso: el **mutex** (de _mutual exclusion_).

Slide U2_4: _"Versión simplificada de un semáforo. Sólo dos estados. Se usan para garantizar la exclusión mutua. Son fáciles y eficientes para implementar."_

#### Cómo funciona: `down` y `up` sobre un semáforo binario (⭐)
Pensado como semáforo, con **1 = libre** y **0 = ocupado**:

|Operación|Si está **libre** (1)|Si está **ocupado** (0)|
|---|---|---|
|**`down`** (entrar)|Lo pone en 0 y **entra** a la región crítica|**Se duerme** en la lista de espera|
|**`up`** (salir)|— (no debería pasar: nadie sale sin haber entrado)|Si **hay alguien esperando**: lo despierta y le pasa la llave (queda en 0). Si **no hay nadie**: lo pone en 1|
#### Exclusión mutua entre dos procesos con una variable global (⭐)
```c
int x = 0;                   // variable global compartida
semaphore mutex = 1;         // 1 = libre

// ──────────────── Proceso A ────────────────
while (TRUE) {
    down(&mutex);            // entro (o duermo si B está adentro)
    x = x + 1;               // región crítica
    up(&mutex);              // salgo (y despierto a B si esperaba)
    otras_cosas();
}

// ──────────────── Proceso B ────────────────
while (TRUE) {
    down(&mutex);
    x = x - 1;               // región crítica
    up(&mutex);
    otras_cosas();
}
```
Siempre el mismo patrón: **`down` antes de la región crítica y `up` después**, con el semáforo arrancando en 1.

#### Implementación en espacio de usuario (slide "Mutex, ejemplo de implementación")

La slide dice: _"si está disponible la instrucción TSL, se pueden implementar en espacio de usuario"_. En C, sería así:
```c
int mutex = 0;                        // convención de la slide: 0 = libre

// ──────────────── mutex_lock ────────────────
void mutex_lock() {
    while (TSL(&mutex) == 1)          // ¿estaba ocupado?
        thread_yield();               // → cedo la CPU a otro hilo y                                               reintento
    // devolvió 0: estaba libre y ya es mío
}

// ─────────────── mutex_unlock ───────────────
void mutex_unlock() {
    mutex = 0;
}
```
Es casi el spinlock de `Test-and-set`, con **una diferencia clave**: si está ocupado, **no gira**, sino que llama a `thread_yield()` y le cede la CPU a otro hilo. Te pregunto por qué en la pregunta 1.
#### En pthreads (slide "Mutexes en pthreads")

| Llamada                 | Qué hace                                                                   |
| ----------------------- | -------------------------------------------------------------------------- |
| `pthread_mutex_init`    | Crea un mutex                                                              |
| `pthread_mutex_destroy` | Lo destruye                                                                |
| `pthread_mutex_lock`    | Toma el lock, o **se bloquea** si está ocupado                             |
| `pthread_mutex_trylock` | Intenta tomarlo; si está ocupado, **falla y vuelve enseguida** (no espera) |
| `pthread_mutex_unlock`  | Lo libera                                                                  |
#### Con qué se confunde

|                   | Semáforo (contador)                                | Mutex                                    |
| ----------------- | -------------------------------------------------- | ---------------------------------------- |
| Valores           | 0 a N                                              | Solo libre / ocupado                     |
| Para qué          | Contar recursos, esperar eventos, mutual exclusion | **Solo** mutual exclusion                |
| ¿Quién lo libera? | Cualquiera puede hacer `up`                        | Por convención, **el mismo** que lo tomó |
**Mutex ≠ spinlock**: el spinlock gira esperando; el mutex, si está ocupado, **cede la CPU o se duerme**.