---
requiere:
  - "[[Resource allocation graph]]"
habilita:
  - "[[Deadlock recovery]]"
creado: 2026-09-30
---
#### Qué problema resuelve
Con ostrich no mirás nada. Pero hay sistemas donde no podés ignorar el deadlock, como una base de datos, y donde prevenirlo sale muy caro. La alternativa es **dejar que pase, encontrarlo y arreglarlo** (p.4–5). Esta estrategia tiene dos partes:

- **Detection**: encontrar el deadlock. Es este concepto. 
> ^ *Solamente en el instante o foto actual del sistema*, lo representado por el grafo. Puede ser que en la siguiente iteracion este en deadlock.
- **Recovery**: arreglarlo. Es el ⭐8.

#### Caso 1: una instancia de cada resource
Es lo que ya sabés:

1. Armar el `Resource allocation graph`.
2. Buscar ciclos. Es un problema clásico de grafos, con algoritmos conocidos.
3. **Ciclo = deadlock**, y los procesos que están **en el ciclo** son los que están en deadlock.

#### Caso 2: varias instancias del mismo resource
Acá **un ciclo no alcanza**. Mirá el ejemplo de la p.17. Recurso 2 tiene **2 instancias**, una asignada a B y otra a C:

|Proceso|Tiene|Espera|
|---|---|---|
|A|Recurso 1|Recurso 2|
|B|Recurso 2 (una instancia)|Recurso 1|
|C|Recurso 2 (la otra instancia)|nada|

Hay un ciclo: A → R2 → B → R1 → A. Pero **C no espera nada**: puede terminar y soltar su instancia de R2. A la agarra, termina y suelta R1. Después B agarra R1 y termina. **No hay deadlock.**

El algoritmo general consiste en **simular que los procesos terminan**. Esta forma viene de Tanenbaum, cap. 6.4.2, que lo hace con matrices; las slides solo muestran el grafo.

1. Buscar un proceso cuyo pedido **se pueda satisfacer** con lo que está libre.
2. Suponer que termina y **libera todo** lo que tiene.
3. Repetir con lo que quedó libre.
4. Los procesos que **nunca** se pudieron marcar están en deadlock.

Aplicado a la p.17:

|Paso|¿Quién puede terminar?|Libera|Queda libre|
|---|---|---|---|
|1|C (no espera nada)|1 instancia de R2|R2 ×1|
|2|A (pedía R2 → ahora hay)|R1 y R2|R1, R2 ×2|
|3|B (pedía R1 → ahora hay)|todo|todo|
|—|**Terminaron todos → no hay deadlock**|||

#### El problema práctico: ¿cuándo correrlo? (p.13, p.22) ⭐

Esta es la segunda mitad de la pregunta del parcial. Detectar **cuesta**, porque hay que recorrer el grafo entero. Entonces, ¿cada cuánto lo corrés?

| Cuándo                                    | Ventaja                                                                     | Desventaja                                                                                                                                                       |
| ----------------------------------------- | --------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **En cada pedido** de resource            | Lo encontrás **apenas pasa**, y sabés **quién lo causó** (el último pedido) | **Carísimo**: pagás el recorrido en cada `down`                                                                                                                  |
| **Cada tanto** (ej. cada k minutos)       | Barato                                                                      | Los procesos quedan colgados hasta la próxima corrida. Y (p.22) _"puede haber muchos ciclos… no podríamos saber cuál de los muchos procesos 'causó' el bloqueo"_ |
| **Cuando baja el uso de CPU** (Tanenbaum) | Es una señal indirecta: muchos procesos bloqueados → CPU ociosa             | Es una heurística; puede tardar en darse cuenta                                                                                                                  |

La slide p.22 dice que la decisión depende de:
- **Con qué frecuencia** es probable que ocurran deadlocks.
- **Cuántos procesos** vas a tener que revertir: al menos **uno por cada ciclo disjunto**.

#### Con qué se confunde
- **Detection vs `Deadlock avoidance`**: detection **mira después**, cuando el deadlock ya pasó. Avoidance **mira antes** de conceder cada pedido, y lo retrasa si crearía un ciclo o un estado peligroso (p.2).
- **Detection vs `Deadlock recovery`**: detection solo **dice** que hay deadlock y quiénes están en él. No arregla nada.
- **Detection vs `Ostrich algorithm`**: los dos dejan que pase. Ostrich no lo busca; detection sí.