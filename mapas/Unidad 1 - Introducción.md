---
titulo: Unidad 1 - Introducción
tipo: mapa
unidad: 1 - Introducción
estado: en-progreso
creado: 2026-09-21
tags: []
---

# Unidad 1 - Introducción

## De qué va esta unidad

Qué es un sistema operativo y cómo hace el hardware para que el kernel pueda
controlar a los programas: modos de ejecución, interrupciones y llamadas al
sistema como única puerta de entrada al kernel.

## Conceptos en orden de dependencia

⭐ = aparece en las preguntas ejemplo del primer parcial (parcial: 2026-09-30).

#### Base de hardware
1. [[Sistema Operativo]] ⭐ — extended machine + resource manager (las "dos funciones principales")
2. [[RAM]] — array de bytes con address: donde viven instrucciones y datos
3. [[Arithmethic Logic Unit]] — el circuito que hace cuentas y comparaciones
4. [[CPU]] — registers + ALU + control unit + clock; el ciclo fetch/decode/execute
5. [[Kernel]] — la parte privilegiada y siempre residente del SO
6. [[System Programs]] — lo que viene con el SO pero no es kernel

#### Protección y entrada al kernel
7. [[Dual-mode operation]] ⭐ — el mode bit y las instrucciones privilegiadas
8. [[Interrupt]] ⭐ — el hardware le avisa a la CPU; se atiende en cualquier modo y pasa a kernel
9. [[Trap]] ⭐ — interrupción provocada por el programa (syscall, exception); diferencia con un call a subrutina
   - [[Exception]] — el trap no intencional: error de la instrucción, casi siempre mata al proceso
10. [[Interrupt vector table]] ⭐ — por qué los traps se identifican con un número y no con una dirección
11. [[Kernel stack]] ⭐ — por qué el kernel no usa la stack del proceso interrumpido
12. [[Timer]] — garantiza que el kernel recupere la CPU
13. [[System Calls]] ⭐ — la interfaz entre procesos y kernel (`read()`, `write()`, `exit()`…)

#### Aprovechar la CPU
14. [[Multiprogramming]] ⭐ — tener varios programas en memoria para no dejar la CPU ociosa
15. [[Multitasking (time sharing)]] — time sharing: alternar rápido para que parezca simultáneo

#### Resto de la unidad
16. [[Shell]] — el intérprete de comandos, un programa más
17. [[Bootstrap]] — cómo arranca la máquina y se carga el kernel
18. [[Linker and Loader]] — de código fuente a programa cargado en memoria
19. [[Virtual Machine]] — emular una máquina entera

## Conexiones con otras unidades

- Va hacia: [[Unidad 2 - Procesos]] — [[Interrupt]], [[Dual-mode operation]] y
  [[Timer]] son la base de [[Cambio de contexto]] y del [[Scheduler]].

## Dudas abiertas de esta unidad

- *(ver [[Dudas abiertas]])*

## Bibliografía de la unidad

- [[U1 - Introducción]]

Volver a [[Inicio]].
