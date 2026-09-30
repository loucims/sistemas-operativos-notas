---
requiere:
  - "[[Safe state]]"
  - "[[Banker's algorithm]]"
  - "[[Resource allocation graph]]"
habilita: []
creado: 2026-09-30
---
#### Qué problema resuelve
Es la estrategia 4 de la p.35 del U3_1: _"Evitarlo mediante una asignación cuidadosa de recursos"_. Prevention pone **reglas fijas** que molestan siempre, y detection deja que el deadlock **pase** y después hay que recuperarse. Avoidance es el punto medio: **no prohíbe nada de antemano**, pero **decide cada pedido** mirando si concederlo puede llevar a un deadlock.

> p.29: _"Evitar ⇒ asegúrese de que un sistema nunca entrará en un estado inseguro."_

### Que condiciones de deadlock evita? Ninguna.
Si revisás las 4 condiciones con avoidance:

|Condición|¿Sigue siendo posible con avoidance?|
|---|---|
|Mutual exclusion|✅ Sí: los resources siguen siendo de uno a la vez|
|Hold and wait|✅ Sí: los procesos tienen resources y esperan más|
|No preemption|✅ Sí: nadie les saca nada|
|Circular wait|⚠️ **Sería posible**, pero el SO **nunca deja que se forme**|

**La respuesta es: ninguna, por regla.** Avoidance deja las 4 condiciones posibles y, pedido por pedido, **esquiva los estados donde la espera circular podría llegar a formarse**. La slide p.26 lo dice así: el algoritmo _"examina dinámicamente el estado… para garantizar que nunca pueda haber una condición de espera circular"_.

#### Qué necesita (p.26)
- **Información a priori**: cada proceso **declara su máximo** de antemano.
- Un algoritmo que **examina dinámicamente** el estado en cada pedido, para garantizar que **nunca** se forme una espera circular.
#### Con qué herramienta (p.32)

| Caso                               | Herramienta                      | Regla                                                                                                                                       |
| ---------------------------------- | -------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------- |
| **Una instancia** de cada resource | `=[[Resource allocation graph]]` | Antes de conceder, ver si **se crearía un ciclo**. Si sí, **retrasar** el pedido y correr otro proceso hasta que se liberen resources (p.2) |
| **Varias instancias**              | `=[[Banker's algorithm]]`        | Conceder solo si el estado resultante es **seguro**                                                                                         |
#### Trayectorias de recursos (p.24–25)
Es una forma *gráfica* de ver lo mismo. Mirá la slide p.24 mientras leés:

- **Eje X** = cuánto avanzó el proceso *A* (instrucciones I1…I4). **Eje Y** = cuánto avanzó *B* (I5…I8).
- Cada punto del plano es un **estado** del sistema. Como hay una sola CPU, el camino avanza *horizontal* (corre A) o *vertical* (corre B).
- A usa la printer entre I1 e I3 y el plotter entre I2 e I4. B usa el plotter entre I5 e I7 y la printer entre I6 e I8.
- **Zonas rayadas** = los dos procesos tendrían ***el mismo resource a la vez.*** Es imposible por mutual exclusion; el camino no puede entrar ahí.

![[Deadlock avoidance 2026-09-30 12.00.24.excalidraw|600]]

*La zona peligrosa* es el rectángulo *entre I1–I2 y I5–I6*, justo arriba y a la derecha del punto **t**. No está rayado, así que se puede entrar, pero ahí **A tiene la printer y B tiene el plotter**.

- **Antes de entrar al rectángulo** (en t, por ejemplo): el deadlock es **posible**, según el orden que tome el camino.
- **Una vez adentro**: el deadlock es **inevitable**, porque **cualquier** orden de ahí en adelante choca contra lo rayado. Los dos procesos van a pedir sí o sí lo que tiene el otro.


|                           | Qué sabés de cada proceso                                                  | Estado inseguro significa…                                                 |
| ------------------------- | -------------------------------------------------------------------------- | -------------------------------------------------------------------------- |
| **Banquero / Safe state** | Solo el **máximo** que **podría** pedir                                    | Deadlock **posible**. Quizás nadie llega a pedir su máximo, y se zafa      |
| **Trayectorias (p.24)**   | El **código exacto**: sabés que A **va a** ejecutar `down(&plotter)` en I2 | Deadlock **inevitable**, porque no hay "quizás": los pedidos van a ocurrir |

*Decisión en t*: B pide el plotter. Si el SO se lo da, el camino entra en la zona peligrosa. Lo seguro es *no dárselo* y correr *A hasta I4*, cuando A ya soltó todo.

Slide p.25, *"aquí no hay posibilidad de bloqueos"*: ahora A pide **primero el plotter y después la printer**, en el **mismo orden** que B. Las zonas rayadas quedan pegadas y no dejan ningún rincón donde quedar atrapado. Es lo mismo que viste en `Resource acquisition`: **mismo orden, sin deadlock**.
![[Screenshot 2026-09-30 at 12.05.24.png|417]]

#### Las estrategias, lado a lado

| Estrategia                                           | Cuándo actúa             | Qué necesita                           | Costo                                                         |
| ---------------------------------------------------- | ------------------------ | -------------------------------------- | ------------------------------------------------------------- |
| `=[[Ostrich algorithm]]`                             | Nunca                    | Nada                                   | Si pasa, reiniciar                                            |
| `=[[Deadlock prevention]]`                           | **Siempre**, por diseño  | Una regla fija que rompa una condición | Restringe a **todos**, siempre                                |
| `=[[Deadlock detection]]` + `=[[Deadlock recovery]]` | **Después** del deadlock | Correr la detección                    | Perder trabajo en el recovery                                 |
| **`=[[Deadlock avoidance]]`**                        | **Antes de cada pedido** | **Conocer los máximos** de antemano    | Chequear cada pedido; procesos esperando con resources libres |

#### Con qué se confunde
- **Avoidance vs `Deadlock prevention`** (la confusión clásica del parcial):
    - **Prevention** rompe una condición **por regla**, para siempre. Por ejemplo, "siempre pedir en orden numérico": la condición **nunca** puede darse.
    - **Avoidance** deja que **las 4 condiciones sean posibles**, pero **elige con cuidado** cada asignación para que el ciclo nunca llegue a formarse.
- **Avoidance vs `Banker's algorithm`**: avoidance es la **estrategia**; el banquero es **un algoritmo** para implementarla, el del caso con varias instancias.