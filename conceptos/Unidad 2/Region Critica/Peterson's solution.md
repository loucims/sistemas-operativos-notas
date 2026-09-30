---
requiere:
  - "[[Strict alternation]]"
habilita:
creado: 2026-09-29
---
Strict alternation cumple mutual exclusion, pero viola progreso: si le toca al otro y el otro no quiere entrar, *igual te quedás esperando.* Lo que le falta es saber si el *otro quiere entrar o no.*

Peterson (1981) suma esa información: cada proceso avisa *"me interesa entrar",* y *el turno se usa solo para desempatar cuando los dos quieren a la vez.* Funciona para **2 procesos**

Analogía: un puente angosto donde pasa un auto por vez.
En la entrada del puente hay dos cosas:
- **Un guiño por auto** (`interested[i]`): lo prendés cuando querés cruzar y lo apagás cuando terminaste.
- **Un solo cartel compartido: "cede el paso: ----- "** (`turn`). Al llegar, cada auto escribe **su propio nombre**, pisando lo que hubiera antes.

La regla: *esperás solo si el otro tiene el guiño prendido y en el cartel dice tu nombre.*

- **Si llegás solo**, el guiño del otro está apagado y cruzás directo, diga lo que diga el cartel. Esto arregla el problema de progreso. `interested[other] = false`
- **Si llegan los dos a la vez**, los dos escriben su nombre, pero el cartel tiene lugar para **uno solo**. Queda *el del **último** que escribió,* y ese es el que espera. El otro cruza.
- **Al salir del puente**, apagás tu guiño, y el que esperaba ve el guiño apagado y cruza.

#### Cómo funciona (slide U2_3, "Algoritmo de Peterson")
```c
int turn;                   // compartida: quién fue el ÚLTIMO en llegar
int interested[2];          // compartida: interested[i] = TRUE → i quiere entrar

void enter_region(int process) {       // process = 0 o 1
    int other = 1 - process;           // el otro (si soy 0 → 1, si soy 1 → 0)
    interested[process] = TRUE;        // 1. "quiero pasar"
    turn = process;                    // 2. "es mi turno"
    while (turn == process && interested[other] == TRUE)
        ;                              // 3. espero SOLO SI llegué último Y el otro quiere
}

void leave_region(int process) {
    interested[process] = FALSE;       // 4. "ya no quiero" (bajo la mano)
}
```

Se espera en el `while` solo si se cumplen **las dos** condiciones: **(a)** yo fui el último en escribir `turn`, **y (b)** el otro también quiere entrar.

#### Los 3 casos

**Caso 1: solo P0 quiere entrar**

|#|P0|`interested`|`turn`|
|---|---|---|---|
|1|`interested[0] = TRUE`|[T, F]||
|2|`turn = 0`|[T, F]|0|
|3|`while (turn == 0 && interested[1])` → `interested[1]` es F → **entra**|||

P0 entra directo, aunque "le toque" a él ceder. **Progreso ✅**: esto es justo lo que strict alternation no podía hacer.

**Caso 2: los dos llegan casi a la vez**

|#|P0|P1|`interested`|`turn`|
|---|---|---|---|---|
|1|`interested[0] = TRUE`||[T, F]||
|2||`interested[1] = TRUE`|[T, T]||
|3|`turn = 0`||[T, T]|0|
|4||`turn = 1`|[T, T]|**1**|
|5|`while (turn == 0 && …)` → `turn` es 1 → **entra**||||
|6||`while (turn == 1 && interested[0])` → las dos son T → **gira**|||
|7|`leave_region`: `interested[0] = FALSE`||[F, T]|1|
|8||`interested[0]` es F → **entra**|||

Entra el que **no** escribió `turn` último. `turn` solo puede tener un valor, así que siempre hay un solo "último".

**Caso 3: P0 está adentro y llega P1** → P1 pone su mano y `turn = 1`, y gira hasta que P0 sale y baja su mano (pasos 6 a 8 de arriba).

#### Por qué nunca pueden estar los dos adentro

Para que **los dos** pasen el `while`, cada uno necesita que su condición sea falsa. Pero los dos levantaron la mano (`interested` = [T, T]), así que:

- P0 necesita `turn != 0`, o sea `turn == 1`;
- P1 necesita `turn != 1`, o sea `turn == 0`.

`turn` no puede valer 0 y 1 al mismo tiempo. **Imposible.** Mutual exclusion ✅.

#### Evaluación

| Requisito                     | ¿Cumple?                                                                                                                      |
| ----------------------------- | ----------------------------------------------------------------------------------------------------------------------------- |
| `=[[Mutual exclusion]]`       | ✅ (el argumento de arriba)                                                                                                    |
| Progreso                      | ✅ (caso 1: si el otro no quiere, entrás)                                                                                      |
| Espera limitada               | ✅ (si P0 sale y quiere volver a entrar, se pone `turn = 0`: queda como "último" y le cede el paso a P1, que estaba esperando) |
| Sin suposiciones de velocidad | ✅                                                                                                                             |

### **Desventajas:**
- **Solo para 2 procesos.** Para N existen generalizaciones, pero son complicadas.
- **Busy waiting** (slide "Spinlock": es una de _"las tres soluciones correctas"_ y las tres giran).
- **En CPUs modernas no funciona tal cual**: para ir más rápido, el compilador y la CPU pueden **reordenar** lecturas y escrituras a memoria. Si el `turn = process` se "adelanta" al `interested[process] = TRUE`, el razonamiento de arriba se rompe. Hacen falta _barreras de memoria_. Por eso el comentario de `incrementar.cpp` dice _"proceso similar a Peterson pero seguro por hardware"_.