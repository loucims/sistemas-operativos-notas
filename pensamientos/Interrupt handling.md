---
creado: 2026-09-28
tags: [e-s, cpu]
---
**Pregunta:** ¿qué pasa con las interrupciones que llegan mientras están deshabilitadas? ¿Hay una cola? ¿Qué pasa cuando llegan muchas, y cómo se recupera lo que se "perdió"?

**Respuesta corta:** no hay cola, hay un **bit de "pendiente" por dispositivo**. Si un dispositivo avisa varias veces mientras están apagadas, los avisos se funden en uno. No se pierden datos porque el dato no viaja en la interrupción: queda guardado en el **buffer del dispositivo**, y el handler, cuando corre, le pregunta cuánto hay y procesa todo junto.

## 1. El camino de una interrupción
1. Un dispositivo (disco, teclado, red, `=[[Timer]]`) quiere avisar algo. Levanta **su línea** hacia el **interrupt controller** (un chip entre los dispositivos y la CPU; en x86 se llama APIC).
2. El controlador marca esa línea como **pendiente**.
3. Si la CPU tiene las interrupciones **prendidas**, el controlador le avisa. La CPU termina la instrucción actual, pasa a modo kernel y salta al handler usando la `=[[Interrupt vector table]]`.
4. El handler atiende al dispositivo y le avisa al controlador "listo" (*acknowledge*). El bit de pendiente se borra.

Si las interrupciones están **apagadas**, el paso 3 espera: el bit queda marcado hasta que se prendan.

## 2. Un bit, no un contador
| Mientras estaban apagadas… | Al prenderlas se atiende |
|---|---|
| El disco avisó 1 vez | 1 vez |
| El disco avisó 3 veces | **1 vez** (los avisos se fundieron) |
| El timer hizo 5 ticks | **1 tick** |
| Avisaron el disco y el teclado | Las dos, por separado (son líneas distintas), **por prioridad** |

Si hay varias pendientes, el controlador entrega primero la de **mayor prioridad**, no la que llegó primero.

## 3. Cómo no se pierden datos: el buffer del dispositivo
La interrupción es solo un **timbre**: "tengo algo para vos". El dato en sí queda en el dispositivo:
- El teclado guarda las teclas apretadas en un pequeño buffer.
- La placa de red guarda los paquetes en un buffer circular en RAM (se los deja ahí con DMA).
- El disco deja el bloque leído en la RAM y marca "terminé".

Por eso el handler **nunca asume "llegó una cosa"**: lee el estado del dispositivo y procesa **todo** lo que haya acumulado. Así, tres avisos fundidos en uno no son un problema.

Lo que sí se puede perder: si el buffer del dispositivo se **llena** antes de que lo atiendan (por ejemplo, la placa de red recibe más paquetes de los que entran), lo que no entra se descarta. Por eso el kernel apaga las interrupciones lo menos posible.

**El caso del timer:** si se funden varios ticks, el contador de tiempo del sistema se atrasa. Los sistemas modernos no cuentan ticks para saber la hora: leen un **contador de hardware** que avanza solo (en x86, el TSC), así que la hora no se pierde aunque se fundan ticks.

## 4. Cuando llegan MUCHAS interrupciones
### Handlers en dos mitades (top half / bottom half)
Mientras corre el handler, las interrupciones de ese dispositivo (o todas) están apagadas. Para no tenerlas apagadas mucho tiempo, el trabajo se parte:
1. **Top half**: la parte urgente y cortita. Avisarle al dispositivo "te escuché", copiar el dato a un lugar seguro y anotar "hay trabajo pendiente". Corre con interrupciones apagadas.
2. **Bottom half**: el trabajo pesado (procesar el paquete de red, despertar al proceso que esperaba el disco). Corre **después**, con las interrupciones prendidas.

### Interrupt storm → pasar a polling
Una placa de red rápida puede generar cientos de miles de interrupciones por segundo. Cada una implica entrar al kernel, guardar registros y volver, así que la CPU se la pasa atendiendo timbres y no hace nada más (*interrupt storm* / *livelock*).

La solución (en Linux se llama NAPI):
1. Llega la primera interrupción de la placa.
2. El kernel **apaga las interrupciones de esa placa**.
3. Pasa a **polling**: le va preguntando "¿tenés más paquetes?" y los procesa de a tandas.
4. Cuando la placa se vacía, **vuelve a prender** sus interrupciones.

Es el mismo trade-off que `=[[Busy waiting]]` contra bloquearse: con poco tráfico conviene que el dispositivo avise (interrupción); con mucho tráfico conviene ir a preguntar (polling).

## Relación con la materia
- Es la razón práctica de por qué `=[[Disabling interrupts]]` tiene que ser corto: cuanto más tiempo están apagadas, más avisos se funden y más se arriesga a que se llenen los buffers.
- Una interrupción se atiende tanto en modo usuario como en modo kernel, y al atenderla la CPU pasa a modo kernel (`=[[Interrupt]]`, pregunta del parcial).
- Ver también `=[[Dual-mode operation]]`: apagar interrupciones es una instrucción privilegiada.
