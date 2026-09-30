---
requiere:
  - "[[Sleep and wakeup]]"
  - "[[Producer-consumer problem]]"
habilita:
  - "[[Mutex]]"
  - "[[Monitor]]"
  - "[[Readers-writers problem]]"
creado: 2026-09-29
---
`Sleep and wakeup` tenía dos fallas:
1. `wakeup` **no tiene memoria**: si llega cuando nadie duerme, se pierde.
2. El "mirar `count` y dormirse" **no es atómico**.

Mi propuesta en la pregunta del bit ya era la solución: un **contador** que guarde los wakeups, con *el "mirar y dormir" hecho de forma atómica por el kernel.* Eso es un **semáforo** (Dijkstra, 1965).

#### Cómo funciona (slide U2_4, "Semáforos")
Un semáforo `S` es un **entero no negativo**, más una **lista de procesos dormidos**, con dos operaciones **atómicas**:

| Operación     | Qué hace                                                                                  |
| ------------- | ----------------------------------------------------------------------------------------- |
| **`down(S)`** | Si `S > 0`: le resta 1 y sigue. Si `S == 0`: el proceso **se duerme** (va a la lista)     |
| **`up(S)`**   | Si hay alguien dormido en la lista: **despierta a uno**. Si no hay nadie: le suma 1 a `S` |
`S` es un **número de permisos disponibles**: cuántos `down` más pueden pasar **sin dormirse**.

> Un semaforo `S` se puede pensar como una pila o counter de recursos, una *data structure* mas, la cual 
>
> - con **down()** de manera atomica chequeas la pila o contador si es igual a 0, *te dormis hasta que vuelva a tener recursos* (te sumas a una lista de procesos dormidos) o si tiene recursos adentro *le resta uno y sigue*.
> 
> - Y con **up()** de manera atomica *le agregas 1 al counter o pila*. (Y si hay alguien dormido, *lo despierta automaticamente* **sin sumar 1**)  

#### Cuándo usar un semáforo (pensándolo como data structure)
Sirve cuando hay **algo que se puede contar** y hilos que tienen que **esperar** a que haya. Para diseñarlo, tres preguntas:
1. **¿Qué cuento?** → qué es un recurso.
2. **¿Cuántas hay al principio?** → el valor inicial.
3. **¿Quién toma (`down`) y quién devuelve (`up`)?**

| Situación | Valor inicial | Ejemplo |
|---|---|---|
| **1 recurso exclusivo**: que entre uno solo | 1 | Proteger una `=[[Region critica]]` (mutex) |
| **N recursos iguales**: que usen como mucho N a la vez | N | Un pool de 10 conexiones a una base de datos; casilleros libres de un buffer |
| **Esperar un evento** que produce otro hilo | 0 | "Avisame cuando termines": B hace `down` y duerme, A hace `up` al terminar. Items listos en un buffer |

**Cuándo NO conviene:**
- Solo querés actualizar un número compartido (`x++`) → una operación atómica (`=[[Compare-and-swap]]`, fetch-and-add) es más simple y barata.
- La espera es cortísima y estás en el kernel → un spinlock (`=[[Test-and-set]]`) cuesta menos que dormir y despertar.
- La condición para esperar es compleja (no es solo "¿hay fichas?") → `=[[Monitor]]` con variables de condición.

###### **Analogía 1: un estacionamiento con N lugares y un contador en la entrada.**

- **`down`** = querés entrar. Si el contador marca lugares libres, restás 1 y entrás. Si marca 0, esperás en la fila (dormido, sin dar vueltas a la manzana: no hay busy waiting).
- **`up`** = salís. Si hay alguien en la fila, le das tu lugar directamente. Si no hay nadie, sumás 1 al contador: el lugar **queda guardado** para el próximo.

Ese "queda guardado" es lo que arregla el wakeup perdido: un `up` sin nadie esperando **no se pierde**, suma 1, y el próximo `down` lo encuentra y no se duerme.

#### Implementación (slide "Implementación de los semáforos")
```c
typedef struct {
    int value;
    lista_de_procesos wait_list;     // procesos dormidos en este semáforo
} semaphore;

// ──────────────── down ────────────────
void down(semaphore *S) {
    if (S->value > 0)
        S->value--;                  // había lugar → lo tomo y sigo
    else {
        agregar_a(S->wait_list, este_proceso);
        sleep();                     // no hay → me duermo (blocked)
    }
}

// ───────────────── up ─────────────────
void up(semaphore *S) {
    if (lista_vacia(S->wait_list))
        S->value++;                  // nadie esperando → guardo el                                                                 "permiso"
    else {
        P = sacar_uno_de(S->wait_list);
        wakeup(P);                   // le paso el permiso directo a P
    }
}
```

La slide avisa: _"up() y down() tienen que ser protegidos como secciones críticas."_ Si no, tendrían la misma race condition de siempre (mirar `value` y actuar).

**¿Cómo se hacen atómicos? (⭐)** Slide: _"Implementados en el kernel. Enmascarar las interrupciones cuando se opera sobre el semáforo. En multiprocesadores, usar también Test and Set. Ambas duran solo unos pocos nanosegundos."_
```c
void down(semaphore *S) {
    apagar_interrupciones();         // nadie me interrumpe en esta CPU
    lock_TSL(&S->lock);              // ninguna otra CPU entra (si hay varias)
    // ... el if/else de arriba ...
    unlock_TSL(&S->lock);
    prender_interrupciones();
}
```

#### ¿A quién despierta `up` cuando hay varios esperando? (⭐)
A **uno solo**. **Cuál** no lo define el semáforo: la slide dice _"select and remove a process P"_, sin especificar. Depende de la implementación. Lo más común es **FIFO** (el que llegó primero a la fila), porque así nadie espera para siempre. Si fuera, por ejemplo, "el de mayor prioridad", uno de baja prioridad podría no despertarse nunca (`Starvation`).

#### Los dos usos (slide "Dos usos diferentes de los semáforos")

| Tipo                             | Valor inicial | Para qué                                                                                |
| -------------------------------- | ------------- | --------------------------------------------------------------------------------------- |
| **Binario** (mutex)              | 1             | `Mutual exclusion`: `down` al entrar a la región crítica, `up` al salir                 |
| **Contador**                     | N > 1         | Permitir hasta N a la vez (N lugares, N conexiones)                                     |
| **Contador para sincronización** | 0             | "Esperá hasta que pase X": uno hace `down` y se duerme, el otro hace `up` cuando X pasó |

#### Producer-consumer, resuelto (slide "Problema del productor/consumidor, revisado")
Tres semáforos:

| Semáforo | Inicial | Qué cuenta                                                    |
| -------- | ------- | ------------------------------------------------------------- |
| `empty`  | N       | Casilleros **libres**: el productor espera si es 0 (lleno)    |
| `full`   | 0       | Casilleros **ocupados**: el consumidor espera si es 0 (vacío) |
| `mutex`  | 1       | Acceso exclusivo al buffer                                    |

#### Con qué se confunde

- **`up` ≠ `wakeup`**: si nadie espera, `wakeup` se pierde y `up` **se guarda** (suma 1).
- **`down` puede bloquear, `up` nunca bloquea.**
- **Semáforo ≠ `Mutex`**: el mutex es un semáforo que solo vale 0 o 1. Es el próximo concepto.
- **El orden importa, y mucho**: la slide pregunta _"¿qué sucede si cambiamos el orden de los down?"_ y contesta _"un solo error y todo el sistema se detiene"_. Te lo dejo como pregunta.