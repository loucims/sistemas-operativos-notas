---
requiere:
  - "[[Process states]]"
  - "[[Mutual exclusion]]"
habilita:
  - "[[Producer-consumer problem]]"
  - "[[Semaphore]]"
creado: 2026-09-29
---
#### Qué problema resuelve
Todas las soluciones que viste hasta ahora esperan **girando** (`Busy waiting`): gastan CPU, no sirven con una sola CPU y pueden trabarse con prioridades. La slide del spinlock terminaba con _"para uso general, vamos a preferir una solución **bloqueante**"_.

La idea: en vez de girar preguntando "¿ya puedo?", el hilo le dice al kernel **"dormime"**, y otro hilo, cuando la cosa esté lista, le dice al kernel **"despertalo"**.

#### Cómo funciona (slide U2_4)
Son dos llamadas al sistema **abstractas** (la slide aclara que _no son los nombres de llamadas reales_):

| Llamada     | Qué hace                                                            | Transición de estado  |
| ----------- | ------------------------------------------------------------------- | --------------------- |
| `sleep()`   | El que la llama se bloquea hasta que alguien lo despierte           | running → **blocked** |
| `wakeup(p)` | Desbloquea al proceso `p` (que tiene que haber hecho `sleep` antes) | blocked → **ready**   |
Mientras duerme, el proceso está **blocked**, no gasta CPU, y el scheduler se la da a otros. Es lo mismo que pasa cuando un proceso hace `read()` del disco (`Process states`), pero esperando a **otro proceso** en vez de a un dispositivo.

#### Ejemplo: esperar a que haya datos
Un consumidor espera a que haya algo en un buffer, y un productor lo llena:
```c
int count = 0;                      // compartida: cuántos items hay

// ───────────── Consumidor ─────────────
if (count == 0)
    sleep();                        // no hay nada → me duermo
item = remove_item();
count = count - 1;

// ───────────── Productor ──────────────
insert_item(item);
count = count + 1;
if (count == 1)                     // estaba vacío → el consumidor capaz                                        duerme
    wakeup(consumidor);
```
Si el consumidor llega y no hay nada, se duerme. Cuando el productor pone el primer item, lo despierta.
#### El problema: el wakeup perdido (slide "Productor/Consumidor, solución parcial")
**Chequear `count == 0`** y **llamar a `sleep()`** son **dos pasos**. Si el cambio de proceso cae justo en el medio:

|#|Consumidor|Productor|`count`|
|---|---|---|---|
|1|lee `count == 0` → "voy a dormir"||0|
|—|_⏰ interrupción antes del `sleep()`_|||
|2||inserta, `count = 1`|1|
|3||`count == 1` → `wakeup(consumidor)` → **pero no está durmiendo** → el aviso **se pierde**|1|
|—|_⏰ vuelve el consumidor_|||
|4|`sleep()` (ya había decidido dormir) → **blocked**||1|
|5|duerme **para siempre**: el productor ya mandó su único wakeup||1|
Slide: _"Problema principal: hubo una carrera — el wakeup se perdió."_

Es **el mismo patrón** que la `Lock variable`: **mirar** (`count == 0`) y **actuar** (`sleep()`) no son atómicos. Y además `wakeup` **no tiene memoria**: si llega cuando nadie duerme, desaparece.
#### Hacia dónde va esto

La solución natural es que los wakeups **no se pierdan**: guardarlos en algún lado para que un `sleep` posterior sepa que ya lo despertaron. Esa idea, llevada a un contador y hecha atómica, es el **`Semaphore`**.

#### Con qué se confunde

- **Sleep ≠ busy waiting**: `sleep` bloquea (no gasta CPU), y busy waiting gira (sí gasta).
- **Sleep/wakeup no dan mutual exclusion por sí solos**: son para **esperar un evento** (sincronización), no para proteger una región crítica. Y encima tienen su propia race condition.
- **No es el `sleep(5)` de C**, que duerme una cantidad de segundos. Este duerme **hasta que alguien lo despierte**.