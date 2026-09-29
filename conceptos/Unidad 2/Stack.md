---
requiere:
  - "[[RAM]]"
habilita:
  - "[[Kernel stack]]"
creado: 2026-09-24
---
Cada programa tiene una zona de su memoria llamada **stack** (pila). Ahi se guarda lo que necesita cada *llamada a una funcion* mientras esa funcion se esta ejecutando:

- la *direccion de retorno* (a donde volver cuando la funcion termina),
- sus *variables locales*,
- y *registros guardados*

Un registro de la `=[[CPU]]`, el **stack pointer (SP)**, apunta al tope de la pila.
- Cuando llamas a una funcion, se apila un bloque nuevo, y el **SP** se mueve.
- Cuando la funcion retorna, se desapila y el **SP** *vuelve a donde estaba*.

El **Stack** crece *hacia direcciones mas bajas por convencion*. Tanto el stack como el `=[[Heap]]` crecen uno hacia el otro y asi comparten el espacio libre del medio.

Tambien se utilizan algunos **registros del CPU** para ayudar con cosas como returns y argumentos de funcion
- **El registro de argumento**: sirve para pasarle un dato a una funcion.
- **El registro de resultado**: sirve para devolver un dato

### Por que una *pila?* (LIFO o Last in First Out)
Por que las funciones terminan en ese orden, la ultima que llamaste es la primera que retorna. Lo cual es exactamente lo que hace una pila. Y como cada llamada tiene *su propio frame*, la recursion funciona sola, cada llamada tiene su propia copia de *sus variables y su propia direccion de retorno*.

### Stack vs Heap
En la stack, la memoria se reserva y se libera sola con cada `call`/`ret`, y vive solo mientras la funcion esta en ejecucion. En el heap la pedis y la liberas vos (`malloc`/`free`)

### Desapilar != borrar.
Desapilar solo *mueve el stack pointer*. El dato queda en memoria hasta que lo pisen.

### Ejemplo paso a paso
```c
int g(int x) { int y = x * 2; return y; }
int f()      { int a = 5; int r = g(a); return r + 1; }
// main() llama a f()
```
Supongamos que el sp arranca en `0x7000`.
**1. main hace `call f`:** se apila la dirección de retorno a main y f reserva lugar para `a` y `r`.
```
dirección   contenido
0x6FF8      ret → main     ┐
0x6FF0      a = 5          │ frame de f
0x6FE8      r = ?          ┘ ← sp
```
**2. f hace `call g`:** se apila la dirección de retorno a f y g reserva lugar para `y`.
```
0x6FF8      ret → main     ┐
0x6FF0      a = 5          │ frame de f
0x6FE8      r = ?          ┘
0x6FE0      ret → f        ┐ frame de g
0x6FD8      y = 10         ┘ ← sp
```
**3. g hace `ret`:** libera `y` (sube el sp), hace pop de `ret → f` y vuelve a f con el resultado.
```
0x6FF8      ret → main     ┐
0x6FF0      a = 5          │ frame de f
0x6FE8      r = 10         ┘ ← sp
0x6FE0      ret → f          ← basura: sigue en RAM, pero ya "no existe"
0x6FD8      y = 10           ← basura
```
**4. f hace `ret`:** vuelve a main y el sp queda de nuevo en `0x7000`.




### Segundo ejemplo paso a paso
```c
int doble(int x) {
    int y = x * 2;
    return y;
}

int main() {
    int a = 5;
    int b = 7;
    int r = doble(a);
}
```
Uso casilleros numerados del 100 para abajo, porque la stack crece hacia números más chicos. Cuando digo **registro**, me refiero a una cajita _dentro de la CPU_, no en la RAM. Voy a usar dos:
- 📨 **registro de argumento**: sirve para pasarle un dato a una función.
- 📦 **registro de resultado**: sirve para devolver un dato.
---

**Paso 1: main reserva 3 casilleros**

- **Qué hace:** mueve el sp de 100 a 97.
- **Por qué:** necesita lugar para `a`, `b` y `r`. El compilador ya contó que son 3.

```
100 │ (cosas de antes)
 99 │ a = ?
 98 │ b = ?
 97 │ r = ?          ← sp
```

**Paso 2: main guarda a = 5 y b = 7**

- **Qué hace:** escribe 5 en el 99 y 7 en el 98.
- **Por qué:** sabe que `a` está 2 lugares arriba del sp (97 + 2 = 99) y `b` está 1 lugar arriba (98).

```
 99 │ a = 5
 98 │ b = 7
 97 │ r = ?          ← sp
```

**Paso 3: main pone el 5 en 📨**

- **Qué hace:** copia `a` (5) al registro de argumento.
- **Por qué:** `doble` necesita recibir ese 5, y un registro es la forma más rápida de pasárselo. La memoria no cambia.

**Paso 4: call doble. Main deja anotado a dónde volver**

- **Qué hace:** baja el sp a 96 y escribe en el 96 la nota "volver a main, a la parte donde se guarda r". Después salta a `doble`.
- **Por qué:** `doble` no sabe quién la llamó. Sin esa nota, cuando termine no sabría a dónde volver. Esta nota es la **return address**.

```
 97 │ r = ?
 96 │ 📝 volver a main  ← sp
```

**Paso 5: doble reserva 1 casillero y calcula y**

- **Qué hace:** baja el sp a 95, lee el 5 de 📨 y escribe `y = 10` en el 95.
- **Por qué:** `y` es una variable local de `doble` y necesita su propio lugar.

```
 96 │ 📝 volver a main
 95 │ y = 10         ← sp
```

**Paso 6: doble pone el 10 en 📦**

- **Qué hace:** copia `y` (10) al registro de resultado.
- **Por qué:** los casilleros de `doble` están por liberarse. El resultado tiene que sobrevivir en un lugar que no se libere, y ese lugar es un registro. **Así "vuelve" el resultado: no por la stack, sino por 📦.**

**Paso 7: doble libera su casillero**

- **Qué hace:** sube el sp de 95 a 96.
- **Por qué:** `doble` terminó y ya no necesita `y`. Además, así el sp queda justo sobre la nota de retorno, que es donde el paso siguiente la va a buscar.

```
 96 │ 📝 volver a main  ← sp
 95 │ 10             ← quedó ahí, pero ahora es zona libre (basura)
```

**Paso 8: ret. Doble vuelve a main**

- **Qué hace:** lee la nota donde apunta el sp (96), salta a main y sube el sp a 97.
- **Por qué:** la nota ya se usó y no hace falta guardarla más.

```
 97 │ r = ?          ← sp
 96 │ 📝 (basura)
 95 │ 10 (basura)
```

**Paso 9: main copia 📦 en r**

- **Qué hace:** lee el 10 del registro de resultado y lo escribe en `r` (el 97).
- **Por qué:** esta es la segunda mitad de `int r = doble(a);`. Primero se llamó a la función; ahora se guarda lo que devolvió. **Es main la que llena `r`, no doble.**

```
100 │ (cosas de antes)
 99 │ a = 5
 98 │ b = 7
 97 │ r = 10         ← sp
```