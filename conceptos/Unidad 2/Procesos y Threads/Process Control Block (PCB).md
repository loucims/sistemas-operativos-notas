---
requiere:
  - "[[Process]]"
habilita:
  - "[[Cambio de contexto]]"
  - "[[Process states]]"
creado: 2026-09-25
---
Cuando el kernel le saca la CPU a un proceso tiene que guardar sus registros en algun lado, o el proceso no puede volver. Ademas, el kernel necesita llevar la cuenta de todo lo que es de cada proceso: su *PID* (Process ID), su *estado*, sus *archivos abiertos*, y *donde esta su memoria*.

El **PCB o Process Control Block** es esa ficha, *un struct por proceso, guardado en la memoria del kernel* con todo lo que el kernel necesita saber sobre ese proceso.

El kernel tiene una *process table* que es basicamente un array de PCBs.

Qué guarda, según la slide, y cómo se ve en el `struct proc` de xv6 (el que dieron en clase):

| Categoría (slide) | Campo en xv6               | Para qué                                                  |
| ----------------- | -------------------------- | --------------------------------------------------------- |
| Estado            | `state`                    | Running, ready, blocked…                                  |
| PC y registros    | `trapframe`, `context`     | Guardar la CPU del proceso para poder retomarlo           |
| Mapa de memoria   | `pagetable`, `sz`          | Dónde está su address space y cuánto mide                 |
| Identidad         | `pid`, `parent`, `name`    | Quién es y quién lo creó                                  |
| Estado de I/O     | `ofile[]`, `cwd`           | Archivos abiertos y directorio actual                     |
| Info contable     | `ctx_switches`             | Estadísticas (en xv6 es un agregado de la cátedra)        |
| Kernel stack      | `kstack`                   | La dirección de su `Kernel stack`                         |
| Espera / fin      | `chan`, `killed`, `xstate` | Qué evento espera si duerme; el exit status para el padre |
|                   |                            |                                                           |

#### `sz`: hasta dónde llega la memoria del proceso
Todo lo que está entre la dirección `0` y `sz` es memoria válida del proceso; de ahí para arriba no existe. En xv6 el `=[[Heap]]` está arriba de todo, así que agrandar la memoria es solo mover ese tope:
- `malloc` se queda sin lugar → llama a la syscall `sbrk(n)` → el kernel agrega páginas a la page table arriba de `sz` y hace `sz += n`.
- `=[[Fork]]` copia al hijo los bytes de `0` a `sz`.
- Acceder más allá de `sz` → `=[[Exception]]` → el proceso muere.

Así se usa cuando A deja la CPU y entra B:
1. Llega una interrupción (por ejemplo, el `Timer`). La CPU pasa a modo kernel.
2. El kernel **copia los registros de la CPU al PCB de A** y pone `A.state = READY`.
3. Elige a B.
4. **Copia los registros del PCB de B a la CPU** y pone `B.state = RUNNING`.
5. Vuelve a modo usuario, y B sigue exactamente donde había quedado.

Ese ida y vuelta es el `=[[Cambio de contexto]]`, y el PCB es de donde sale y a donde va todo.

Con que se confunde usualmente.
- **PCB ≠ process**: el proceso es todo (código, memoria, ejecución). El PCB es solo la ficha que el kernel tiene sobre él.
- **PCB ≠ address space**: el PCB no contiene la memoria del proceso, solo un **puntero** a ella (`pagetable`). La memoria sigue estando en su lugar.
- **PCB ≠ `Kernel stack`**: son dos cosas separadas. El PCB solo guarda la dirección (`kstack`).