---
requiere:
  - "[[Scheduling criteria]]"
habilita:
  - "[[Shortest Job First]]"
  - "[[Round Robin]]"
creado: 2026-09-29
---
#### Qué es (FCFS)
El algoritmo más simple: **la ready queue es una fila FIFO**, como la del banco.
- El que llega se pone **al final**.
- El scheduler elige **siempre al primero** de la fila.
- Ese proceso **corre hasta terminar su ráfaga de CPU**: termina o se bloquea. **No apropiativo**: nadie lo saca.
- Si se bloquea (por ejemplo, para leer del disco), cuando vuelve a ready *se pone **al final** de la fila otra vez.*

#### El problema: el efecto convoy
El ejemplo de la slide: P1 = 24, P2 = 3, P3 = 3, todos llegan en t = 0.

| Orden de llegada | Gantt                             | Espera promedio            | Retorno promedio            |
| ---------------- | --------------------------------- | -------------------------- | --------------------------- |
| P1, P2, P3       | \| P1 (24) \| P2 (3) \| P4 (3) \| | (0 + 24 + 27) / 3 = **17** | (24 + 27 + 30) / 3 = **27** |
| P2, P3, P1       | \| P2 (3) \| P3 (3) \| P4 (24) \| | (0 + 3 + 6) / 3 = **3**    | (3 + 6 + 30) / 3 = **13**   |
**Los mismos procesos**, y solo por el orden en que llegaron, la espera promedio pasa de 3 a 17. Cuando un proceso largo llega primero, todos los cortos quedan atrás, como autos detrás de un camión en una ruta de un carril. Eso se llama **efecto convoy**. Es lo que la slide llama _"grandes fluctuaciones en el tiempo promedio de retorno"_.

#### Ventajas y desventajas (slide)

|✅|❌|
|---|---|
|**Simple** y con **poca sobrecarga**: no hay que calcular nada, y hay pocos cambios de contexto|**Efecto convoy**: los cortos esperan detrás de los largos|
|**Sin inanición**: todos terminan siendo atendidos|**No sirve para sistemas interactivos**: un proceso largo congela a todos los demás|
|Justo en el sentido de "orden de llegada"|Los I/O-bound quedan atrás de los CPU-bound, y los dispositivos se quedan parados|

#### Cuando llegan en momentos distintos
Hasta ahora todos llegaban en t = 0. Si no, las fórmulas tienen que usar el **arrival time** (el momento en que llega):
- **Turnaround time** = completion − arrival
- **Waiting time** = **suma de todos los ratos que pasa en la ready queue**. La forma más fácil de calcularlo, sirva para cualquier algoritmo, es: **waiting time = turnaround time − burst time** (todo lo que tardó, menos lo que estuvo corriendo).

En **FCFS** hay un atajo: como un proceso nunca es interrumpido, tiene **un solo** rato en la ready queue, el de antes de empezar. Entonces ahí, y solo ahí, **waiting time = start − arrival**. En algoritmos que cortan procesos (Round Robin, SRTN), hay que sumar **todos** los ratos, o usar turnaround − burst.

| Proceso | Arrival | Burst |
| ------- | ------- | ----- |
| P1      | 0       | 5     |
| P2      | 1       | 3     |
| P3      | 2       | 1     |
Entonces seria algo como: 
```
 | P1              | P2        | P3 |
 0                 5           8    9
```

|Proceso|Start|Completion|Turnaround (completion − arrival)|Waiting (turnaround − burst)|
|---|---|---|---|---|
|P1|0|5|5 − 0 = **5**|5 − 5 = **0**|
|P2|5|8|8 − 1 = **7**|7 − 3 = **4**|
|P3|8|9|9 − 2 = **7**|7 − 1 = **6**|
|**Promedio**|||19 / 3 = **6,33**|10 / 3 = **3,33**|
(Con el atajo de FCFS da lo mismo: P2 empieza en 5 y llegó en 1 → 4. P3 empieza en 8 y llegó en 2 → 6.)

Cuidado: si la CPU queda libre y el próximo proceso **todavía no llegó**, la CPU queda ociosa hasta que llegue. En el Gantt se dibuja como un hueco.