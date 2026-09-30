---
requiere:
  - "[[Deadlock conditions]]"
habilita:
  - "[[Deadlock detection]]"
  - "[[Deadlock avoidance]]"
creado: 2026-09-30
---
#### Qué problema resuelve
Con 2 procesos ves el deadlock a ojo. Con 50 procesos y 30 resources, no. Hace falta un **modelo** donde la condición _circular wait_ se pueda **ver**, y que una computadora pueda **chequear**. La idea es convertir "quién tiene qué y quién espera qué" en un **grafo**, y buscar un **ciclo**.

#### Cómo funciona (p.6–8)

- **Círculo = proceso**. **Cuadrado = resource**.
-  *Hay dos tipos de flecha:*
```
   [R] ──▶ (A)      R está ASIGNADO a A       "R le pertenece a A"
   (A) ──▶ [S]      A está BLOQUEADO esperando S
```

> Son *fotos de un instante* dentro del sistema. Es un **instante de transicion**.

![[Resource allocation graph - A y B.excalidraw|900]]

#### La regla (p.13)

> _"Sin lazos == no hay interbloqueos. Con lazos == hay interbloqueos."_

Buscar ciclos en un grafo es un problema clásico, con algoritmos conocidos (p.3).

⚠️ **Esta regla vale cuando hay una sola instancia de cada resource** (una impresora, un escáner). Con **varias instancias** del mismo tipo (3 impresoras iguales), un ciclo **no alcanza** para asegurar un deadlock, y se usan matrices en lugar del grafo (p.16–17). Eso lo vemos en `Deadlock detection`.

#### Con qué se confunde
- **La dirección de las flechas**: es el error típico. **R → P = tiene**, **P → R = espera**.
- **Cadena vs ciclo**: una cadena de esperas que **termina** en alguien que corre (tu P1 → P2 → P3 de antes) **no** es deadlock. Tiene que **volver al principio**.
^^^^^^^EL MAS IMPORTANTE
