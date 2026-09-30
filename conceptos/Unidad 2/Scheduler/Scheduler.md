---
requiere:
  - "[[Process states]]"
  - "[[Cambio de contexto]]"
habilita:
  - "[[Preemptive scheduling]]"
  - "[[Scheduling criteria]]"
creado: 2026-09-29
---
#### Qué problema resuelve

En `=[[Process states]]` viste la transición **ready → running**: _"el scheduler lo elige"_. Hasta ahora eso fue una caja negra. Pero casi siempre hay **varios** procesos ready y **una** CPU (o menos CPUs que procesos ready), y **a quién** se la das cambia muchísimo
- que la máquina se sienta rápida o lenta,
- que los dispositivos estén ocupados o parados (lo viste en `CPU-bound and I-O-bound`),
- que alguien sufra `Starvation`.

#### Cómo funciona
El scheduler **no es un proceso** que corre todo el tiempo: es **código del kernel** que se ejecuta en momentos puntuales. Cuando corre, mira la **ready queue** (la lista de procesos en estado ready) y elige uno, usando un **algoritmo de planificación** (FCFS, SJF, Round Robin…, los próximos conceptos). Después, el `Cambio de contexto` pone a correr al elegido.

```
 ready queue:  [ P3 ][ P7 ][ P1 ][ P9 ]
                    │
                    ▼  scheduler: "¿cuál?" ← según el algoritmo
                  elige P7
                    │
                    ▼  cambio de contexto
               CPU: corre P7
```

Hay que distinguir dos cosas:

|                                       | Qué hace                                                           |
| ------------------------------------- | ------------------------------------------------------------------ |
| **Política** (el algoritmo)           | Decide **a quién** le toca                                         |
| **Mecanismo** (el cambio de contexto) | **Hace** el cambio: guarda y carga registros, cambia la page table |
#### Cuándo se planifica (slide "En qué momentos se ejecuta la planificación")

| #   | Momento                                                | Transición           | Ejemplo                                                      |
| --- | ------------------------------------------------------ | -------------------- | ------------------------------------------------------------ |
| 1   | Un proceso se **bloquea**                              | running → blocked    | Hace `read()` del disco, o `wait()` esperando a un hijo      |
| 2   | Llega una **interrupción** y el proceso vuelve a ready | running → ready      | El `Timer` avisa que se acabó el quantum                     |
| 3   | Un proceso **se desbloquea**                           | blocked → ready      | Terminó su E/S. ¿Le conviene correr ya, antes que el actual? |
| 4   | Un proceso **termina**                                 | running → terminated | `exit()`                                                     |
En 1 y 4, la CPU **queda libre**. En 2 y 3, el proceso actual **podría seguir**. Esa diferencia es la base del próximo concepto (`Preemptive scheduling`), y te la pregunto en la pregunta 1.

#### Qué busca optimizar: depende del sistema (slide "Categorías")

|Tipo de sistema|Qué importa|Ejemplo|
|---|---|---|
|**Por lotes** (_batch_)|Terminar muchos trabajos por hora, que cada uno termine rápido, tener la CPU siempre ocupada|Procesar las liquidaciones de sueldo de noche|
|**Interactivo**|**Tiempo de respuesta**: que el usuario no espere|Tu editor, el navegador|
|**Tiempo real**|**Cumplir plazos**, ser predecible. _"Un proceso tardío puede ser un proceso inútil"_|Control de un avión, audio en vivo|
No hay un algoritmo "mejor" en general: cada uno optimiza cosas distintas. Eso es `Scheduling criteria`, que viene después

#### Con qué se confunde
- **Scheduler ≠ cambio de contexto**: el scheduler **elige**, el cambio de contexto **ejecuta** la decisión.
- **El scheduler no "corre al lado" de los procesos**: es código del kernel que se activa en esos 4 momentos, siempre después de un trap o una interrupción.
- Slide: _"Las aplicaciones no deben depender del comportamiento del planificador."_ Pregunta 2.