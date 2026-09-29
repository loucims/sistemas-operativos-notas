---
requiere:
  - "[[Process Control Block (PCB)]]"
habilita:
  - "[[Scheduler]]"
creado: 2026-09-25
---
El `=[[Scheduler]]` tiene que elegir a quien darle la CPU. Pero no todos los procesos *pueden* usarla: si uno esta esperando que el usuario apriete una tecla, darle la CPU es tirarla, ya que estaria esperando.

Entonces dentro del [[Process Control Block (PCB)]], esta el campo `state`, donde el kernel clasifica a cada proceso si puede *avanzar o no*.

**Los 3 estados principales**

| Estado      | Qué significa                            | Si le das la CPU…                                                |
| ----------- | ---------------------------------------- | ---------------------------------------------------------------- |
| **Running** | Está usando la CPU ahora                 | Ya la tiene                                                      |
| **Ready**   | Podría correr, pero la CPU la tiene otro | La usa enseguida                                                 |
| **Blocked** | Espera un evento (disco, teclado, red)   | No sirve de nada: no puede hacer nada hasta que llegue el evento |
![[Process states 2026-09-26 11.50.28.excalidraw|900]]

|#|Transición|Qué la causa|¿Quién la decide?|
|---|---|---|---|
|1|Running → Blocked|El proceso hace una syscall que tiene que esperar (`read()`)|El propio proceso, sin querer: pidió algo lento|
|2|Running → Ready|Interrupción del `Timer`: se le terminó el turno|El kernel|
|3|Ready → Running|El scheduler lo elige|El kernel|
|4|Blocked → Ready|Llega el evento (el disco termina y manda una `Interrupt`)|El kernel, al atender la interrupción|
Además hay dos estados en los bordes: **New** (se está creando, el PCB todavía no está listo) y **Terminated** (terminó, pero su PCB todavía no se liberó).
#### En xv6
Son los mismos con otros nombres (el `enum procstate` de `proc.h`):

|xv6|Equivale a|
|---|---|
|`UNUSED`|Slot libre de la process table (no hay proceso)|
|`USED`|New|
|`RUNNABLE`|Ready|
|`RUNNING`|Running|
|`SLEEPING`|Blocked (el campo `chan` dice qué evento espera)|
|`ZOMBIE`|Terminated: ya terminó, pero el padre todavía no leyó su exit status con `wait()`|