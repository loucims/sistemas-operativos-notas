---
requiere:
  - "[[Preemptive scheduling]]"
  - "[[First-Come First-Served]]"
  - "[[Cambio de contexto]]"
habilita:
  - "[[Priority Scheduling]]"
creado: 2026-09-29
---
#### Qué problema resuelve
FCFS (First Come First Served), SJF (Shortest Job First) y SRTN (Shortest Time Remaining) son para **sistemas por lotes**: miran el turnaround. 
En un sistema **interactivo** eso no alcanza: si un proceso largo agarra la CPU, el usuario ve la pantalla congelada hasta que termine. Lo que importa es el **response time**: que **todos** reciban un poco de CPU **seguido**.

**Round Robin:** *turnos cortos, en círculo,* para todos.
Es FCFS (*First Come First Served*) **más** preemption por timer.

#### Cómo funciona
- La ready queue es una **fila FIFO**, como en FCFS.
- Cada proceso corre como mucho un **quantum** (`q`): una cantidad fija de tiempo.
- Si **termina o se bloquea antes** del quantum, el siguiente arranca enseguida y se vuelve a empezar a contar. *El quantum es un timer que el scheduler arranca de cero* cada vez que *le da la CPU a alguien*
- Si **se le acaba el quantum** y no terminó, el `Timer` interrumpe, el proceso va **al final de la fila** (preemption), y corre el siguiente.

#### Ejemplo de la slide: 24, 3, 3 con q = 3 (todos llegan en 0)

|t|Corre|Qué pasa|Fila después|
|---|---|---|---|
|0–3|P1|Usa su quantum; le quedan 21 → al final|P2, P3, P1|
|3–6|P2|Termina justo (necesitaba 3)|P3, P1|
|6–9|P3|Termina|P1|
|9–30|P1|Está solo: corre quantum tras quantum hasta terminar|—|

```
 | P1 | P2 | P3 | P1 | P1 | P1 | P1 | P1 | P1 | P1 |
 0    3    6    9    12   15   18   21   24   27   30
```

|Proceso|Burst|Completion|Turnaround|Waiting (turnaround − burst)|Response|
|---|---|---|---|---|---|
|P1|24|30|30|6|0|
|P2|3|6|6|3|3|
|P3|3|9|9|6|6|
|**Promedio**|||**15**|**5**|**3**|

Comparación con la misma carga:

|s|FCFS|SJF|**RR (q = 3)**|
|---|---|---|---|
|Average waiting|17|3|**5**|
|Average turnaround|27|13|**15**|
Slide: _"RR da tiempos de espera y retorno intermedios entre FCFS y SJF."_ No es el mejor en waiting ni en turnaround. Su fuerte es el **response time**, y que **no necesita saber cuánto dura cada proceso** (SJF sí).

#### El tamaño del quantum (slide "Importancia de un buen factor de cuanto")

| Quantum        | Qué pasa                                                                                                                                                                                                                |
| -------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Muy chico**  | Muchísimos **cambios de contexto**. Cada uno cuesta _"centenares de instrucciones de CPU"_, así que buena parte del tiempo se va en cambiar y no en trabajar. _"La máquina parecerá más lenta de lo que realmente es."_ |
| **Muy grande** | Pocos cambios, pero un proceso puede tener la CPU mucho tiempo seguido, y los demás esperan su turno. Con quantum infinito, **RR se convierte en FCFS**                                                                 |
La regla de la slide: el quantum tiene que ser **mucho mayor que el costo del cambio de contexto**, pero **mucho menor que el tiempo de respuesta deseado**.

Un número para verlo: si un cambio de contexto cuesta 1 ms, con q = 4 ms **1 de cada 5 ms** se va en cambios (20% desperdiciado). Con q = 100 ms, el desperdicio baja a ~1%, pero con 10 procesos, cada uno espera hasta ~1 segundo entre turno y turno.

#### Con qué se confunde
- **Round Robin ≠ FCFS**: los dos usan una fila FIFO, pero RR **saca** al proceso cuando se le acaba el quantum.
- **El quantum no se "guarda"**: si un proceso se bloquea a mitad del quantum, lo que le sobraba se pierde. La próxima vez arranca con un quantum nuevo, entero.
- **Detalle para ejercicios con llegadas**: si en el mismo instante llega un proceso nuevo y otro vuelve a la fila por fin de quantum, la convención más común es poner **primero al que llega**. (En el parcial los ejercicios suelen tener a todos llegando en t = 0, y ahí no pasa.)