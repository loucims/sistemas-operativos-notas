---
titulo: RAM
tipo: concepto
unidad: 1 - Introducción
estado: sin-empezar
requiere: []
habilita:
  - "[[CPU]]"
  - "[[Stack]]"
creado: 2026-09-22
tags:
  - memoria
---

La RAM (Random Access Memory) es un array gigante de bytes
cada uno con su propio address y contenido

| address   | 100      | 101      | 102      | 103      | 104      | 105      |     |
| --------- | -------- | -------- | -------- | -------- | -------- | -------- | --- |
| contenido | 00000001 | 00000002 | 00000001 | 01010101 | 01001001 | 01001001 |     |

### Que es una instruccion?
Una instruccion es una **orden chiquita codificada como numeros.** La cual empieza con un opcode (que son los comandos de assembly hardcodeados en el hardware que puede hacer el CPU).