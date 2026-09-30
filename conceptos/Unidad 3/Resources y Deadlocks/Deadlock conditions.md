---
requiere:
  - "[[Deadlock]]"
habilita:
  - "[[Resource allocation graph]]"
  - "[[Deadlock prevention]]"
creado: 2026-09-30
---
#### Qué problema resuelve
La definición de `Deadlock` te dice **cómo reconocer** un deadlock cuando ya pasó. Pero para **prevenirlo** necesitás saber **qué ingredientes lo hacen posible**. Coffman (1971) los encontró: son **4 condiciones**, y **tienen que cumplirse las 4 a la vez**. Si falta una sola, el deadlock es imposible.

#### Las 4 condiciones (p.33)

Se pueden leer como una historia, en orden:

| #   | Condición            | Slide                                                                           | En criollo                                               | Dónde la viste                                   |
| --- | -------------------- | ------------------------------------------------------------------------------- | -------------------------------------------------------- | ------------------------------------------------ |
| 1   | **Mutual exclusion** | _"Cada recurso está disponible o asignado a un solo proceso."_                  | Un resource lo usa **uno a la vez**                      | Es la definición misma de `Resource`             |
| 2   | **Hold and wait**    | _"Los procesos que poseen recursos pueden solicitar más."_                      | **Me quedo** con lo que tengo **y pido más**             | A tiene `resource1` y hace `down(&resource2)`    |
| 3   | **No preemption**    | _"No se pueden retirar recursos de un proceso involuntariamente."_              | **Nadie me lo puede sacar**: lo suelto solo si yo quiero | Resource **nonpreemptable** (impresora, Blu-ray) |
| 4   | **Circular wait**    | _"Debe haber una cadena de dos o más procesos esperando al siguiente miembro."_ | Las esperas **se cierran en círculo**                    | El filósofo 4 cierra el círculo pidiendo 4 → 0   |
Si lo aplicás al ejemplo cruzado de A y B, ves las 4 condiciones a la vez:

```
1. Mutual exclusion:  resource1 es solo de A, resource2 es solo de B
2. Hold and wait:     A tiene resource1 y espera resource2 (B igual, al revés)
3. No preemption:     nadie puede sacarle resource1 a A; solo A hace el up
4. Circular wait:     A → espera a B → espera a A → ...

   ┌──────── A espera lo que tiene B ────────┐
   │                                         ▼
   A                                         B
   ▲                                         │
   └──────── B espera lo que tiene A ────────┘
```

#### Por qué son "precondiciones" (p.34)

Son **condiciones necesarias**: el deadlock **no puede existir** sin las 4. De ahí sale el "secreto" de la slide:

> _"Consiste simplemente eliminar cualquiera de las cuatro precondiciones. No interesa cual. Entonces el bloqueo no puede suceder."_

Esa es la base de `Deadlock prevention` (⭐12), que ataca las condiciones de a una. Cada ataque tiene su dificultad, y el parcial pregunta por cada una.

#### Con qué se confunde

- **La condición _mutual exclusion_ vs `Mutual exclusion` de `Region critica`**: es la **misma idea**, uno a la vez. Pero en la Unidad 2 era algo que **queríamos** garantizar, y acá es un **ingrediente** del deadlock. Por eso es tan difícil de eliminar: muchas veces la necesitás sí o sí.
- **No preemption vs `Preemptive scheduling`**: la condición habla de **sacarle un resource** a un proceso, y el scheduling habla de **sacarle la CPU**. El scheduling preemptive rompe esta condición, pero **solo para la CPU**. Por eso nunca hay deadlock por la CPU.
- **Circular wait vs `Deadlock`**: circular wait es **una** de las 4 condiciones, la "forma" que toma el deadlock. Si las otras 3 también se cumplen, el ciclo **es** el deadlock.