---
requiere: []
habilita: []
creado: 2026-09-26
---
Un proceso ocupa cosas: memoria, archivos abiertos y un slot en la process table. Cuando termina, el kernel tiene que *recuperar todo eso*. 
Si no se acumularia basura hasta que no entre ningun proceso mas.

Tambien, *al padre le importa como termino el hijo*. Por lo que es necesario devolver un *exit status.*

Para esto estan los comandos de 
- **`exit(status)`**: termina el proceso que la llama. El kernel libera su memoria y sus archivos, y guarda `status` en el PCB para que lo lea el padre. El proceso queda ZOMBIE. Nunca retorna.
	-**Aclaracion sobre zombies:** hay un "garbage collector", y es **`init`**:
	- Si el padre **muere** sin hacer `wait`, `init` adopta al zombie y hace `wait` por él.
	- Si el padre **sigue vivo** y nunca hace `wait`, el zombie queda ahí hasta que el padre muera. No hay nada automático antes (salvo que el padre le avise al kernel que ignora a sus hijos, y ahí el kernel los libera solo).
- **`wait(&status)`**: el padre se **bloquea** hasta que termine algún hijo. Recibe el exit status de ese hijo y, al leerlo, el kernel libera el PCB del hijo. Devuelve el PID del hijo que terminó.
- **`kill(pid, señal)`**: le manda una **señal** a otro proceso (solo del mismo usuario). Si el proceso tiene un handler para esa señal, se ejecuta el handler. Si no, se hace la acción por defecto, que casi siempre es terminarlo.

| Forma                | ¿Voluntaria? | Quién lo decide                                | Ejemplo                                                         |
| -------------------- | ------------ | ---------------------------------------------- | --------------------------------------------------------------- |
| **Salida normal**    | Sí           | El programa: terminó su trabajo                | `return 0` de `main`, o `exit(0)`                               |
| **Salida por error** | Sí           | El programa: detecta un problema y decide irse | `gcc noexiste.c` → "no such file" → `exit(1)`                   |
| **Error fatal**      | No           | El **kernel**, por una `Exception`             | Dividir por cero, acceder a memoria que no es tuya (_segfault_) |
| **Matado por otro**  | No           | **Otro proceso**, con `kill()`                 | Matar un programa colgado desde el administrador de tareas      |

Dos cosas que conviene tener claras:
- `exit(n)` recibe un número, el **exit status**. Por convención, `0` es "salió bien" y otro valor es "hubo error". Cuando `main` hace `return`, se llama a `exit` automáticamente.
- `kill()` en realidad **envía una señal**. Si el proceso tiene un _signal handler_ para esa señal, se ejecuta ese handler (como si fuera una interrupción, pero de software). Si no tiene, la acción por defecto casi siempre es terminar. Solo se le pueden mandar señales a procesos **del mismo usuario**.

#### Qué hace el kernel cuando un proceso termina
En xv6 son `exit()` y `wait()` en `proc.c`:

**En `exit()` (lo ejecuta el hijo):**
1. Cierra los archivos abiertos y suelta el directorio (`cwd`).
2. Si tiene hijos, se los pasa a `init` (`reparent`), para que no queden huérfanos.
3. Si el padre estaba bloqueado en `wait()`, lo despierta (blocked → ready).
4. Guarda el exit status en el PCB (`xstate`) y pone `state = ZOMBIE`. Ya no vuelve a correr nunca.

**En `wait()` (lo ejecuta el padre):**
5. Encuentra al hijo ZOMBIE y copia su `xstate` a la variable del padre (el `&status`).
6. Libera la memoria del hijo (page table y address space) y su PCB → `state = UNUSED`.

*(En Linux, la memoria ya se libera en el `exit`, y el zombie conserva solo la ficha. La idea es la misma: lo último en irse es el PCB con el exit status.)*

#### Qué pasa con los threads
- **`exit()` termina el proceso ENTERO**, lo llame el hilo que lo llame (y hacer `return` en `main` es lo mismo que `exit`). Todos los hilos mueren en el acto, aunque estén a mitad de algo, porque se libera el address space donde viven su código y su stack.
- **Los hilos no quedan zombie.** El que queda zombie es el **proceso**: un solo PCB, con un solo exit status para el padre.
- **`pthread_exit()` termina solo el hilo que la llama.** El proceso sigue vivo mientras quede algún hilo. Cuando termina el último, termina el proceso (como `exit(0)`).
- **Un hilo que terminó y al que nadie le hizo `pthread_join`** guarda su valor de retorno hasta que alguien lo recoja. Es el equivalente del zombie, pero a nivel hilo, y desaparece cuando termina el proceso.
- xv6 no tiene hilos: un proceso = un solo hilo.

| Procesos | Hilos |
|---|---|
| `exit(status)` → termina el proceso | `pthread_exit(ret)` → termina solo el hilo |
| `wait(&status)` → espera a un hijo y lee su exit status | `pthread_join(t, &ret)` → espera a un hilo y lee lo que devolvió |
| Hijo terminado sin `wait` → ZOMBIE | Hilo terminado sin `join` → guarda su valor hasta que lo recojan |


#### Por qué existe el ZOMBIE
El exit status tiene que quedar guardado **en algún lado** hasta que el padre lo lea. Pero en el paso 2 la memoria del hijo ya se liberó. El único lugar que queda es el **PCB**, que está en memoria del kernel. Por eso el PCB sobrevive un rato más: un proceso zombie **no usa CPU ni memoria**, solo ocupa su slot en la process table.

¿Y si el padre muere antes que el hijo? El hijo queda **huérfano** y el kernel lo "adopta" con `init` como padre (en xv6, `reparent()`). `init` se pasa la vida haciendo `wait()`, así que recoge a todos los huérfanos y no se acumulan zombies.


#### Con qué se confunde
- **Zombie ≠ proceso colgado o bloqueado**: el zombie **ya terminó**. No se puede matar porque ya está muerto: solo espera que el padre lo recoja.
- **Salida por error ≠ error fatal**: en la salida por error el **programa** detecta el problema y elige salir. En el error fatal el programa ni se entera: la CPU genera una exception y el **kernel** lo termina.
- **`kill()` no siempre mata**: manda una señal, y el proceso puede atraparla con un handler.