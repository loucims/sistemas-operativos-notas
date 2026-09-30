---
requiere:
  - "[[Mutual exclusion]]"
habilita:
  - "[[Lock variable]]"
  - "[[Strict alternation]]"
  - "[[Test-and-set]]"
creado: 2026-09-28
---
Toda solución de `=[[Mutual exclusion]]` necesita que un hilo *espere* cuando la región crítica está ocupada. Hay dos formas de esperar:

1. **Girar**: preguntar una y otra vez "¿ya está libre?" hasta que lo esté. Eso es busy waiting.
2. **Bloquearse**: pedirle al kernel "dormime y despertame cuando se libere".

#### Cómo funciona
```c
while (ocupado)
    ;               // cuerpo vacío: no hace nada, vuelve a preguntar
// salió del while → está libre → entra a la región crítica
```

El `;` suelto es un loop **sin cuerpo**. El hilo no avanza, pero **está corriendo**: está en estado _running_, usando la CPU para preguntar millones de veces por segundo.


Comparemos las dos formas de esperar:

|                                 | **Busy waiting** (girar)              | **Bloquearse**                                    |
| ------------------------------- | ------------------------------------- | ------------------------------------------------- |
| Estado del hilo mientras espera | *Running*                             | *Blocked*                                         |
| ¿Usa CPU mientras espera?       | Sí, el 100% de su turno               | No: la CPU la usan otros                          |
| ¿Pasa por el kernel?            | No                                    | Sí: syscall para dormirse y otra para despertarlo |
| Costo                           | La CPU que gasta girando              | 2 cambios de contexto (dormir + despertar)        |
| Cuándo conviene                 | Esperas *muy cortas*, con varias CPUs | Esperas largas o de duración desconocida          |
La regla de decisión: si la espera va a durar **menos que dos cambios de contexto**, girar es más barato que bloquearse. Por eso el kernel usa spinlocks internamente para esperas cortitas (slide: _"son útiles para esperas cortas, en los kernels de los SO"_).

#### Los dos problemas del busy waiting (slide "Spinlock")
**1. Malo: gasta CPU.** Con **una sola CPU** es peor todavía: mientras B gira, A (que tiene el lock) **no está corriendo**, así que no puede liberarlo. B se gasta todo su quantum girando al pedo.

**2. Peor: puede trabarse para siempre (_priority inversion_).** Con un scheduler que siempre corre al de mayor prioridad:

| #   | H (Prioridad alta)                                                    | L (Prioridad baja)                                                       |
| --- | --------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| 1   | blocked (esperando E/S)                                               | corre, **toma el lock**, entra a la región crítica                       |
| 2   | llega su E/S → ready. Como es más prioritario, **le saca la CPU a L** | ready, **con el lock tomado**                                            |
| 3   | quiere el lock → está ocupado → **gira**                              | ready. Nunca lo eligen, porque H siempre está ready y es más prioritario |
| 4   | gira… gira… para siempre                                              | nunca corre → nunca libera el lock                                       |

H espera a L, y L no puede correr porque H nunca deja la CPU. Ninguno avanza.
Por eso la conclusión de la slide: _"Para uso general, vamos a preferir una solución **bloqueante**."_ Es lo que viene con `Sleep and wakeup`, `Semaphore` y `Mutex`.

*Con una sola CPU*, el busy waiting **no tiene sentido**: lo mejor es ceder la CPU enseguida (bloquearse), así A puede terminar antes. Con **varias CPUs** es distinto: A puede estar corriendo en otro núcleo y liberar el lock mientras B gira, y si la región crítica es corta, B entra enseguida.

#### Con qué se confunde
- **Busy waiting ≠ blocked**: el que gira está _running_. Para el scheduler es un hilo trabajando, así que no sabe que está esperando.
- **Busy waiting no es "incorrecto"**: Peterson, strict alternation y TSL son soluciones **correctas** (cumplen mutual exclusion) y usan busy waiting. El problema es de **eficiencia**, y en el caso de las prioridades, de progreso.
- **Busy waiting != spinlock**: Busy waiting es la *tecnica*, esperar girando en un loop mientras preguntas una y otra vez. Mientras que *Spinlock* es un *lock que espera con esa tecnica*: una variable (libre / ocupado) mas dos funciones, `acquire` (tomar) y `release` (soltar). Si el lock esta ocupado, `acquire` gira.

