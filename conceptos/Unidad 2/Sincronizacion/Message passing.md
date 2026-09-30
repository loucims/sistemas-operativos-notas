---
requiere:
  - "[[Producer-consumer problem]]"
habilita: []
creado: 2026-09-29
---
#### Qué problema resuelve
Todo lo que vimos en `=[[Region critica]]` y sincronizacion (TSL, semáforos, monitores) supone que los procesos **comparten memoria**: una variable `lock`, un contador, un buffer que todos pueden tocar. Pero:

- Dos procesos en **máquinas distintas** no comparten memoria.
- Aun en la misma máquina, dos **procesos** (no hilos) tienen address spaces separados, así que tampoco la comparten por defecto.

La alternativa es comunicarse **mandándose mensajes**. El SO (o l*a red*) se encarga de llevarlos, y la sincronización sale del hecho de que **recibir bloquea** hasta que llegue algo.

#### Cómo funciona (slide U2_4, "Paso de mensajes")
Dos operaciones:

| Operación                       | Qué hace          | ¿Cuándo bloquea?                             |
| ------------------------------- | ----------------- | -------------------------------------------- |
| **`send(destino, &mensaje)`**   | Manda un mensaje  | Si los buffers del receptor están **llenos** |
| **`receive(origen, &mensaje)`** | Recibe un mensaje | Si **no hay** ningún mensaje disponible      |
Slide: _"El receptor se bloquea si no hay datos disponibles. El emisor se bloquea si los búferes del receptor están llenos."_ Esas dos reglas **ya son** las condiciones 2 y 3 del producer-consumer.

Formas de usarlo (slide "Uso del paso de mensajes"):

|Paradigma|Cómo es|
|---|---|
|**Rendezvous**|No hay buffer. `send` bloquea hasta que el otro hace `receive`: se "encuentran"|
|**Buzones** (_mailboxes_)|Un buffer de tamaño fijo en el medio. El emisor bloquea si está lleno|
|**Solicitudes explícitas**|El consumidor le manda mensajes **vacíos** al productor, y cada vacío es un "permiso" para producir. Es el ejemplo de la slide|
#### Producer-consumer con mensajes (slide)
```c
#define N 100

// ──────────────── Consumidor ────────────────
void consumer() {
    message m;
    for (int i = 0; i < N; i++)
        send(producer, &m);            // 1. al arrancar: mando N mensajes VACÍOS
    while (TRUE) {
        receive(producer, &m);         // 2. espero un mensaje CON item
        item = extract_item(&m);
        send(producer, &m);            // 3. devuelvo el mensaje, ahora                                             vacío
        consume_item(item);
    }
}

// ──────────────── Productor ─────────────────
void producer() {
    message m;
    while (TRUE) {
        item = produce_item();
        receive(consumer, &m);         // 4. espero un mensaje VACÍO
        build_message(&m, item);       // 5. lo lleno con el item
        send(consumer, &m);            // 6. se lo mando
    }
}
```
La idea: **hay siempre exactamente N mensajes dando vueltas**, que son los "casilleros" del buffer.

- Un mensaje **vacío** es un casillero libre: el productor necesita uno para producir.
- Un mensaje **lleno** es un item listo: el consumidor necesita uno para consumir.

```
        ┌──── mensajes vacíos (casilleros libres) ────┐
        │                                              ▼
   Consumidor                                      Productor
        ▲                                              │
        └──── mensajes llenos (items listos) ──────────┘
```

Si el productor va más rápido, se queda sin vacíos y bloquea en su `receive`. Si el consumidor va más rápido, se queda sin llenos y bloquea en el suyo.

#### Con qué se confunde
- **No es solo para redes**: un **pipe** (`ls | grep`) es message passing en la misma máquina, y una cola de RabbitMQ también.
- **Mensaje ≠ memoria compartida**: el mensaje se **copia** de un proceso al otro. Nadie toca los datos del otro, así que no hay race conditions sobre ellos. El precio es que copiar es más lento que leer una variable compartida.
- La slide deja afuera lo difícil de las redes: _"confirmación de recepción, control de flujo, detección y corrección de errores, ordenación de mensajes y autenticación. Temas de otra materia."_