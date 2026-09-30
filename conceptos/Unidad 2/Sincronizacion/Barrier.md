⚠️ **No confundir** con las **barreras de memoria** (_memory barriers / fences_) de la slide U2_3, las de Peterson y `incrementar.cpp`. Mismo nombre, cosas distintas:

|            | Memory barrier                               | Barrier (este concepto)                    |
| ---------- | -------------------------------------------- | ------------------------------------------ |
| Qué es     | Una instrucción de la CPU                    | Un mecanismo de sincronización entre hilos |
| Qué ordena | Las lecturas y escrituras de **un** hilo     | El avance de **N** hilos                   |
| Idea       | "No adelantes escrituras de acá para arriba" | "Nadie sigue hasta que lleguemos todos"    |
#### Qué problema resuelve
Muchos programas paralelos trabajan **por fases**, y cada fase necesita que **todos** hayan terminado la anterior.

Ejemplo: una simulación del clima, con el mapa dividido en 4 pedazos, uno por hilo. En cada paso de tiempo, cada hilo calcula su pedazo, pero para eso necesita los bordes de sus vecinos **del paso anterior**. Si un hilo rápido arranca el paso 2 mientras otro sigue en el paso 1, usa datos a medio calcular.

```
 Hilo A:  [paso 1]─────►│ espera │[paso 2]──►
 Hilo B:  [paso 1]──────────────►│[paso 2]──►
 Hilo C:  [paso 1]───►│  espera  │[paso 2]──►
                               BARRERA
                  (se abre cuando llega el último)
```

Una **barrera** es un punto del código donde cada hilo que llega **se duerme**, hasta que llegan **los N**. Cuando llega el último, se despiertan todos y siguen juntos.

#### Cómo funciona
En pthreads ya viene hecho:
```c
pthread_barrier_t barrera;
pthread_barrier_init(&barrera, NULL, 4);      // 4 hilos

// ────────── cada hilo ──────────
for (int paso = 0; paso < PASOS; paso++) {
    calcular_mi_pedazo(paso);
    pthread_barrier_wait(&barrera);           // espero a los otros 3
}
```

Y se puede armar con lo que ya sabés, un mutex y un semáforo:

```c
int count = 0;                 // cuántos llegaron
semaphore mutex   = 1;         // protege count
semaphore barrera = 0;         // acá duermen los que esperan

void llegar() {
    down(&mutex);
    count = count + 1;
    if (count == N) {                   // soy el ÚLTIMO en llegar
        for (int i = 0; i < N - 1; i++)
            up(&barrera);               // despierto a los N-1 que esperan
        count = 0;                      // la dejo lista para la próxima                                             fase
        up(&mutex);
    } else {                            // no soy el último
        up(&mutex);                     // suelto la llave ANTES de                                                  dormirme
        down(&barrera);                 // y espero
    }
}
```

Fijate en dos cosas que ya viste:
- `barrera` arranca en **0**: es el semáforo de "esperar un evento" (la tercera fila de tu tabla en `Semaphore`).
- El que no es último **suelta `mutex` antes de dormirse**. Si durmiera con la llave, el último no podría entrar a sumar `count`, y todos quedarían trabados. Es la misma regla que el deadlock del producer-consumer con los `down` al revés.

#### Con qué se confunde
- **Barrier ≠ memory barrier** (la tabla de arriba).
- **Barrier ≠ `pthread_join`**: `join` espera a que un hilo **termine**. La barrera espera a que todos **lleguen a un punto**, y después **siguen** vivos. Te pregunto por qué importa en la pregunta 2.
- **Barrier ≠ mutual exclusion**: acá no se protege un dato. Es pura **sincronización**: coordinar _cuándo_ avanza cada uno.