---
requiere:
  - "[[Shortest Job First]]"
  - "[[Preemptive scheduling]]"
habilita: []
creado: 2026-09-29
---
#### Qué problema resuelve
En tu ejercicio de SJF, P1 (el más largo) corrió primero solo porque llegó antes, y P2 y P3, mucho más cortos, tuvieron que esperar a que terminara. SJF no puede corregir eso porque **no saca a nadie**.

SRTN es **SJF apropiativo**. Slide U2_5: _"El planificador elige el proceso cuyo tiempo de ejecución **restante** se estima más corto. Cuando arranca una tarea nueva, su tiempo estimado es comparado con el restante del proceso actual; si es menor, este se suspende y la nueva tarea toma el control."_

#### Cómo funciona
El scheduler decide **en dos momentos**:

1. **Cuando llega un proceso nuevo**: compara su burst con lo que **le falta** (_remaining time_) al que está corriendo. Si el nuevo es **más corto**, saca al actual (vuelve a ready, con lo que le falte) y corre el nuevo. Si empatan, sigue el actual.
2. **Cuando un proceso termina**: elige, entre los ready, el de **menor remaining time**.

No hace falta un timer: el disparador es **la llegada** de un proceso.

#### Ejemplo paso a paso

|Proceso|Arrival|Burst|
|---|---|---|
|P1|0|8|
|P2|1|4|
|P3|2|9|
|P4|3|5|

|t|Evento|Remaining de cada uno|Decisión|
|---|---|---|---|
|0|Llega P1|P1 = 8|Corre **P1**|
|1|Llega P2 (4)|P1 = **7**, P2 = 4|4 < 7 → **saca a P1**, corre **P2**|
|2|Llega P3 (9)|P2 = **3**, P3 = 9|9 > 3 → sigue **P2**|
|3|Llega P4 (5)|P2 = **2**, P4 = 5|5 > 2 → sigue **P2**|
|5|Termina P2|P1 = 7, P3 = 9, P4 = 5|El menor: **P4**|
|10|Termina P4|P1 = 7, P3 = 9|El menor: **P1**|
|17|Termina P1|P3 = 9|**P3**|
|26|Termina P3|||

```
 | P1 | P2           | P4              | P1                     | P3                         |
 0    1              5                 10                       17                           26
```

|Proceso|Arrival|Burst|Completion|Turnaround (completion − arrival)|Waiting (turnaround − burst)|
|---|---|---|---|---|---|
|P1|0|8|17|17|9|
|P2|1|4|5|4|0|
|P3|2|9|26|24|15|
|P4|3|5|10|7|2|
|**Promedio**||||52 / 4 = **13**|26 / 4 = **6,5**|

Fijate en P1: esperó **dos veces**, de 1 a 10 (lo sacaron), y nada antes de empezar. Por eso acá conviene la fórmula **waiting = turnaround − burst**: 17 − 8 = 9. Si sumás los ratos en ready, da igual: 0 (antes de empezar) + 9 (de t = 1 a t = 10) = 9.