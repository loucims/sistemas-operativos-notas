---
requiere:
  - "[[Interrupt]]"
  - "[[Mutual exclusion]]"
habilita:
  - "[[Semaphore]]"
creado: 2026-09-28
---
#### Qué problema resuelve
Pensá **con una sola CPU**: ¿qué puede hacer que un hilo pierda la CPU a mitad de la región crítica (entre el leer y el guardar del `x++`)?

- Que se **bloquee** solo, pidiendo E/S. Eso no pasa: dentro de la región crítica no se piden cosas lentas.
- Que llegue una **interrupción**. El `Timer` avisa que se acabó el quantum → el kernel hace un `Cambio de contexto` → entra otro hilo. **Esta es la causa de la race condition.**

Entonces, si **apagás las interrupciones** antes de entrar, nadie te puede sacar la CPU hasta que las prendas de nuevo.

#### Cómo funciona
```c
apagar_interrupciones();    // x86: instrucción cli (clear interrupt flag)
x++;                        // región crítica: nadie me puede interrumpir
prender_interrupciones();   // x86: instrucción sti (set interrupt flag)
```

| #   | Hilo A                | Lo que pasa afuera                                                  |
| --- | --------------------- | ------------------------------------------------------------------- |
| 1   | apaga interrupciones  |                                                                     |
| 2   | lee x                 | ⏰ el timer quiere interrumpir → **queda pendiente**, no se atiende  |
| 3   | suma 1, guarda        |                                                                     |
| 4   | prende interrupciones | → recién ahora se atiende el timer y puede haber cambio de contexto |
Los 3 pasos del `x++` quedan juntos. Con una sola CPU, eso ya es mutual exclusion.

#### Inconvenientes (slide U2_3)
La slide dice que _"satisface los requerimientos, pero **no es adecuada para los procesos de usuario**"_:

| Inconveniente                                      | Por qué                                                                                                                                            |
| -------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Es privilegiada**                                | Apagar interrupciones es una instrucción de modo kernel (`Dual-mode operation`). Un proceso de usuario no puede usarla                             |
| **Si te olvidás de prenderlas, hay que reiniciar** | Sin timer, el kernel nunca recupera la CPU                                                                                                         |
| **Demasiado poderosa**                             | No frena solo a los hilos que quieren esa región crítica: frena a **todos**. Y también las interrupciones de disco, teclado y red quedan esperando |
| **Inflexible: todo o nada**                        | No podés decir "que no entre nadie a _esta_ región crítica". Es apagar todo                                                                        |
| **⚠️ No sirve con varias CPUs**                    | Solo apaga las interrupciones de **tu** CPU. Un hilo en otra CPU sigue corriendo y puede tocar el mismo dato al mismo tiempo                       |

**¿Dónde se usa, entonces?** Adentro del **kernel**, para regiones críticas cortitas (slide: _"en las partes de bajo nivel del sistema operativo, ej: durante el manejo de interrupciones"_). En máquinas con varias CPUs se combina con un spinlock: las interrupciones apagadas cubren tu propia CPU y el spinlock cubre a las otras. Es lo que hace `acquire()` en xv6.

#### Con qué se confunde
- **"Apagar interrupciones = que no corra nadie más"**: solo en **tu** CPU. Las otras siguen trabajando.
- **No es busy waiting**: acá nadie gira esperando. Directamente no hay cambio de contexto mientras las interrupciones están apagadas. (La slide igual la lista entre las soluciones "con espera activa", porque agrupa ahí todo lo que no se bloquea.)

### ¿Cómo funciona con varias CPUs en el kernel?

Hay un malentendido: el kernel **no le da un spinlock a los demás**. Hay **un solo spinlock por cada dato compartido** (por ejemplo, uno para la process table), y **cada CPU que quiera ese dato** hace los mismos dos pasos por su cuenta. Así es `acquire()` en xv6, simplificado:

```c
void acquire(struct spinlock *lk) {
    apagar_interrupciones();              // 1. en MI CPU
    while (test_and_set(&lk->locked))     // 2. intentar tomar el lock; si está ocupado, girar
        ;
}

void release(struct spinlock *lk) {
    lk->locked = 0;                       // soltar el lock
    prender_interrupciones();             // en MI CPU
}
```

`test_and_set` es "leer el lock y ponerlo en 1 **en un solo paso indivisible**". Es el concepto `Test-and-set`, que viene en un rato.

Con 2 CPUs y la process table:

| #   | CPU 0                                                    | CPU 1                                                            | lock            |
| --- | -------------------------------------------------------- | ---------------------------------------------------------------- | --------------- |
| 1   | `acquire`: apaga **sus** interrupciones, toma el lock    |                                                                  | ocupado (CPU 0) |
| 2   | trabaja con la process table                             | `acquire`: apaga **sus** interrupciones, lock ocupado → **gira** | ocupado         |
| 3   | `release`: suelta el lock, prende **sus** interrupciones | sigue girando…                                                   | libre           |
| 4   |                                                          | ve el lock libre → lo toma → entra                               | ocupado (CPU 1) |
Cada cosa protege contra algo distinto:

| Mecanismo                 | Protege contra                                                                                                           |
| ------------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| **Spinlock**              | Las **otras CPUs**                                                                                                       |
| **Apagar interrupciones** | **Tu propia CPU**: que un handler de interrupción que quiere el mismo lock no te interrumpa a mitad de la región crítica |
