---
creado: 2026-09-29
tags: [concurrencia]
---
**Pregunta:** ¿qué es eso de que el buffer del `=[[Producer-consumer problem]]` es circular, con `in` y `out`?

**Respuesta corta:** es una forma de guardar una fila (el primero que entra es el primero que sale) en un array de tamaño fijo **sin mover nunca los items**: en vez de correr los datos, se mueven dos índices que, al llegar al final, vuelven al 0.

## 1. El problema que resuelve
El productor pone al final y el consumidor saca del principio. Con un array común, al sacar el primero queda un hueco adelante:

```
 saco A del principio:  [ A ][ B ][ C ][   ][   ]
                    →   [   ][ B ][ C ][   ][   ]   ← hueco
```

Para no dejar huecos habría que correr todos los demás un lugar cada vez que sacás uno. Es lento. La solución: **no mover los items, mover los índices**.

## 2. Cómo funciona
- **`in`**: el próximo casillero donde el productor **pone**.
- **`out`**: el próximo casillero de donde el consumidor **saca**.

Cada uno avanza de a uno y, cuando llega al final, **vuelve al 0**, como un reloj de N posiciones.

```c
// ──────── poner (productor) ────────
buffer[in] = item;
in = (in + 1) % N;          // % N: si llega a N, vuelve a 0

// ──────── sacar (consumidor) ───────
item = buffer[out];
out = (out + 1) % N;
```

## 3. Paso a paso (N = 5, casilleros 0 a 4)
**1. El productor pone A, B, C**
```
 casillero:   0    1    2    3    4
            [ A ][ B ][ C ][   ][   ]
              ↑               ↑
             out              in            count = 3
```

**2. El consumidor saca A** → lee `buffer[0]` y `out` pasa a 1. A no se borra ni se mueve nada: ese casillero queda libre para reusar.
```
            [ · ][ B ][ C ][   ][   ]
                   ↑          ↑
                  out         in            count = 2
```

**3. El productor pone D y E** → usa los casilleros 3 y 4. `in` llega a 5, y `5 % 5 = 0`, así que **vuelve al 0**.
```
            [ · ][ B ][ C ][ D ][ E ]
              ↑    ↑
              in  out                       count = 4
```

**4. El productor pone F** → va al casillero **0**, que había quedado libre. Dio la vuelta.
```
            [ F ][ B ][ C ][ D ][ E ]
                   ↑
                in = out                    count = 5  → LLENO
```

El orden de salida sigue siendo el de llegada: B, C, D, E, F.

## Relación con la materia
- `in` lo toca solo el productor y `out` solo el consumidor, pero **`count` lo tocan los dos**: es `=[[Region critica]]`.
- **Lleno** = `count == N` → el productor espera. **Vacío** = `count == 0` → el consumidor espera. Son las dos condiciones de sincronización del problema.
- Ojo: con `in == out` no alcanza para saber si está lleno o vacío (pasa en los dos casos). Por eso se usa `count`.
