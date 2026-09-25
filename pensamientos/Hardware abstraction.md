---
creado: 2026-09-25
tags: [e-s, cpu]
---
**Pregunta:** ¿el kernel tiene que conocer el hardware específico (una RAM en particular, un disco en particular)? ¿Cómo se abstrae eso para poder escribir un kernel?

**Respuesta corta:** sí sabe de hardware, pero no de todo al mismo nivel. Hay tres casos.

## 1. La CPU (arquitectura)
- Sí tiene que saber: x86-64 y ARM tienen instrucciones, traps y registros especiales distintos.
- Casi todo el kernel está en C genérico. Solo una parte chica es específica de la arquitectura: entrada de traps, cambio de contexto, mode bit, `=[[Interrupt vector table]]`. En Linux vive en `arch/x86`, `arch/arm64`, etc.
- Para llevar el kernel a otra CPU se reescribe esa parte chica y el resto se recompila.

## 2. La RAM
- No necesita saber marca ni modelo. Los detalles eléctricos los maneja el controlador de memoria (hardware) y el firmware.
- Solo necesita saber cuánta RAM hay y en qué direcciones. Se lo informa el firmware durante el `=[[Bootstrap]]` con un "mapa de memoria".
- Para el kernel la `=[[RAM]]` es un array de bytes con dirección: el hardware ya hizo la abstracción.

## 3. Los dispositivos (disco, red, video, teclado...)
- Acá está el problema real: miles de modelos, cada uno se controla distinto.
- **Device driver**: código del kernel que sabe manejar un dispositivo específico y le ofrece al resto del kernel una interfaz estándar ("leé el bloque N", "mandá este paquete").
- Muchos dispositivos siguen estándares (NVMe para SSD, USB para teclado y mouse), así que un driver genérico sirve para muchas marcas.
- Al arrancar, el kernel recorre los buses (ej. PCI). Cada dispositivo responde con un número de fabricante y modelo, y el kernel elige el driver que corresponde.

## Las capas de un `read()`
1. Programa: `read(fd, buffer, 100)` → "quiero bytes de este archivo".
2. `=[[System Calls]]`: entra al kernel.
3. Sistema de archivos: "bytes 0 a 100 de notas.txt" → "bloque 5082 del disco".
4. Capa genérica de discos: "leé el bloque 5082" (igual para cualquier disco).
5. Driver (ej. NVMe): lo traduce a los comandos exactos de ese SSD.
6. Hardware: lee y avisa con un `=[[Interrupt]]` al terminar.

Solo el paso 5 conoce el hardware concreto.

## Relación con la materia
- Es la idea de extended machine de `=[[Sistema Operativo]]`: los programas ven archivos, no sectores de un SSD de tal marca.
- **HAL (Hardware Abstraction Layer)**: nombre que usa Windows para esta capa. Misma idea.
- xv6 casi no tiene drivers: "una sola terminal, un solo disco" (`U1_xv6.pdf`). Por eso en `main.c` hay un único `virtio_disk_init()`.
