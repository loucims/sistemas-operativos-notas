---
titulo: Unidad 3 - Recursos y bloqueos
tipo: mapa
unidad: 3 - Recursos y bloqueos
estado: en-progreso
creado: 2026-09-22
tags: [concurrencia]
---

# Unidad 3 - Recursos y bloqueos

## De qué va esta unidad

Qué pasa cuando varios procesos compiten por recursos que no se pueden compartir:
cómo se llega a un deadlock, y las tres formas de enfrentarlo — detectar y
recuperar, evitar (estados seguros) o prevenir (romper una condición).

## Conceptos en orden de dependencia

⭐ = aparece en las preguntas ejemplo del primer parcial (parcial: 2026-09-30).

#### El problema
1. [[Resource]] ⭐ — qué es un recurso; preemptable vs nonpreemptable, con ejemplos
2. [[Resource acquisition]] ⭐ — request → use → release; pedir dos recursos sin bloquearse
3. [[Deadlock]] ⭐ — un conjunto de procesos esperando algo que solo otro del conjunto puede dar
4. [[Deadlock conditions]] ⭐ — mutual exclusion, hold and wait, no preemption, circular wait
5. [[Resource allocation graph]] — modelar asignaciones y pedidos como grafo; ciclo = deadlock

#### Las estrategias
6. [[Ostrich algorithm]] ⭐ — ignorar el problema, y por qué a veces es razonable
7. [[Deadlock detection]] ⭐ — buscar ciclos / matrices; el problema práctico de cuándo correrlo
8. [[Deadlock recovery]] ⭐ — preemption, rollback, matar procesos
9. [[Safe state]] ⭐ — existe algún orden en que todos terminan
10. [[Banker's algorithm]] ⭐ — solo conceder pedidos que dejan un safe state; secuencia de estados
11. [[Deadlock avoidance]] ⭐ — decidir cada pedido mirando el futuro (usa [[Safe state]])
12. [[Deadlock prevention]] ⭐ — atacar cada una de las 4 condiciones y sus dificultades
13. [[Two-phase locking]] — eliminar hold and wait: tomar todo o soltar todo

#### Parientes del deadlock
14. [[Livelock]] ⭐ — nadie está bloqueado pero nadie avanza; diferencia con deadlock
15. [[Starvation]] ⭐ — viene de [[Unidad 2 - Procesos]]; diferencia con deadlock

## Conexiones con otras unidades

- Viene de: [[Mutual exclusion]], [[Semaphore]], [[Dining philosophers problem]] ([[Unidad 2 - Procesos]])

## Dudas abiertas de esta unidad

- *(ver [[Dudas abiertas]])*

## Bibliografía de la unidad

- *(pendiente: cargar en `referencias/` las slides U3_1 y U3_2)*

Volver a [[Inicio]].
