---
titulo: Unidad 2 - Procesos
tipo: mapa
unidad: 2 - Procesos
estado: en-progreso
creado: 2026-09-21
tags: [procesos]
---

# Unidad 2 - Procesos

## De qué va esta unidad

Cómo el SO convierte un programa (algo estático en disco) en un proceso (algo
que avanza en el tiempo), y cómo hace para que muchos procesos parezcan correr a
la vez sobre menos CPUs de las que harían falta.

## Conceptos en orden de dependencia

1. [[Proceso]] — la abstracción base: programa en ejecución con su propio estado
2. [[PCB]] — dónde guarda el SO ese estado cuando el proceso no está corriendo
3. [[Estados de un proceso]] — nuevo, listo, ejecutando, bloqueado, terminado
4. [[Cambio de contexto]] — el mecanismo que permite alternar entre procesos
5. [[Planificador]] — quién decide a cuál de los listos le toca
6. [[Hilo]] — dividir un proceso en varios flujos de ejecución

## Conexiones con otras unidades

- Viene de: [[Interrupción]] y [[Modo kernel y modo usuario]] (Unidad 1)
- Va hacia: concurrencia y sincronización (Unidad 3), que asume procesos e hilos

## Dudas abiertas de esta unidad

- *(ver [[Dudas abiertas]])*

## Bibliografía de la unidad

- *(pendiente: cargar en `referencias/`)*

Volver a [[Inicio]].
