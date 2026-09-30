---
requiere:
  - "[[Scheduler]]"
habilita:
  - "[[First-Come First-Served]]"
  - "[[Shortest Job First]]"
  - "[[Round Robin]]"
creado: 2026-09-29
---
#### Qué problema resuelve
Para decir que un algoritmo es "mejor" que otro hace falta **medir**. Pero "mejor" depende de qué te importa: que terminen muchos trabajos, que el usuario no espere, que nadie se quede atrás… Estas son las métricas con las que se comparan.

#### Las métricas 
> Las primeras dos son de *sistema*, todas las demas de *proceso*

| Métrica                                 | Qué mide                                                        | Cómo se calcula                 | ¿Mejor alto o bajo? |
| --------------------------------------- | --------------------------------------------------------------- | ------------------------------- | ------------------- |
| **Utilización de CPU**                  | Qué fracción del tiempo la CPU está trabajando                  | tiempo ocupada / tiempo total   | Alto                |
| **Throughput** (rendimiento)            | Cuántos procesos terminan por unidad de tiempo                  | procesos terminados / tiempo    | Alto                |
| **Turnaround time** (tiempo de retorno) | Cuánto tarda cada proceso **desde que llega hasta que termina** | **fin − llegada**               | Bajo                |
| **Waiting time** (tiempo de espera)     | Cuánto tiempo pasa **en la ready queue**, esperando la CPU      | **turnaround − ráfaga de CPU**  | Bajo                |
| **Response time** (tiempo de respuesta) | Cuánto tarda en **empezar** a correr (o en responder)           | primera vez que corre − llegada | Bajo                |
>La relación clave: **turnaround = waiting_time + ejecución**. Un proceso que arranca y termina sin interrupciones y empieza recien llega tiene espera 0, y su retorno es exactamente lo que tarda en correr.
### Las mas importantes ecuaciones
## **Turnaround = completion - arrival**
## **Waiting Time = Turnaround - burst_time**

#### Diferencia entre response time y waiting time
La diferencia es una sola: **response cuenta solo la PRIMERA espera. Waiting suma TODAS las esperas.**
###### Ejemplo: la guardia de un hospital
1. Llegás a la guardia.
2. Esperás **30 min** y te atiende la enfermera (te toma la presión).
3. Te manda a sentarte de nuevo. Esperás **2 horas** más.
4. Te atiende el médico, y te vas.

|                   | Qué cuenta                                      | Resultado                     |
| ----------------- | ----------------------------------------------- | ----------------------------- |
| **Response time** | Cuánto tardaron en atenderte **la primera vez** | **30 min**                    |
| **Waiting time**  | Cuánto estuviste **sentado esperando en total** | 30 min + 2 h = **2 h 30 min** |
La response termina **apenas alguien te da bola por primera vez**. Lo que pase después ya no le importa. La waiting sigue sumando cada vez que te mandan a esperar de nuevo.

#### Qué importa en cada tipo de sistema

| Sistema         | Métricas clave                             | Por qué                                                                                                                             |
| --------------- | ------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------- |
| **Por lotes**   | Throughput, turnaround, utilización de CPU | Nadie está mirando: importa terminar mucho y rápido                                                                                 |
| **Interactivo** | **Tiempo de respuesta**, proporcionalidad  | El usuario está esperando. Slide: _"los usuarios se sienten más satisfechos si las solicitudes 'baratas' se completan rápidamente"_ |
| **Tiempo real** | **Cumplir plazos**, previsibilidad         | Un resultado tarde puede ser inútil                                                                                                 |
--> Mas info sobre tiempo real [^1]
#### Cómo se calculan: el diagrama de Gantt
La herramienta es el **diagrama de Gantt**: una línea de tiempo que muestra quién tiene la CPU en cada momento.

Ejemplo de la slide: tres procesos que **llegan todos en t = 0**, 
con ráfagas de CPU de P1 = 24, P2 = 3 y P3 = 3, atendidos en el orden P1, P2, P3:
![[Scheduling criteria 2026-09-29 20.15.43.excalidraw]]

| Proceso      | Ráfaga | Llegada | Fin | **Turnaround \| Retorno**  (fin − llegada) | **Waiting time \| Espera** (retorno − ráfaga) |
| ------------ | ------ | ------- | --- | ------------------------------------------ | --------------------------------------------- |
| P1           | 24     | 0       | 24  | 24                                         | 0                                             |
| P2           | 3      | 0       | 27  | 27                                         | 24                                            |
| P3           | 3      | 0       | 30  | 30                                         | 27                                            |
| **Promedio** |        |         |     | **27**                                     | **17**                                        |
Throughput: 3 procesos en 30 unidades = **0,1 procesos por unidad**. Utilización: **100%** (la CPU nunca estuvo parada).

Fijate lo que pasó: P2 y P3 duran solo 3, pero esperaron 24 y 27 porque tenían adelante a P1. El **orden** cambia muchísimo los promedios. Eso lo vas a comprobar vos en la pregunta 2.

#### Con qué se confunde
- **Waiting Time ≠ Turnaround**: el retorno incluye el tiempo **corriendo**; la espera, solo el tiempo **en la fila**.
- **Tiempo de espera ≠ tiempo bloqueado**: el tiempo en **blocked** (esperando E/S) **no** cuenta como espera. La espera es el tiempo en **ready**: podía correr y no le tocaba.
- **Las métricas chocan entre sí**: un quantum chico mejora el tiempo de respuesta pero empeora el throughput (más cambios de contexto). No se puede optimizar todo a la vez.


[^1]: Qué es un plazo
	Cada tarea tiene una **fecha límite** (_deadline_): un momento antes del cual **tiene que haber terminado**. Si termina antes, da igual cuánto antes. Si termina después, **falló**, aunque sea por un milisegundo.
	
	En términos de las métricas: lo que importa es que **el turnaround de cada tarea sea menor o igual que su plazo**. No que sea chico, ni el promedio: **cada una, todas las veces**.
