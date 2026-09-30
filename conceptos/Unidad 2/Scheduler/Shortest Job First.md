---
requiere:
  - "[[First-Come First-Served]]"
habilita:
  - "[[Shortest Remaining Time Next]]"
creado: 2026-09-29
---
#### Qué problema resuelve
El **efecto convoy** de FCFS: un proceso largo adelante hace esperar a todos los cortos. En tu ejercicio de `Scheduling criteria` descubriste la regla solo: **poniendo los cortos primero, bajan el waiting time y el turnaround time**. *SJF* (*Shortest Job First*) es exactamente esa regla.

#### Cómo funciona (slide U2_5, "SJF")
- Cada vez que la CPU queda libre, el scheduler elige, **entre los procesos que ya llegaron**, el de **burst time más corto**.
- Una vez que un proceso empieza, **corre hasta terminar**: es **no apropiativo**.
- Si dos tienen el mismo burst, se desempata por orden de llegada (FCFS).

El ejemplo de la slide (24, 3, 3; todos llegan en 0):
```
 | P2 | P3 | P1                          |
 0    3    6                             30
```
Average waiting: (0 + 3 + 6) / 3 = **3**, contra **17** con FCFS. Slide: _**"SJF siempre da mínimo tiempo de espera y de retorno."_** <---- ==IMPORTANTE==

**¿Por qué es el óptimo?** Si un proceso largo está adelante de uno corto, intercambiarlos hace que el corto espere **mucho menos** y el largo espere **un poco más**. Por ejemplo, con 24 y 3: el de 3 deja de esperar 24, y el de 24 pasa a esperar solo 3. En total se ganan 21. Siempre conviene que el corto vaya primero, así que el orden "del más corto al más largo" es el mejor posible.
#### Cuando llegan en momentos distintos
Ojo con dos cosas:
1. Solo podés elegir entre los que **ya llegaron**. Si en t = 0 hay uno solo, corre ese, aunque sea el más largo.
2. Una vez que arrancó, **nadie lo saca**, aunque llegue uno más corto.

(El "siempre es óptimo" de la slide vale cuando **todos llegan al mismo tiempo**. *Con llegadas distintas*, *a veces un algoritmo que puede sacar procesos lo hace mejor.* Esa es la idea del próximo concepto.)

#### Ventajas y desventajas (slide)

| ✅                                                            | ❌                                                                                                               |
| ------------------------------------------------------------ | --------------------------------------------------------------------------------------------------------------- |
| **Óptimo** en average waiting time y average turnaround time | **Starvation**: si siempre llegan procesos cortos, el largo nunca corre                                         |
|                                                              | **No se puede implementar tal cual**: el scheduler **no sabe** cuánto va a durar cada ráfaga antes de que corra |
**¿Cómo se usa entonces?** Se **estima**. La slide dice: _"usar promedio exponencial del comportamiento en el pasado para tratar de predecir la longitud de la próxima ráfaga de CPU"_. La *idea*: si las últimas ráfagas de un proceso fueron cortas, probablemente la próxima también. La estimación nueva es una mezcla entre la ráfaga que acaba de pasar y la estimación anterior:

**estimación nueva = α · última ráfaga real + (1 − α) · estimación anterior**

(con α entre 0 y 1: cuanto más grande, más peso tiene lo último que pasó).

#### Con qué se confunde
- **SJF ≠ el proceso más corto en total**: se mira la **próxima ráfaga de CPU**, no cuánto dura el programa entero. Un proceso I/O-bound tiene ráfagas cortas, así que SJF lo favorece (lo que viste que conviene en `CPU-bound and I-O-bound`).
- **SJF ≠ SRTN**: SJF no saca a nadie. Su versión apropiativa es **SRTN**, el próximo concepto.