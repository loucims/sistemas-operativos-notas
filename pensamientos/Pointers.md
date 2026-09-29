---
creado: 2026-09-26
tags: [procesos]
---
**Pregunta:** ¿qué es el `&` en `wait(&status)`? ¿Cómo funcionan los pointers?

**Respuesta corta:** `&status` es la **dirección** de la variable `status` local del padre. Se la pasás a `wait` para que `wait` pueda escribir el exit status del hijo **adentro de tu variable**.

## 1. Toda variable tiene dirección
Una variable es un casillero de la `=[[RAM]]`. Tiene un **valor** (lo que hay adentro) y una **dirección** (dónde está).

```c
int status = 0;
```

```
 dirección   contenido
 0x7000      status = 0
```

- `status` → el valor: `0`
- `&status` → la dirección: `0x7000` ("dirección de")

## 2. Un pointer es una variable que guarda una dirección
```c
int *p = &status;   // p guarda 0x7000
*p = 5;             // "andá a la dirección que tiene p y escribí 5"
```

```
 0x7000      status = 5    ← cambió sin nombrar a status
 0x6FF8      p = 0x7000
```

| Símbolo  | Dónde             | Significa                                                 |
| -------- | ----------------- | --------------------------------------------------------- |
| `int *p` | En la declaración | "p es un pointer a int" (guarda una dirección)            |
| `&x`     | En una expresión  | "la dirección de x"                                       |
| `*p`     | En una expresión  | "lo que hay en la dirección que guarda p" (*dereference*) |

## 3. Por qué hace falta: C copia los argumentos
Cuando llamás a una función, recibe una **copia** del valor (igual que el `📨` registro de argumento de la nota `=[[Stack]]`). Si la función cambia la copia, tu variable no se entera:

```c
void poner5(int x)  { x = 5; }    // cambia la copia
void poner5p(int *x) { *x = 5; }  // escribe en la dirección original

int a = 0;
poner5(a);    // a sigue en 0
poner5p(&a);  // a pasa a 5
```

Con la dirección, la función puede escribir en el casillero original.

## 4. El caso de `wait`
Una función en C devuelve **un solo valor**. `wait` necesita devolver dos cosas:
1. **Qué hijo** terminó → va por el valor de retorno (el PID).
2. **Cómo** terminó → el exit status, que va por el pointer.

```c
int status;
pid_t hijo = wait(&status);
// hijo   = PID del hijo que terminó
// status = su exit status (lo escribió el kernel)
```

Paso a paso:
1. El padre le pasa `&status` (ej. `0x7000`) al kernel.
2. El padre se bloquea hasta que termine un hijo.
3. El hijo hace `exit(1)`: el kernel guarda `1` en su PCB (`xstate`).
4. El kernel copia ese valor a la dirección `0x7000` **del padre**, o sea, adentro de `status`.
5. `wait` retorna el PID del hijo.

Detalle: en Linux, `status` no queda en `1` literal: trae el código y otra info mezclada. Se lee con `WEXITSTATUS(status)`. En xv6 sí queda el número tal cual.

## 5. `wait(NULL)`
`NULL` es "dirección vacía" (0). Le dice a `wait`: "no me interesa cómo terminó, no escribas nada". Es lo que usa `fig3.8.c`.

## Relación con la materia
- El kernel escribe en la memoria del padre, así que tiene que chequear que la dirección sea válida y del padre (en xv6, `copyout` usando la page table del proceso). Si no, un proceso podría pasarle una dirección del kernel y hacerlo escribir donde no debe.
- `fork` y `exec` usan pointers igual: `execv(path, argv)` recibe la dirección de un array de strings.
