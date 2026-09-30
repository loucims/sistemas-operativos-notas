---
requiere:
  - "[[Sleep and wakeup]]"
habilita:
  - "[[Semaphore]]"
creado: 2026-09-29
---
#### Qué problema es
Dos (o más) procesos comparten un **buffer de tamaño fijo N**:
- el **productor** genera items y los **pone** en el buffer;
- el **consumidor** los **saca** y los procesa.

Lo usás todo el tiempo sin nombrarlo así:

| Ejemplo              | Productor                     | Buffer               | Consumidor                |
| -------------------- | ----------------------------- | -------------------- | ------------------------- |
| `ls \| grep txt`     | `ls` escribe                  | el pipe              | `grep` lee                |
| Imprimir             | los programas mandan trabajos | la cola de impresión | la impresora              |
| Una cola de RabbitMQ | quien publica mensajes        | la cola              | el worker que los procesa |
#### Qué tiene que garantizar una solución

| #   | Condición                                                 | Tipo                 |
| --- | --------------------------------------------------------- | -------------------- |
| 1   | Nunca dos procesos tocando el buffer (y `count`) a la vez | **Mutual exclusion** |
| 2   | Si el buffer está **lleno**, el productor espera          | **Sincronización**   |
| 3   | Si el buffer está **vacío**, el consumidor espera         | **Sincronización**   |
Esto es lo que hace interesante al problema: necesita **las dos cosas** a la vez. Mutual exclusion sola no alcanza (lo viste con el `x * 2` / `x + 3`: no ordena), y hace falta que uno **espere a que el otro haga algo**.

El buffer suele ser circular (ver `=[[Circular buffer]]`): dos índices, `in` (dónde pongo) y `out` (de dónde saco), que dan la vuelta al llegar a N.

```
 buffer (N = 5):  [ A ][ B ][ C ][   ][   ]
                    ↑              ↑
                   out             in        count = 3
```

#### La solución "ingenua" con sleep/wakeup (slide U2_4, "solución parcial")
```c
#define N 100
int count = 0;                      // compartida: cuántos items hay

// ──────────────── Productor ────────────────
while (TRUE) {
    item = produce_item();
    if (count == N)                 // lleno → me duermo
        sleep();
    insert_item(item);
    count = count + 1;
    if (count == 1)                 // estaba vacío → despierto al 
        wakeup(consumer);           // consumidor
}

// ──────────────── Consumidor ───────────────
while (TRUE) {
    if (count == 0)                 // vacío → me duermo
        sleep();
    item = remove_item();
    count = count - 1;
    if (count == N - 1)             // estaba lleno → despierto al 
        wakeup(producer);           // productor
    consume_item(item);
}
```

La lógica tiene sentido: cada uno duerme cuando no puede avanzar, y despierta al otro justo cuando le vuelve a dar trabajo. **Pero no funciona**, por dos motivos:

1. **`count = count + 1` y `count = count - 1` no son atómicos**: es el `x++` de siempre, sin ninguna mutual exclusion. Viola la condición 1.
2. **El wakeup perdido** que viste en Sleep and wakeup: el consumidor mira `count == 0`, lo interrumpen antes del `sleep()`, el productor manda un `wakeup` que nadie recibe, y el consumidor se duerme igual. Slide: _"El consumidor queda bloqueado para siempre."_

#### Con qué se confunde
- **No es "un problema de mutual exclusion"**: es mutual exclusion **más** sincronización. Por eso las herramientas que lo resuelven tienen que dar las dos cosas.
- **Buffer acotado ≠ cola infinita**: si el buffer fuera infinito, el productor nunca tendría que esperar y desaparecería la condición 2. Lo difícil es justamente que tiene tamaño N.