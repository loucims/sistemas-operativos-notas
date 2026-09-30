---
titulo: Unidad 2 - Procesos
tipo: mapa
unidad: 2 - Procesos
estado: en-progreso
creado: 2026-09-21
tags: [procesos, concurrencia, cpu]
---
 
# Unidad 2 - Procesos

## De qué va esta unidad

Cómo el SO convierte un programa (algo estático en disco) en un proceso (algo
que avanza en el tiempo), cómo hace para que muchos procesos e hilos parezcan
correr a la vez, cómo se coordinan cuando comparten datos, y quién decide a
cuál le toca la CPU.

## Conceptos en orden de dependencia

⭐ = aparece en las preguntas ejemplo del primer parcial (parcial: 2026-09-30).

#### Procesos
1. [[Process]] ⭐ — programa en ejecución con su propio espacio de direcciones; diferencia con programa
2. [[Stack]] — la región de memoria de llamadas y variables locales (text / data / heap / stack)
3. [[Process Control Block (PCB)]] — dónde guarda el SO el estado de un proceso que no está corriendo
4. [[Process states]] ⭐ — running, ready, blocked (+ new, terminated) y sus transiciones
5. [[Fork]] ⭐ — crear un proceso copiando al padre; qué imprime `value` después del fork
6. [[Exec]] ⭐ — reemplazar la imagen del proceso; por qué no retorna si tiene éxito
7. [[Process Termination]] ⭐ — `exit()`, error, error fatal, matado por otro
8. [[Cambio de contexto]] ⭐ — cómo se inicia; voluntario vs involuntario
9. [[CPU-bound and I-O bound]] ⭐ — qué tipo de proceso tiene más cambios de contexto de cada clase

#### Hilos
10. [[Thread]] ⭐ — qué es propio del hilo (registros, PC, stack, estado) y qué comparte el proceso
11. [[User-level thread]] — hilos manejados por una librería, el kernel no los ve
12. [[Kernel-level thread]] — hilos que el kernel planifica
13. [[POSIX threads]] — la API `pthread_create` / `pthread_join`

#### Región crítica
14. [[Race condition]] ⭐ — resultado depende del orden de ejecución; "siempre / a veces / nunca"
15. [[Region critica]] ⭐ — el pedazo de código que toca datos compartidos
16. [[Mutual exclusion]] ⭐ — que nunca haya dos adentro de la critical section
17. [[Busy waiting]] ⭐ — esperar girando en un loop; vs bloquearse
18. [[Disabling interrupts]] ⭐ — exclusión mutua apagando interrupciones y sus inconvenientes
19. [[Lock variable]] — la solución ingenua que tiene su propia race condition
20. [[Strict alternation]] ⭐ — turnos con variable `turn`; pseudocódigo para 3 procesos
21. [[Peterson's solution]] — exclusión mutua por software para 2 procesos
22. [[Test-and-set]] ⭐ — instrucción atómica TSL; ¿tiene que ser privilegiada?
23. [[Compare-and-swap]] — la otra instrucción atómica (ejemplo `incrcas.c`)

#### Sincronización sin espera activa
24. [[Sleep and wakeup]] — bloquearse en vez de girar; el problema del wakeup perdido
25. [[Producer-consumer problem]] ⭐ — buffer acotado compartido
26. [[Semaphore]] ⭐ — contador + down/up atómicos; a quién despierta; implementarlo con interrupciones apagadas
27. [[Mutex]] ⭐ — semáforo binario para exclusión mutua
28. [[Monitor]] — exclusión mutua provista por el lenguaje
29. [[Message passing]] — comunicarse sin memoria compartida
30. [[Barrier]] — esperar a que todos lleguen a un punto
31. [[Readers-writers problem]] ⭐ — a quién le da preferencia una solución; inanición
32. [[Dining philosophers problem]] — el clásico de deadlock entre procesos
33. [[Starvation]] ⭐ — un proceso que nunca consigue avanzar

#### Planificación
34. [[Scheduler]] ⭐ — quién decide a cuál de los ready le toca; cuándo planificar
35. [[Preemptive scheduling]] ⭐ — expropiativo vs no expropiativo
36. [[Scheduling criteria]] ⭐ — waiting time, turnaround, throughput, uso de CPU
37. [[First-Come First-Served]] ⭐ — FCFS
38. [[Shortest Job First]] ⭐ — SJF, óptimo en tiempo medio de espera
39. [[Shortest Remaining Time Next]] — SRTN, la versión expropiativa de SJF
40. [[Round Robin]] ⭐ — turno circular; el tamaño del quantum
41. [[Priority Scheduling]] ⭐ — colas por prioridad para no recorrer toda la ready queue
42. [[Real-time scheduling]] ⭐ — procesos periódicos; ¿es planificable? (∑ C/P ≤ 1)

## Conexiones con otras unidades

- Viene de: [[Interrupt]], [[Trap]], [[Dual-mode operation]] y [[Timer]] ([[Unidad 1 - Introducción]])
- Va hacia: [[Unidad 3 - Recursos y bloqueos]] — [[Mutual exclusion]], [[Semaphore]] y
  [[Starvation]] son la base de [[Deadlock]]

## Dudas abiertas de esta unidad

- *(ver [[Dudas abiertas]])*

## Bibliografía de la unidad

- *(pendiente: cargar en `referencias/` las slides U2_1 a U2_5)*

Volver a [[Inicio]].
