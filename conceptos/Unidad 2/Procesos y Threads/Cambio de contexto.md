---
titulo: Cambio de contexto
tipo: concepto
unidad: 2 - Procesos
estado: en-progreso
requiere:
  - "[[Interrupt]]"
  - "[[Dual-mode operation]]"
  - "[[Process Control Block (PCB)]]"
habilita:
  - "[[Multiprogramming]]"
  - "[[Scheduler]]"
creado: 2026-09-21
tags:
  - procesos
  - cpu
---
Ya con el `=[[Process Control Block (PCB)]]` se guarda el estado de cada proceso, y los `=[[Process states]]` dicen quien puede correr. Ahora lo que falta es el mecanismo que *saca a un proceso de la CPU y pone a otro*, de forma que el 
primero pueda volver despues exactamente donde estaba.
Sin eso, no hay `=[[Multiprogramming]]`.

#### Cómo se inicia (⭐)
Para cambiar de proceso, el **kernel** tiene que tener el control, porque *un proceso de usuario no puede tocar PCBs ni page tables* (`Dual-mode operation`). Y el kernel solo recupera el control por un **trap** o una **interrupción**
Eso da dos familias:

|                      | Voluntario                                                        | Involuntario (preemption)                                                                                         |
| -------------------- | ----------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| Quien lo provoca     | El *propio proceso*: pide algo que no se puede seguir sin esperar | El *kernel*, que le saca la CPU aunque el proceso podia seguir                                                    |
| Como entra al kernel | Trap (una syscall)                                                | Interrupcion                                                                                                      |
| Ejemplos             | read() del disco, wait(), sleep(), exit()                         | El `=[[Timer]]` avisa que se termino el quantum; o una interrupcion de I/O despierta a un proceso mas prioritario |
| Transicion           | Running -> blocked                                                | Running -> ready                                                                                                  |
#### Cómo funciona, paso a paso
A está corriendo; el timer interrumpe y el scheduler elige a B:
```
 A (user) ──timer──► KERNEL ─────────────────────────────► B (user)
                     1. CPU pasa a modo kernel,
                        salta a la kernel stack de A
                     2. guarda registros de A en su PCB   A.state = READY
                     3. scheduler elige a B
                     4. carga registros de B desde su PCB B.state = RUNNING
                     5. cambia la page table a la de B
                     6. vuelve a modo usuario → B sigue donde había quedado
```
#### Por qué es caro
- **Costo directo**: cientos de instrucciones para guardar, elegir y cargar (slide U2_5). Durante ese rato la CPU no trabaja para ningún proceso.
- **Costo indirecto**, que suele ser el mayor: las cachés de la CPU quedan llenas de datos de A, así que B arranca "en frío", con fallos de cache hasta recargarlas.

Por eso el quantum tiene que ser **mucho mayor que el costo del cambio de contexto** (si no, la CPU se la pasa cambiando) y **mucho menor que el tiempo de respuesta deseado** (si no, la máquina se siente lenta).


#### Con qué se confunde
- **Cambio de contexto ≠ cambio de modo**: toda syscall implica un cambio de modo (user → kernel → user). Pero si la syscall no bloquea, por ejemplo `getpid()`, vuelve al **mismo** proceso y no hay cambio de contexto. Todo cambio de contexto incluye un cambio de modo; al revés, no.
- **No toda interrupción causa un cambio de contexto**: si el timer interrumpe antes de que se termine el quantum, o llega una interrupción de I/O que no despierta a nadie más prioritario, el kernel la atiende y **vuelve al mismo proceso**
- **Entre hilos del mismo proceso** es más barato, porque no hay que cambiar la page table. Lo vemos en `Thread`.

#### Qué hace exactamente la CPU/kernel en un cambio de contexto 
De A (saliente) a B (entrante): 
1. **Entrar al kernel**: llega un `=[[Interrupt]]` (timer, E/S) o un `=[[Trap]]` (syscall que bloquea). El hardware pasa a modo kernel, guarda el PC de A y salta al handler del kernel usando la `=[[Kernel stack]]`. 
2. **Guardar el estado de A** en su `=[[Process Control Block]]`: PC, SP, todos los registros de propósito general y los flags. 
3. **Actualizar el estado de A**: `ready` si lo sacó el timer, `blocked` si pidió E/S. 
4. **Correr el `=[[Scheduler]]`**: recorrer los procesos/hilos ready y elegir a B. 
5. **Cambiar la page table** a la de B (instrucción privilegiada). Esto invalida la TLB. *(Si A y B son hilos del mismo proceso, este paso se saltea.)* 
6. **Cargar el estado de B** desde su PCB: registros, SP y PC. Estado de B → `running`. 
7. **Volver a modo usuario**: B sigue en la instrucción exacta donde había quedado. 
#### Por qué es caro 
- **Costo directo**: los pasos 1 a 7 son cientos de instrucciones en las que la CPU no trabaja para ningún proceso. Es tiempo perdido. 
- **Costo indirecto** (el mayor): 
	- **Cache fría**: la cache de la CPU guarda los datos que A venía usando. B usa otros, así que al principio casi todo lo que pide tiene que ir a buscarlo a la `=[[RAM]]`, que es mucho más lenta. 
	- **TLB vacía**: la TLB es una cache de traducciones "dirección virtual → física" (para no consultar la page table en cada acceso). Al cambiar de page table, las traducciones de A no le sirven a B, y B arranca traduciendo todo de cero. Por eso el quantum tiene que ser **mucho mayor** que el costo del cambio (si no, la CPU se la pasa cambiando) y **mucho menor** que el tiempo de respuesta deseado (U2_5). 
#### Proceso vs hilo del mismo proceso 

| Paso                            | Entre procesos | Entre hilos del mismo proceso |
| ------------------------------- | -------------- | ----------------------------- |
| Guardar/cargar registros        | Si             | Si                            |
| Cambiar page table / vaciar TLB | Si             | **No**                        |
| Cache fria                      | Si             | **Poco** (comparten datos)    |

