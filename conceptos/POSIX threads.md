---
requiere: []
habilita: []
creado: 2026-09-27
---
Cada SO tenía su propia forma de crear hilos: Windows tiene `CreateThread` (lo ves en `thrd-win32.c`) y cada Unix tenía la suya. Un programa con hilos escrito para un sistema no compilaba en otro.

**POSIX** es un **estándar**: una especificación de funciones que los sistemas tipo Unix (Linux, macOS, BSD) se comprometen a ofrecer. **Pthreads** es la parte del estándar que define hilos. Escribís con `pthread_*` y compila en cualquier sistema POSIX.

Ojo: pthreads define la **interfaz** (qué funciones hay y qué hacen), no **cómo** se implementan. En Linux cada `pthread_create` crea un kernel-level thread (one-to-one). Otra implementación podría hacerlo con user-level threads, y tu código no cambiaría.
### Funciones de pthreads

| Función                               | Qué hace                                                                                                                                                     | Argumento   | Qué es ese argumento                                                                                   |
| ------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----------- | ------------------------------------------------------------------------------------------------------ |
| `pthread_create(&t, attr, func, arg)` | Crea un hilo nuevo que arranca ejecutando `func(arg)`. Devuelve `0` si salió bien. Equivale a `fork()`, pero el hilo comparte la memoria en vez de copiarla. | `&t`        | Dirección de una variable `pthread_t` donde la función **guarda el ID** del hilo nuevo                 |
|                                       |                                                                                                                                                              | `attr`      | Atributos del hilo (por ejemplo, tamaño de stack). `NULL` = los de por defecto                         |
|                                       |                                                                                                                                                              | `func`      | La función que va a ejecutar el hilo: es su "`main`". Tiene que tener la forma `void* func(void* arg)` |
|                                       |                                                                                                                                                              | `arg`       | El dato que recibe `func` como parámetro. `NULL` si no le pasás nada                                   |
| `pthread_join(t, &ret)`               | **Bloquea** al que la llama hasta que el hilo `t` termine. Equivale a `wait()`.                                                                              | `t`         | El ID del hilo a esperar (el que guardó `pthread_create`)                                              |
|                                       |                                                                                                                                                              | `&ret`      | Dirección de una variable donde guardar **lo que devolvió** el hilo. `NULL` = no me interesa           |
| `pthread_exit(ret)`                   | Termina **solo el hilo** que la llama (hacer `return ret` desde `func` es lo mismo). Equivale a `exit()`, pero para un hilo.                                 | `ret`       | El valor que va a recibir el que haga `pthread_join`                                                   |
| `pthread_self()`                      | Devuelve el ID del hilo que la llama. Equivale a `getpid()`.                                                                                                 | _(ninguno)_ |                                                                                                        |

###### Qué es `void *`
Vamos por partes.
**1. `void` solo** significa "nada". Ya lo viste en `void f()`: la función **no devuelve nada**.
**2. `int *`** significa "pointer a int": una **dirección** donde hay un `int` (tu nota `Pointers`).
**3. `void *`** significa "pointer a _algo_": una **dirección**, pero sin decir qué tipo de dato hay ahí. Es una dirección "genérica".

¿Para qué sirve? La librería de pthreads no sabe qué le vas a querer pasar a tu hilo: un número, un texto, un struct… Así que dice "pasame una dirección a lo que sea y arreglate

**Cómo se lee la firma:**
```c
void *hilo(void *arg)
```
El `*` va pegado al **tipo**, no al nombre. Se lee así:

- `hilo` es una función,
- que recibe `arg`, de tipo `void *` (una dirección genérica),
- y devuelve un `void *` (otra dirección genérica).