---
requiere:
  - "[[Deadlock]]"
habilita: []
creado: 2026-09-30
---
Es la primera de las **4 estrategias** frente al deadlock (p.35):

|#|Estrategia|Idea|
|---|---|---|
|1|**Ignorar** (`Ostrich algorithm`)|Hacer como que no existe|
|2|**Prevenir** (`Deadlock prevention`)|Romper una de las 4 condiciones|
|3|**Detectar y recuperar** (`Deadlock detection` / `Deadlock recovery`)|Dejar que pase, encontrarlo y arreglarlo|
|4|**Evitar** (`Deadlock avoidance`)|Mirar cada pedido y no conceder los peligrosos|
El nombre viene del avestruz que mete la cabez en la tierra para no ver el problema.

#### Qué problema resuelve
Las otras 3 estrategias **cuestan**, y cuestan **siempre**: restricciones para los programadores, chequeos en cada pedido, algoritmos corriendo. Si el deadlock pasa una vez cada muchos años, pagar ese costo todo el tiempo **sale más caro que el problema**.

#### Cómo funciona
No hace nada. Si un día se cuelga, **se reinicia** (p.37).

**El ejemplo de la slide (p.36):**
- Tu usuario puede tener como máximo **1024 archivos abiertos**. Esa cantidad de "lugares" es un **resource finito**. Lo podés ver con `ulimit -a`.
- Supongamos que un proceso que no puede abrir un archivo se duerme un rato y lo reintenta.
- Ahora, 10 procesos que necesitan 110 archivos cada uno. Entre todos ya ocuparon los 1024 lugares, y a cada uno le faltan algunos:

```
Cada proceso:  tiene ~102 archivos abiertos  Y  espera un lugar libre
               → nadie suelta los suyos hasta terminar
               → nadie termina porque le faltan archivos
               → deadlock
```

Se cumplen las 4 condiciones. Pero la slide dice: _"Siendo realistas, simplemente no sucede."_

> _"El problema es evitable, pero solo a un alto costo."_

#### La cuenta que hay que hacer (trade-off)
```
¿Vale la pena ignorarlo?

  costo de prevenir     vs.   (probabilidad de deadlock) × (costo si pasa)
  (lo pagás SIEMPRE)          (lo pagás casi NUNCA)
```

|✅ Es razonable ignorarlo|❌ No se puede ignorar (p.37)|
|---|---|
|El deadlock es muy raro|Adentro del **kernel** del SO|
|Si pasa, reiniciar es barato|**Bases de datos**|
|No se pierde nada importante|**Sistemas transaccionales** (bancos, pagos)|
#### Con qué se confunde
- **Ostrich vs `Deadlock detection`**: los dos dejan que el deadlock pase. La diferencia es que **detection lo busca activamente** (ciclos en el grafo) y **se recupera sola**. Ostrich **no mira nada**: se da cuenta un humano ("se colgó") y reinicia a mano.
- **"Ignorar" ≠ "no saber"**: el avestruz **sabe** que el deadlock es posible. Decide conscientemente no pagar el costo de evitarlo.