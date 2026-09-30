---
creado: 2026-09-29
tags: [concurrencia, procesos]
---
**Pregunta:** sabiendo `=[[Message passing]]` y `=[[Producer-consumer problem]]`, ¿cómo funciona RabbitMQ por detrás?

**Respuesta corta:** RabbitMQ es un **producer-consumer con el buffer puesto en un tercer proceso** (el *broker*). Productores y consumidores no comparten memoria con nadie: le mandan mensajes al broker por la red, y el broker guarda la cola, decide a quién entregar cada mensaje y frena a quien va demasiado rápido. Todo lo de la materia aparece, pero repartido entre máquinas.

## 1. Las piezas
```
 Productor ──TCP──►  BROKER (RabbitMQ)  ──TCP──► Consumidor 1
 (tu API)            exchange → cola              (worker)
                                        ──TCP──► Consumidor 2
```
- **Productor** (*publisher*): el proceso que publica mensajes. Por ejemplo, tu API.
- **Broker**: el servidor de RabbitMQ, un proceso aparte (a menudo en otra máquina).
  - **Exchange**: recibe cada mensaje y decide a qué cola(s) va, según reglas de ruteo.
  - **Cola** (*queue*): la fila FIFO donde esperan los mensajes. **Es el buffer** del producer-consumer.
- **Consumidor**: el proceso que saca mensajes y los procesa. Por ejemplo, un worker.

La comunicación es por **sockets TCP** con un protocolo llamado AMQP. Es message passing de verdad: cada mensaje se **copia** por la red, y nadie toca la memoria del otro.

## 2. El mapa: concepto de la materia → RabbitMQ
| Concepto | En RabbitMQ |
|---|---|
| Buffer acotado | La **cola**. Tiene un límite (largo máximo configurable, o la memoria/disco del broker) |
| `send` bloquea si el buffer está lleno | **Flow control**: si el broker se queda sin memoria o la cola no da abasto, **frena a los publishers** (deja de leer de sus sockets, y su `send` queda esperando) |
| `receive` bloquea si no hay mensajes | El consumidor espera en su socket. Por debajo es un `read()` sobre la red que lo deja **blocked** (`=[[Process states]]`); cuando llega un paquete, la interrupción de la placa de red lo despierta |
| `=[[Semaphore]]` con N fichas | El **prefetch** (`basic.qos`): cuántos mensajes puede tener un consumidor "en mano" sin confirmar. Con prefetch = 10, el broker le manda hasta 10 y después espera a que devuelva confirmaciones (acks). Cada ack es un `up` |
| Cada item lo consume **uno solo** | Con varios consumidores en la misma cola, el broker reparte **round-robin** y cada mensaje va a uno solo |
| `=[[Mutual exclusion]]` sobre el buffer | La hace el broker por dentro: cada cola la maneja **un solo proceso interno**, así que los accesos a la cola quedan en fila de a uno |

## 3. Lo que agrega sobre el modelo de la materia: fallas
Con memoria compartida, si un hilo muere el proceso entero muere. Con procesos en red, **un consumidor puede morir y el resto seguir**. RabbitMQ lo maneja con **acks**:
1. El broker le entrega un mensaje al consumidor, y el mensaje queda como **"sin confirmar"** (no se borra todavía).
2. Si el consumidor lo procesa bien, manda un **ack** → recién ahí el broker lo borra.
3. Si el consumidor se muere antes de mandar el ack (se corta la conexión), el broker **vuelve a poner el mensaje en la cola** para otro consumidor.

Consecuencia: un mensaje puede procesarse **más de una vez** (si el consumidor murió justo después de procesarlo pero antes del ack). Por eso los consumidores se diseñan *idempotentes*: procesar el mismo mensaje dos veces tiene que dar el mismo resultado.

**Mensajes persistentes**: si el mensaje se marca como persistente y la cola como durable, el broker lo escribe a **disco** antes de confirmarle al publisher. Si el broker se reinicia, no se pierde.

## 4. Por dentro del broker: hilos y procesos
RabbitMQ está escrito en **Erlang**, y la máquina virtual de Erlang (BEAM) es un ejemplo directo de la unidad:
- Cada cola, cada conexión y cada canal es un **"proceso" de Erlang**: una unidad liviana manejada por la VM, no por el SO. Es como un `=[[User-level thread]]`: miles de ellos, muy baratos de crear.
- La VM corre **un hilo del SO por núcleo** (los *schedulers*) y reparte los procesos de Erlang sobre esos hilos: el modelo **many-to-many** de la slide de hilos. Así evita el problema de los user-level threads puros (usar un solo núcleo).
- A diferencia de los user-level threads cooperativos, la VM hace **preemption**: cuenta cuánto trabajo hizo cada proceso (*reductions*) y lo cambia por otro después de un rato, aunque no ceda el turno.
- Los procesos de Erlang **no comparten memoria**: se comunican **solo con mensajes**. Así que el broker usa message passing también por dentro, y por eso no necesita locks para la cola: un único proceso es el "dueño" de la cola y atiende los pedidos de a uno.

## Relación con la materia
- `=[[Producer-consumer problem]]`: la cola es el buffer acotado; el flow control y el `receive` bloqueante son las condiciones de "lleno" y "vacío".
- `=[[Message passing]]`: no hay memoria compartida entre productor, broker y consumidor.
- `=[[Semaphore]]`: el prefetch es un semáforo contador con N fichas por consumidor.
- `=[[Kernel-level thread]]` / `=[[User-level thread]]`: la BEAM es un ejemplo real del modelo many-to-many.
