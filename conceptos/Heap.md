---
requiere:
  - "[[RAM]]"
  - "[[Process]]"
habilita: []
creado: 2026-09-27
---
La `Stack` es cómoda, porque se reserva y se libera sola, pero tiene tres límites:

1. **Muere con la función.** Cuando la función hace `ret`, su frame se desapila. No podés crear algo en una función y que siga vivo después.
2. **El tamaño tiene que conocerse de antemano.** El compilador cuenta cuántos casilleros necesita cada frame. Si el tamaño lo decide el usuario mientras el programa corre ("¿cuántos alumnos querés cargar?"), la stack no sirve.
3. **Es chica.** Unos pocos MB (lo viste con las guard pages). Un array de 100 millones de elementos no entra.

El **heap** es la zona de memoria para eso: **memoria que pedís y liberás vos, cuando quieras y del tamaño que quieras**, y que vive hasta que la liberes. Es *dinamica*.

#### Cómo funciona
- El heap está en el address space, arriba de _data/bss_, y **crece hacia arriba**, hacia la stack (tu nota `Stack` ya lo menciona).
- **`malloc(n)`**: "necesito `n` bytes". Te devuelve la **dirección** de un bloque libre de ese tamaño. Es un pointer (tu nota `Pointers`).
- **`free(p)`**: "ya no uso el bloque que empieza en `p`". Ese bloque queda libre para otro `malloc`.
- Quien lleva la cuenta de qué bloques están libres y cuáles ocupados es el **allocator**, una parte de la librería de C que corre en user mode, no el kernel.
- **Conexión con la materia**: si el allocator se queda sin lugar, le pide más memoria al kernel con una syscall. En xv6 es `sbrk()`, y lo que crece es el campo `sz` del `Process Control Block`: el tamaño de la memoria del proceso.

#### Stack vs heap

|                        | Stack                         | Heap                                            |
| ---------------------- | ----------------------------- | ----------------------------------------------- |
| Quién reserva y libera | Solo, con cada `call` / `ret` | Vos, con `malloc` / `free`                      |
| Cuánto vive            | Hasta que la función retorna  | Hasta que hacés `free` (o termina el proceso)   |
| Tamaño                 | Fijo, lo cuenta el compilador | Lo decidís mientras corre el programa           |
| Tamaño total           | Chico (MB)                    | Grande (hasta lo que dé la RAM)                 |
| Velocidad              | Muy rápido (solo mueve el SP) | Más lento (el allocator tiene que buscar lugar) |
| Con hilos              | **Una por hilo**              | **Uno solo, compartido** entre todos los hilos  |

#### Ejemplo paso a paso
```c
int *crear_array(int n) {
    int *p = malloc(n * sizeof(int));   // pido lugar para n                                           ints en el heap
    for (int i = 0; i < n; i++)
        p[i] = i * 10;
    return p;                           // devuelvo la                                                DIRECCIÓN del bloque
}

int main() {
    int *arr = crear_array(3);
    printf("%d\n", arr[2]);             // 20
    free(arr);
}
```
Uso los mismos casilleros que en tu nota `Stack`: **stack** del 100 para abajo y **heap** del 50 para arriba. Cada casillero es un `int`.

---

**Paso 1: main llama a `crear_array(3)`**
- **Qué hace:** se apila el frame de `crear_array`, con lugar para `n` y `p`.
- **Por qué:** `n` y `p` son variables locales: viven en la stack.
```
 STACK                          HEAP
 99 │ arr = ?      (main)        50 │ (libre)
 98 │ 📝 volver a main           51 │ (libre)
 97 │ n = 3        ┐ crear_      52 │ (libre)
 96 │ p = ?        ┘ array ← sp
```

**Paso 2: `malloc(3 ints)`**
- **Qué hace:** el allocator busca 3 casilleros libres seguidos en el heap, los marca como ocupados y devuelve la dirección del primero: **50**. Se guarda en `p`.
- **Por qué:** `p` no es el array: es una variable **en la stack** que guarda **dónde está** el array en el heap.
```
 STACK                          HEAP
 97 │ n = 3                      50 │ ?   ┐
 96 │ p = 50  ──────────────────► 51 │ ?   │ bloque ocupado
                                 52 │ ?   ┘
```

**Paso 3: el loop llena el bloque**
- **Qué hace:** `p[i]` significa "el casillero `p + i`". Escribe en el 50, el 51 y el 52.
- **Por qué:** se escribe **a través de la dirección**, así que los datos quedan en el heap, no en la stack.
```
                                 50 │ 0
 96 │ p = 50  ──────────────────► 51 │ 10
                                 52 │ 20
```

**Paso 4: `return p`**
- **Qué hace:** copia el **50** al 📦 registro de resultado y desapila el frame de `crear_array`. `n` y `p` pasan a ser basura.
- **Por qué:** el frame muere, pero **el heap no se toca**. El bloque 50–52 sigue ocupado y con los datos. Esta es la diferencia clave con la stack.
```
 STACK                          HEAP
 99 │ arr = ?      ← sp          50 │ 0
 97 │ (basura)                   51 │ 10    ← sigue vivo
 96 │ (basura)                   52 │ 20
```

**Paso 5: main guarda el resultado en `arr`**
```
 99 │ arr = 50  ─────────────────► 50 │ 0
                                  51 │ 10
                                  52 │ 20
```

**Paso 6: `arr[2]`**
- **Qué hace:** va al casillero 50 + 2 = **52** y lee **20**.

**Paso 7: `free(arr)`**
- **Qué hace:** el allocator marca 50–52 como **libres**.
- **Por qué:** como en la stack, **liberar ≠ borrar**. El 20 sigue ahí hasta que otro `malloc` reuse ese lugar. Y `arr` **sigue valiendo 50**: apunta a memoria que ya no es tuya (un _dangling pointer_).

### Con qué se confunde / errores típicos
- **Devolver la dirección de una variable local**: si `crear_array` usara `int a[3];` y devolviera `a`, estaría devolviendo una dirección **de su stack**, que muere en el paso 4. Por eso existe el heap.
- **Memory leak**: hacer `malloc` y nunca `free`. El heap crece y crece. Igual, cuando el proceso termina, el kernel libera **todo** su address space (`Process termination`), así que el leak se "arregla" al salir.
- **Use after free**: usar `arr` después del `free`. Puede funcionar de casualidad (el dato sigue ahí) hasta que otro `malloc` pisa ese lugar.
- **El pointer no es el dato**: `arr` vive en la stack y solo guarda un número (50). El array vive en el heap.