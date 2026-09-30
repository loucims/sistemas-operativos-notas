---
requiere:
  - "[[Safe state]]"
habilita:
  - "[[Deadlock avoidance]]"
creado: 2026-09-30
---
#### Qué problema resuelve
`Safe state` te da un **test**: dado un estado, ¿es seguro? Falta la **política**, o sea qué hace el SO **cada vez que un proceso pide algo**. El banquero (Dijkstra, 1965) es esa política: ***solo concede pedidos que dejan al sistema en un estado seguro.***

La analogía del nombre: un banquero tiene poca plata en caja, y cada cliente tiene un *límite de crédito* (su Máx). El banquero solo presta si, después de prestar, *todavía puede garantizar* que todos los clientes lleguen a su límite en algún orden y devuelvan la plata.

#### Cuándo se usa (p.32–33)
- Con **una instancia** de cada resource alcanza con el `Resource allocation graph`.
- Con **varias instancias** se usa el **banquero** (p.32).
- Requisitos (p.33):
    - Cada proceso **declara su máximo de antemano**.
    - Cuando un proceso termina de recibir todo lo que necesita, **devuelve todo en un tiempo finito**.

#### Cómo funciona: frente a cada pedido
1. ¿El pedido cabe en lo que le *Necesita*? Si no, es un error: pidió más que su máximo.
2. ¿Hay suficientes *Libres*? Si no, *espera*.
3. *Simulá* que se lo das: *Tiene* sube, *Libres* baja.
4. Corré el test de `Safe state` sobre ese estado simulado:
    - **Seguro** → se concede.
    - **Inseguro** → **no se concede**: se deshace la simulación y el proceso **espera**.

⚠️ Esta es la parte que confunde (p.33, _"puede que tenga que esperar"_): el banquero puede hacer esperar a un proceso **aunque haya resources libres**. No le falta stock: concederlo sería peligroso.

#### Desventajas (p.34)

Por esto casi no se usa en la práctica:
- **Rara vez se sabe de antemano** el máximo que va a necesitar un proceso.
- **La cantidad de procesos cambia** todo el tiempo.
- **Un resource disponible puede desaparecer**: por ejemplo, se rompe una impresora.

#### Con qué se confunde
- **Banquero vs `Safe state`**: safe state es el **test**; el banquero es la **política** que corre ese test en cada pedido.
- **Banquero vs `Deadlock detection`**: los dos simulan que los procesos terminan. Detection lo hace **después**, con los pedidos actuales. El banquero lo hace **antes de conceder**, con los máximos.
- **"No concede" ≠ "rechaza para siempre"**: el proceso **espera** y se reevalúa cuando otro libere resources.


#### Ejemplo en formato de parcial: 10 instancias
1. Ejercicio tipo parcial. Hay **12** instancias:

| Proceso | Tiene | Máx | **Necesita** |
| ------- | ----- | --- | ------------ |
| A       | 3     | 8   | 5            |
| B       | 2     | 5   | 3            |
| C       | 4     | 6   | 2            |
| D       | 1     | 4   | 3            |
*Libres* = 12 - (3+2+4+1) = 2

C okay
libres = 6

B okay
libres = 8

D okay
libres = 9

A okay 
libres = 12

2. Si C pide 1 mas?

| Proceso | Tiene | Máx | **Necesita** |
| ------- | ----- | --- | ------------ |
| A       | 3     | 8   | 5            |
| B       | 2     | 5   | 3            |
| C       | 5     | 6   | 1            |
| D       | 1     | 4   | 3            |
Libres = 1

C okay
Libres = 6
... y sigue normalmente

*Tabla de estados*

|            | inicio (C recibió 1) | C recibe 1 | C termina | B recibe 3 | B termina | D recibe 3 | D termina | A recibe 5 | A termina |
| ---------- | -------------------- | ---------- | --------- | ---------- | --------- | ---------- | --------- | ---------- | --------- |
| A          | 3/8                  | 3/8        | 3/8       | 3/8        | 3/8       | 3/8        | 3/8       | 8/8        | —         |
| B          | 2/5                  | 2/5        | 2/5       | 5/5        | —         | —          | —         | —          | —         |
| C          | 5/6                  | 6/6        | —         | —          | —         | —          | —         | —          | —         |
| D          | 1/4                  | 1/4        | 1/4       | 1/4        | 1/4       | 4/4        | —         | —          | —         |
| **Libres** | **1**                | **0**      | **6**     | **3**      | **8**     | **5**      | **9**     | **4**      | **12** ✔️ |


3. Si D pide 1 mas?

| Proceso | Tiene | Máx | **Necesita** |
| ------- | ----- | --- | ------------ |
| A       | 3     | 8   | 5            |
| B       | 2     | 5   | 3            |
| C       | 4     | 6   | 2            |
| D       | 2     | 4   | 2            |

Libres = 1
No hay ninguno que se pueda terminar con la cantidad de libres que hay, estado inseguro!