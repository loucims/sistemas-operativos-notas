---
requiere:
  - "[[Round Robin]]"
  - "[[Starvation]]"
  - "[[Preemptive scheduling]]"
habilita:
  - "[[Real-time scheduling]]"
creado: 2026-09-30
---
### Qué problema resuelve
RR trata a todos los procesos igual, pero no todos son igual de importantes. El proceso que atiende tu mouse no puede esperar lo mismo que un backup que corre de fondo. 

El slide lo lista entre los compromisos del scheduler: **manejo de prioridades**, darle mejor servicio a los procesos más "importantes".

### Cómo funciona
A cada proceso se le asigna un número de **priority**, y el scheduler siempre elige al proceso ready de mayor prioridad.

En la práctica no hay una sola cola: hay **una ready queue por nivel de prioridad** (las _run queues_ del slide, cada una una linked list):

```
Prioridad 4 (alta)  → [P7] → [P2]        ← se atiende primero, RR adentro
Prioridad 3         → [P5]
Prioridad 2         → (vacía)
Prioridad 1 (baja)  → [P1] → [P3] → [P9]  ← solo corre si todo lo de arriba está vacío
```

Las reglas (slide "Planificación de prioridad"):

1. Siempre se elige de *la cola más alta que tenga procesos.*
2. Si esa cola está vacía, se sigue con la siguiente.
3. *Dentro* de una misma cola se usa **Round Robin**. Por eso RR era prerrequisito.

Puede ser **preemptive** o **non-preemptive**:

|                    | Si llega un proceso de mayor prioridad…                   |
| ------------------ | --------------------------------------------------------- |
| **Preemptive**     | Desaloja al que está corriendo en ese momento (como SRTN) |
| **Non-preemptive** | Espera a que el actual termine o se bloquee (como SJF)    |

En cuanto al `Timer` lo que dispara la preemption en priority no es el reloj. *Es un evento que hace que un proceso de mayor prioridad pase a ready*:

| Qué dispara al scheduler                                                         | ¿Necesita timer?                                                        |
| -------------------------------------------------------------------------------- | ----------------------------------------------------------------------- |
| Llega un proceso nuevo con más prioridad (creación)                              | No                                                                      |
| Un proceso de más prioridad vuelve de I/O (interrupción del disco o del teclado) | No, lo despierta la interrupción del dispositivo                        |
| RR **dentro** de una misma cola (**En este caso ei es el Timer**)                | **Sí**: el quantum es el timer                                          |
| Aging con contador por proceso                                                   | **Sí**: el slide dice que se incrementa "con una interrupción de reloj" |
Priority "puro" no necesita timer, sea preemptive (reacciona a eventos) o non-preemptive (solo decide cuando el actual termina o se bloquea). Pero la versión real, *con RR en cada cola y aging, sí lo usa.*

### El problema: starvation, y la solución: aging
Si siempre hay procesos en la cola 4, los de la cola 1 **nunca corren**. Eso es starvation.
La solución es **aging**: subirle la prioridad a quien lleva mucho esperando. El slide da dos esquemas:

| Situación       | Esquema                                                                                                          |
| --------------- | ---------------------------------------------------------------------------------------------------------------- |
| Pocos procesos  | Un contador por proceso que se incrementa con cada interrupción de reloj. Al pasar un límite, sube de prioridad. |
| Muchos procesos | Periódicamente se mueve cada **lista entera** al nivel de arriba.                                                |
Cambiar la prioridad de un proceso es simplemente moverlo a otra lista.

### Prioridades dinámicas (el ejemplo de Windows)
La prioridad no tiene por qué ser fija. Windows la ajusta (slide "Planificación en Windows I"):

- **Si un proceso gasta el quantum entero, baja de prioridad.** Se está comportando como CPU-bound.
- **El proceso de la ventana activa sube de prioridad.** Es el que el usuario está mirando.

Esto conecta con tu pregunta anterior: el I/O-bound que se bloquea antes del quantum no baja, y termina quedando arriba del CPU-bound. Eso compensa lo que RR le hacía perder.

### Con qué se confunde

| Confusión                            | Aclaración                                                                                                                                                 |
| ------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| "SJF y Priority son cosas distintas" | **SJF es un caso particular de priority scheduling**: la prioridad es el burst estimado (más corto = más prioridad). Por eso SJF también tiene starvation. |
| "Número más alto = más prioridad"    | Depende del sistema. En Unix/Linux (`nice`) un número **más bajo** es más prioridad. Leé siempre la convención del enunciado.                              |
| Aging vs Priority                    | Aging no es un algoritmo aparte: es un **parche** a priority scheduling para evitar starvation.                                                            |
| Starvation vs deadlock               | En starvation el proceso **podría** correr, pero siempre hay alguien más prioritario. En deadlock nadie puede avanzar.                                     |