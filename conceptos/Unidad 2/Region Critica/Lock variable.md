---
requiere:
  - "[[Busy waiting]]"
habilita:
  - "[[Test-and-set]]"
creado: 2026-09-28
---
`=[[Disabling interrupts]]` no sirve en modo usuario ni con varias CPUs. Entonces surge la idea más natural de todas, **por software**: *una variable compartida que diga si la región crítica está ocupada.*

- `lock = 0` → libre
- `lock = 1` → ocupada

Como el cartel de "ocupado" de un baño: antes de entrar mirás el cartel, si dice libre entrás y lo das vuelta, y al salir lo volvés a poner en libre.

#### Cómo funciona (slide U2_3, "Variables de locks I")
```c
int lock = 0;              // compartida

while (lock != 0)          // 1. mirar el cartel: si está ocupado, girar
    ;
lock = 1;                  // 2. dar vuelta el cartel: "ocupado"
/* región crítica */
lock = 0;                  // 3. al salir: "libre"
```
Parece que funciona. **No funciona.**
#### Por qué falla (slide "Variables de locks II")
_"Hay una ventana entre la comprobación de cero y su puesta a 1."_ Mirar el cartel (paso 1) y darlo vuelta (paso 2) son **dos pasos separados**, y el cambio de hilo puede caer justo en el medio:

| #   | Hilo A                                                  | Hilo B                                       | `lock` |
| --- | ------------------------------------------------------- | -------------------------------------------- | ------ |
| 1   | mira: `lock == 0` → ¡libre! sale del `while`            |                                              | 0      |
| —   | _⏰ timer: cambio a B_                                   |                                              |        |
| 2   |                                                         | mira: `lock == 0` → ¡libre! sale del `while` | 0      |
| 3   |                                                         | `lock = 1`, **entra** a la región crítica    | 1      |
| —   | _⏰ vuelve A_                                            |                                              |        |
| 4   | `lock = 1` (ya había visto que estaba libre), **entra** |                                              | 1      |
| 5   | **en la región crítica**                                | **en la región crítica**                     | 1      |

Los dos adentro: **viola mutual exclusion**, el requisito 1.

Es **exactamente el mismo problema que el `x++`**: leer un valor, decidir, y escribir. Si alguien se mete entre la lectura y la escritura, trabajás con un valor viejo. Intentamos proteger una variable compartida (`x`) usando **otra variable compartida** (`lock`), que tiene su propia race condition.

#### El intento de la slide: "hacerlo en una sola línea"
```c
while (lock++ != 0)   // leer y sumar 1 en la misma línea
    lock--;
```
Tampoco funciona. La slide muestra el assembler: `lock++` sigue siendo **leer, sumar y guardar** en pasos separados (_"Para una variable en memoria, ¡el incremento no es atómico!"_). Que algo esté en una sola línea de C no lo hace indivisible.

#### La lección
Para que un lock funcione, *"mirar si está libre" y "marcarlo como ocupado" tienen que ser un solo paso que nadie pueda cortar.* Por software puro, con operaciones normales, no se puede. Hacen falta dos caminos, que son los próximos conceptos:

- **Evitar la variable única**: `Strict alternation` y `Peterson's solution` usan otras ideas (turnos, intenciones) para no depender de un "mirar y marcar".
- **Pedirle ayuda al hardware**: `Test-and-set`, una instrucción que hace "leer y poner en 1" en un solo paso atómico.

#### Con qué se confunde
- **"Anda casi siempre, entonces está bien"**: es una race condition clásica. Anda "a veces", que es la respuesta del ⭐ _siempre / a veces / nunca_.
- **Lock variable ≠ spinlock**: un spinlock **correcto** es una lock variable **más** una instrucción atómica (`Test-and-set`). La idea es la misma; lo que cambia es que el "mirar y marcar" sea indivisible.