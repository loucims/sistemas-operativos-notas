---
requiere:
  - "[[Readers-writers problem]]"
  - "[[Dining philosophers problem]]"
  - "[[Producer-consumer problem]]"
habilita: []
creado: 2026-09-29
---
#### Qué es
Un proceso **podría avanzar**, pero **nunca le toca**, porque **siempre** hay otro que pasa antes. Mientras tanto, **el sistema sigue funcionando**: los demás avanzan sin problema. Solo ese uno queda esperando para siempre.

Es exactamente la violación del requisito 3 de `Region critica`: **espera limitada** ("ningún proceso debería tener que esperar indefinidamente").

#### Ya la viste varias veces

| Dónde                                                  | Quién se muere de hambre | Por qué                                                              |
| ------------------------------------------------------ | ------------------------ | -------------------------------------------------------------------- |
| `=[[Readers-writers problem]]`                         | El **escritor**          | Siempre entra un lector nuevo antes de que `rc` llegue a 0           |
| `=[[Test-and-set]]` (spinlock)                         | Un hilo con mala suerte  | Cuando se libera el lock, entra el que haga TSL primero. No hay fila |
| `=[[Compare-and-swap]]`                                | Un hilo con mala suerte  | Su CAS puede fallar siempre, si otros siempre le ganan               |
| `=[[Semaphore]]` que despierta "al de mayor prioridad" | El de **baja prioridad** | Siempre hay uno más prioritario esperando                            |
| `=[[Dining philosophers problem]]` (jerárquica)        | Un filósofo              | Sus vecinos pueden turnarse los tenedores y dejarlo afuera           |
| Scheduler que **siempre** prioriza I/O-bound           | El **CPU-bound**         | Lo que viste en `CPU-bound and I-O-bound`                            |
#### El patrón común
Siempre hay una **regla de elección** que puede favorecer a **otros** una y otra vez, y que **no tiene memoria de cuánto esperaste**: "el de más prioridad", "el más corto", "el lector si ya hay lectores", "el primero que llegue al TSL".
#### Cómo se evita

| Técnica                              | Idea                                                                                                     | Ejemplo                                                                                                  |
| ------------------------------------ | -------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------- |
| **Orden FIFO**                       | Atender por orden de llegada                                                                             | La fila de un semáforo, que despierta al que espera hace más tiempo                                      |
| **Aging** (_envejecimiento_)         | Cuanto más esperás, **más prioridad** tenés. Tarde o temprano pasás a todos                              | Slide U2_5, "Ajustes de prioridad": _"aumentar la prioridad de los procesos que no se están ejecutando"_ |
| **Límite**                           | Después de N veces seguidas, el favorecido **tiene que ceder**                                           | Readers-writers: si hay un escritor esperando, no dejar entrar más lectores nuevos                       |
| **Contas las veces que fue victima** | Meter en el costo cuántas veces ya la eligieron. Cuantas más, más cara, hasta que deja de ser la elegida | Recovery de deadlocks. Es una forma de aging                                                             |
#### Deadlock vs livelock vs starvation

|                       | **Deadlock**                                       | **Livelock**                                    | **Starvation**                                  |
| --------------------- | -------------------------------------------------- | ----------------------------------------------- | ----------------------------------------------- |
| ¿Los procesos corren? | ❌ Todos **dormidos**                               | ✅ Todos **corriendo**                           | ✅ Los demás sí                                  |
| ¿Alguien avanza?      | ❌ Nadie                                            | ❌ Nadie (hacen cosas, pero no progresan)        | ✅ **Todos menos uno** (o unos pocos)            |
| ¿Se destraba solo?    | ❌ Nunca                                            | Puede, con suerte                               | Puede, si en algún momento dejan de adelantarlo |
| Ejemplo               | Los 5 filósofos con un tenedor cada uno, esperando | Los filósofos que toman y sueltan sincronizados | El escritor que nunca escribe                   |

La diferencia clave: en el **deadlock** el sistema **entero** está trabado; en la **starvation**, el sistema funciona, pero **no es justo** con alguien.