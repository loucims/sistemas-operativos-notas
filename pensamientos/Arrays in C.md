---
creado: 2026-09-27
tags: [memoria]
---
**Pregunta:** cuando creás un array en C, ¿`a` es un pointer a la dirección del array? ¿Cómo funciona la definición de arrays?

**Respuesta corta:** no. `a` **es** los casilleros del array, no un casillero aparte que guarda una dirección. Pero cuando lo usás en una expresión, C lo convierte automáticamente en la dirección de su primer elemento.

## 1. Array vs pointer en memoria
```c
int a[3];                          // array en la stack
int *p = malloc(3 * sizeof(int));  // pointer a un bloque en el heap
```

```
 int a[3];                       int *p = malloc(...);

 STACK                           STACK             HEAP
 97 │ a[0]  ┐                    96 │ p = 50 ──────► 50 │ p[0]
 96 │ a[1]  │ esto ES "a"                           51 │ p[1]
 95 │ a[2]  ┘                                       52 │ p[2]
```

- `a` es el **nombre** de 3 casilleros seguidos. No hay ningún casillero extra.
- `p` es **un casillero aparte** (una variable) que **guarda** una dirección (50).

## 2. El "decay": cuando `a` se convierte en dirección
Cuando usás `a` en una expresión, C lo reemplaza por `&a[0]` (la dirección del primer elemento):
- `return a;` → devuelve la dirección 97.
- `f(a);` → la función recibe la dirección 97, no una copia del array.
- `int *q = a;` → `q` pasa a valer 97.

Por eso un array local no se puede devolver: lo que sale es la dirección de casilleros de la `=[[Stack]]` que mueren con la función.

## 3. Qué significa `x[i]`
En C, `x[i]` es exactamente `*(x + i)`: "andá a la dirección `x`, avanzá `i` casilleros y leé ahí". Por eso `a[i]` y `p[i]` se escriben igual aunque `a` y `p` sean cosas distintas.

(El avance es en **casilleros del tipo**, no en bytes: con `int` de 4 bytes, `x + 1` avanza 4 bytes.)

## 4. Prueba de que no son lo mismo
| | `a` (array) | `p` (pointer) |
|---|---|---|
| `sizeof` | 12 (3 ints × 4 bytes) | 8 (tamaño de una dirección) |
| ¿Se puede reasignar? | No: `a = otra_cosa;` no compila | Sí: `p = otra_direccion;` |
| ¿Dónde están los datos? | En los mismos casilleros de `a` | En otro lado (a donde apunta) |

## Relación con la materia
- Ver `=[[Heap]]`: un array que tiene que sobrevivir a la función, o cuyo tamaño se decide mientras corre el programa, se pide con `malloc`.
- Ver también `pensamientos/Pointers`.
