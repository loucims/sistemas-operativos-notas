---
requiere:
  - "[[Lock variable]]"
habilita:
  - "[[Mutual exclusion]]"
creado: 2026-09-29
---
#### Qué problema resuelve
La `Lock variable` fallaba porque **mirar** el lock y **marcarlo** eran dos pasos, y otro hilo se podía meter en el medio. Por software no había forma de juntarlos (Peterson lo esquiva con otra idea, pero solo sirve para 2 y necesita barreras).

La solución es pedirle ayuda al **hardware**: una instrucción de la CPU que **lee el lock y lo pone en 1 en un solo paso indivisible** (_atómico_). Slide U2_3: _"Es mucho más simple cuando tenemos soporte de la CPU: la lectura y la escritura en una sola operación atómica."_

#### Cómo funciona
Lo que hace `TSL` escrito como si fuera C:
```c
// TODO ESTO ES UNA SOLA INSTRUCCIÓN DE LA CPU: nadie se puede meter en el medio
int TSL(int *lock) {
    int viejo = *lock;    // 1. leer el valor que había
    *lock = 1;            // 2. poner 1 ("ocupado")
    return viejo;         // 3. devolver lo que había ANTES
}
```
Y así se usa para armar un lock:
```c
int lock = 0;                     // 0 = libre, 1 = ocupado

void enter_region() {
    while (TSL(&lock) == 1)       // si ya estaba ocupado → girar
        ;
    // si devolvió 0: estaba libre, Y YA LO MARQUÉ como ocupado → entro
}

void leave_region() {
    lock = 0;                     // liberar
}
```
La idea clave es lo que **devuelve**:

- Si devuelve **0**: el lock estaba libre, y en el mismo paso quedó marcado como tuyo. **Entrás.**
- Si devuelve **1**: ya estaba ocupado. Ponerlo en 1 otra vez no cambia nada. **Girás.**
 
#### Por qué ahora sí funciona
El mismo caso que rompió la lock variable, pero con TSL:

| #   | Hilo A                                            | Hilo B                                            | `lock` |
| --- | ------------------------------------------------- | ------------------------------------------------- | ------ |
| 1   | `TSL` → lee 0, pone 1, devuelve **0** → **entra** |                                                   | 1      |
| —   | _⏰ timer: cambio a B_                             |                                                   |        |
| 2   |                                                   | `TSL` → lee **1**, pone 1, devuelve **1** → gira  | 1      |
| 3   |                                                   | gira…                                             | 1      |
| —   | _⏰ vuelve A_                                      |                                                   |        |
| 4   | `leave_region`: `lock = 0`                        |                                                   | 0      |
| 5   |                                                   | `TSL` → lee 0, pone 1, devuelve **0** → **entra** | 1      |

El timer ya no puede caer "entre el mirar y el marcar", porque es una sola instrucción, y la CPU *no atiende interrupciones en medio de una instrucción.*

**¿Y con varias CPUs?** Ahí no alcanza con que sea una sola instrucción, porque otra CPU podría acceder a la memoria a la vez. Slide: _"El bus de memoria se bloquea durante esta instrucción, otras CPUs no pueden interferir."_ Mientras dura el TSL, ninguna otra CPU puede tocar esa dirección de memoria.

En x86 no se llama TSL: hay un prefijo `LOCK` que vuelve atómicas varias instrucciones, y la instrucción `XCHG` (intercambiar), que es atómica siempre. La slide lo menciona: _"otras instrucciones atómicas con la misma funcionalidad: intercambiar (swap)"_.


#### El ejemplo de la cátedra: `incrementar.cpp`
```cpp
std::atomic_flag lock = ATOMIC_FLAG_INIT;        // el lock, arranca en "libre"

while (lock.test_and_set(std::memory_order_acquire)) { }   // enter_region
shared_counter++;                                          // región crítica
lock.clear(std::memory_order_release);                     // leave_region
```

Es exactamente el patrón de arriba. `test_and_set()` es el TSL, y `memory_order_acquire` / `release` son las **barreras de memoria** que vimos con Peterson. El comentario del archivo dice: _"El resultado siempre será exactamente 200000"_. (`incrtsl.c` hace lo mismo en xv6, con la instrucción atómica de su CPU.)

#### Evaluación

|Requisito|¿Cumple?|
|---|---|
|Mutual exclusion|✅ Solo uno puede recibir 0|
|Progreso|✅ Si está libre, entrás|
|Espera limitada|❌ **No garantizada**: no hay orden de llegada. Cuando se libera, entra el primero que haga TSL, y un hilo con mala suerte podría perder siempre|
|Cualquier número de hilos/CPUs|✅ A diferencia de Peterson|

Y sigue con los problemas de `Busy waiting`: gasta CPU girando, y la priority inversion.

#### Con qué se confunde

- **TSL ≠ Disabling interrupts**: TSL no apaga nada. Solo hace que **una** instrucción sea indivisible y bloquea el bus durante esa instrucción.
- **TSL es la instrucción, no el lock**: el lock es la variable más el `while`. Un spinlock "de verdad" es justamente una lock variable que usa TSL para el "mirar y marcar".