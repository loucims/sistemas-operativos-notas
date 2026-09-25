---
requiere:
  - "[[Kernel]]"
  - "[[Timer]]"
  - "[[Multiprogramming]]"
habilita:
  - "[[Scheduler]]"
creado: 2026-09-25
---
Con `=[[Multiprogramming]]`, el kernel cambia de programa solo cuando el *que esta corriendo tiene que esperar.* Pero si un proceso tarda 10 minutos de puro calculo, se *congelaria toda la maquina* por esos 10 minutos.

El tiempo de respuesta tiene que *ser menor a 1 segundo* para que se sienta fluida la maquina, por lo tanto, se le *saca la CPU a un programa aunque no quiera soltarla*. Eso es **Multitasking**.

En **Multitasking**, cada programa *recibe un pedacito de tiempo* llamado *quantum* (por ejemplo 10ms). El `=[[Timer]]` se encarga de que se cumpla.

Si un proceso no termina ni espera, eventualmente el timer setteado al quantum llega 0 *y genera un interrupt*.
La kernel le saca la CPU al proceso actual (esto se le llama *preemption* o *expropiacion* y es un cambio involuntario), y luego de recargar el Timer, se *la da al siguiente proceso en estado listo*. 

#### Con qué se confunde

|                             | Multiprogramming      | Multitasking                                   |
| --------------------------- | --------------------- | ---------------------------------------------- |
| Objetivo                    | CPU no ociosa         | respuesta rapida al usuario                    |
| Cambia de programa cuando.. | el que corre *espera* | el que corre espera o *se le acaba el quantum* |
| Necesita el Timer           | no                    | si                                             |
| Tipo de cambio              | solo voluntario       | voluntario e *involuntario* (preemption)       |

==Multitasking **incluye** a Multiprogramming: sigue aprovechando las esperas, y además corta por tiempo.==

**Multitasking vs. paralelismo.** Con una sola CPU los programas **se turnan** muy rápido; no corren en el mismo instante. Correr en paralelo de verdad requiere varios núcleos.

#### Ejemplo
**1. El kernel le da la CPU a A y carga el Timer con 10 ms**
- **Por qué:** es el "despertador" del concepto anterior. El kernel se asegura de volver a tener el control a los 10 ms.

**2. A corre**
- Si A hace un `read()` y tiene que esperar antes de los 10 ms, se cambia de programa como en Multiprogramming. Ese es un cambio **voluntario**: A soltó la CPU porque la necesitaba soltar.

**3. Si A sigue calculando, el Timer llega a 0 y genera un Interrupt**
- **Qué hace:** la CPU entra al kernel por el handler del Timer.
- **Por qué:** A **no quería** parar, pero el kernel se la saca igual. Esto se llama **preemption** (expropiación), y es un cambio **involuntario**.

**4. El kernel elige al siguiente listo (B), recarga el Timer y le da la CPU**
- **Por qué:** A queda "en la fila" para su próximo turno. No se lo mata; solo se lo pausa. Cómo se guarda dónde iba A para retomarlo después es Cambio de contexto (Unidad 2).

**5. Se repite: A, B, C, A, B, C…**
- Con 3 programas y quantum de 10 ms, cada uno vuelve a tener la CPU cada ~30 ms. Una persona no nota pausas tan cortas (hace falta del orden de 100 ms para empezar a notarlas), así que **parece que los tres corren a la vez**.
```
CPU:    [A][B][C][A][B][C][A][B][C] ...
         ↑  ↑  ↑
       cada cambio lo dispara el Timer (o que el programa espere antes de terminar su quantum)
```

La diapositiva agrega que, con muchos programas listos a la vez, el kernel necesita decidir **a quién le toca**: eso es la **planificación de la CPU** (CPU scheduling), Unidad 2.

