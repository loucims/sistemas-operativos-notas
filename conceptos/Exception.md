---
titulo: Exception
tipo: concepto
unidad: 1 - Introducción
estado: sin-empezar
requiere: ["[[Trap]]", "[[Dual-mode operation]]"]
habilita: []
creado: 2026-09-24
tags: [cpu]
---

Un [[Trap]] **no intencional**: la CPU detecta que la instrucción actual no se puede ejecutar y le pasa el control al kernel.

Ejemplos: división por cero, instrucción privilegiada en user mode, acceso a memoria que no es del proceso.

La instrucción **nunca llega a ejecutarse**. El handler del kernel decide qué hacer y casi siempre **mata al proceso** (en Linux: *segmentation fault*).

#### Exception != [[System Calls]]
Las dos son traps, pero la syscall es intencional (el programa *quiere* entrar al kernel) y vuelve a la instrucción siguiente. La exception no es buscada y normalmente no vuelve.

Volver a [[Unidad 1 - Introducción]]
