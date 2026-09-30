---
creado: 2026-09-30
tags: [memoria, procesos]
---
**Pregunta:** ¿cómo funciona eso de que el SO le "saca" la memoria a un proceso mandándola a disco (swap) y después se la devuelve?

**Respuesta corta:** cuando la `=[[RAM]]` no alcanza, el SO copia la memoria de un proceso que no está corriendo a un área reservada del **disco** (el *swap space*), y usa esa RAM para otro. Cuando el primero tiene que volver a correr, el SO trae de vuelta su memoria del disco. El proceso no se entera: solo nota que anduvo más lento. Por eso la memoria es un **preemptable resource**.

> Esto es tema de la **Unidad 4 - Memoria** ("intercambio" en el programa). No entra en el primer parcial.

## 1. El problema que resuelve
La RAM es finita, pero puede haber más procesos de los que entran. Y muchos de ellos no están haciendo nada: están en estado **blocked** esperando E/S o al usuario (ver `=[[Process states]]`). Ocupan RAM sin usarla.

Idea: mover a disco lo que no se está usando, y liberar RAM para el que sí la necesita.

## 2. Swapping clásico: el proceso entero
Pensalo como mudar una caja a un depósito:

```
          RAM                              Disco (swap space)
 ┌─────────────────────┐              ┌─────────────────────┐
 │ Proceso A (running) │              │                     │
 │ Proceso B (blocked) │ ── swap out ─▶ │ copia de B          │
 │     (libre)         │              │                     │
 └─────────────────────┘              └─────────────────────┘
        ↑ ahora entra el proceso C en el lugar de B
```

**Swap out**, paso a paso:
1. El SO elige una "víctima": un proceso que no está corriendo, idealmente blocked.
2. Copia **todo su espacio de memoria** (código, datos, `=[[Heap]]`, `=[[Stack]]`) al swap space del disco.
3. En su `=[[Process Control Block (PCB)]]` anota que está "swapped out" y dónde quedó en el disco.
4. Esa RAM queda libre para otro proceso.

**Swap in**, cuando le toca correr de nuevo:
1. El SO busca RAM libre (si no hay, primero hace swap out de otro).
2. Copia la memoria del proceso desde el disco a la RAM.
3. El proceso sigue exactamente donde estaba: sus registros ya estaban guardados en el PCB, como en cualquier `=[[Cambio de contexto]]`.

**Por qué el proceso no se entera:** todo su estado se guardó (la memoria en disco, los registros en el PCB) y se restauró completo. Es la misma lógica que hace preemptable a la CPU, llevada a la memoria.

**Un detalle:** al volver, el proceso puede caer en **otra zona** de la RAM. Para que sus direcciones sigan funcionando hace falta que el hardware traduzca direcciones (registros base y límite, y después paginación). Eso es justamente lo que se ve en la Unidad 4.

## 3. Lo que se hace hoy: paginación (no el proceso entero)
Mover un proceso entero de varios GB es carísimo. Los SO modernos parten la memoria en pedacitos de tamaño fijo, las **páginas** (típicamente 4 KB), y mandan a disco **solo las páginas que hace rato no se usan**, aunque el proceso esté corriendo.

1. El proceso toca una dirección cuya página está en disco.
2. El hardware se da cuenta y genera una excepción: un **page fault** (ver `=[[Exception]]`).
3. El SO trae esa página del disco a la RAM (el proceso queda blocked mientras tanto).
4. Se reintenta la instrucción, y ahora funciona.

Desde el proceso sigue siendo invisible: solo ve una pausa.

## 4. El costo: el disco es lentísimo
| Dónde | Tiempo aproximado de un acceso |
|---|---|
| RAM | ~100 ns |
| SSD | ~100 µs (≈ 1.000 veces más lento) |
| Disco rígido | ~10 ms (≈ 100.000 veces más lento) |

Si hay tan poca RAM que el SO se la pasa mandando páginas a disco y trayéndolas de vuelta, la máquina casi no hace trabajo útil: se llama **thrashing**. Es lo que se siente cuando la compu "se arrastra" con mil pestañas abiertas.

## 5. Sin swap, la memoria deja de ser preemptable
En sistemas sin swap (muchos celulares, por ejemplo), el SO no tiene dónde guardar la memoria de un proceso. Entonces no se la puede "sacar y devolver": la única forma de liberar RAM es **matar** el proceso (por eso las apps del celular se cierran solas en segundo plano). Es el ejemplo de Tanenbaum de que preemptable **depende del contexto**.

En tu Mac podés ver cuánto swap se está usando con:
```
sysctl vm.swapusage
```

## Relación con la materia
- Es el ejemplo típico de **preemptable resource** (`=[[Resource]]`, Unidad 3).
- Se parece al `=[[Cambio de contexto]]`: guardar el estado completo para poder restaurarlo. Acá, además de los registros, se guarda la memoria.
- El page fault es una `=[[Exception]]` que maneja el `=[[Kernel]]`.
- Detalle completo (intercambio, paginación, archivo de swap): Unidad 4 - Memoria.
