---
requiere:
  - "[[Scheduler]]"
  - "[[Timer]]"
habilita:
  - "[[Round Robin]]"
  - "[[Shortest Remaining Time Next]]"
creado: 2026-09-29
---
#### Qué problema resuelve
En la pregunta de `=[[Scheduler]]` viste que en los momentos 2 y 3 el scheduler **puede** elegir: dejar que siga el proceso actual o **sacarle** la CPU (*principalmente cuando suceden interrupciones)*. Esa decisión define los dos grandes tipos de planificación.

El problema de fondo: si el scheduler **nunca** le saca la CPU a nadie, *un proceso con un loop largo (o infinito) se queda con la CPU*, y todos los demás esperan. En una máquina interactiva, eso es la pantalla congelada. **Sacarle la CPU a un proceso que esta corriendo,** eso es **Preemptive scheduling**
#### Cómo funciona (slide U2_5, "Tipos de algoritmos de planificación")

|                                | **No apropiativo** (_nonpreemptive_)           | **Apropiativo** (_preemptive_)                                                                     |
| ------------------------------ | ---------------------------------------------- | -------------------------------------------------------------------------------------------------- |
| Cuánto corre un proceso        | _"Tanto tiempo como desee"_                    | _"Durante un tiempo fijo como máximo"_                                                             |
| Cuándo cambia                  | _"Sólo conmuta cuando se bloquea"_ (o termina) | Cuando se bloquea o termina, **y además** cuando se acaba su tiempo o llega alguien más importante |
| Momentos del scheduler que usa | Solo **1 y 4** (la CPU quedó libre)            | **1, 2, 3 y 4**                                                                                    |
| Qué necesita                   | Nada especial                                  | _"Una interrupción del reloj al final del intervalo"_: el `Timer`                                  |
| Cambios de contexto            | Solo **voluntarios**                           | Voluntarios **e involuntarios**                                                                    |
Qué dispara una **preemption** (sacarle la CPU a un proceso que podía seguir):
1. **Se le terminó el quantum**: el timer interrumpe y el proceso pasa running → **ready** (momento 2).
2. **Se despierta uno más importante**: termina la E/S de un proceso de mayor prioridad, que pasa a ready, y el scheduler decide que conviene que corra **ya** (momento 3).

En los dos casos, el proceso sacado va a **ready**, **no** a blocked. No está esperando nada: solo le tocó ceder.

> **Acá hay una confusión importante: no apropiativo ≠ interrupciones apagadas.** Las interrupciones **siguen funcionando** normalmente: el timer, el disco y el teclado interrumpen y el kernel las atiende. Lo único que cambia es que, después de atender la interrupción, el scheduler **no le saca la CPU al proceso**: vuelve al mismo.
> *Basicamente, el kernel no va a reemplazar el proceso actual por scheduler a menos que este se quiera ir* (a menos de que haya alguna Exception)
#### Ventajas y desventajas

|              | No apropiativo                                                                           | Apropiativo                                                                                                                                              |
| ------------ | ---------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| ✅            | Más simple. Menos cambios de contexto. El proceso no se interrumpe en momentos incómodos | Nadie puede acaparar la CPU. Buen tiempo de respuesta. Se puede priorizar a los urgentes                                                                 |
| ❌            | **Un solo proceso puede monopolizar la CPU** (a propósito o por un bug)                  | Más cambios de contexto (costo del cambio). Un proceso puede ser cortado **en cualquier instrucción**, así que hacen falta `Mutual exclusion` y compañía |
| Dónde se usa | Sistemas por lotes, algunos sistemas embebidos simples                                   | Todo sistema interactivo moderno (Linux, Windows, macOS)                                                                                                 |
Fijate la segunda desventaja: **las race conditions existen porque el scheduling es apropiativo** (o porque hay varias CPUs). Con un solo núcleo y sin preemption, un hilo nunca podría ser cortado entre el leer y el guardar del `x++`.

#### Con qué se confunde
- **Preemptive ≠ "bloquear"**: sacar a un proceso por preemption lo manda a **ready**. Bloquearse (running → blocked) lo hace el propio proceso, al pedir algo que tarda.
- **Recursos _preemptable_ vs scheduling _preemptive_**: en la Unidad 3 vas a ver "recursos apropiativos" (que se le pueden sacar a un proceso sin romper nada, como la CPU o la memoria). Es la misma palabra aplicada a otra cosa.
- **Planificación cooperativa**: el nombre que le dimos en `User-level thread` a los hilos que solo cambian cuando llaman a `thread_yield()`. Es planificación **no apropiativa**.

