---
creado: 2026-09-29
tags: [memoria, cpu, concurrencia]
---
**Pregunta:** ¿cómo funciona el bus de memoria? ¿Qué significa que `=[[Test-and-set]]` "bloquee el bus"?

**Respuesta corta:** el bus es el "camino" compartido por el que la CPU (y los dispositivos) le piden datos a la `=[[RAM]]`. Como es compartido, hay un árbitro que decide quién lo usa en cada momento. Una instrucción atómica reserva ese camino (o, en CPUs modernas, un pedacito de la cache) mientras hace su lectura + escritura, para que nadie se meta en el medio.

## 1. Qué es un bus
Un conjunto de cables compartidos entre varios componentes. Un bus de memoria tiene tres grupos de líneas:

| Líneas | Qué llevan |
|---|---|
| **Dirección** | *Dónde*: la dirección de memoria que se quiere leer o escribir |
| **Datos** | *Qué*: el valor leído o a escribir |
| **Control** | *Cómo*: "leer" o "escribir", "listo", "quiero el bus"… |

```
  CPU 0      CPU 1      Disco (DMA)
    │          │           │
 ═══╪══════════╪═══════════╪═══════  ← bus (dirección + datos + control)
               │
      controlador de memoria
               │
              RAM
```

**Una lectura, paso a paso:**
1. La CPU pone la dirección en las líneas de dirección y "leer" en las de control.
2. El controlador de memoria busca ese dato en la RAM.
3. Pone el valor en las líneas de datos y avisa "listo".
4. La CPU lo copia a un registro.

La RAM es **lenta** comparada con la CPU: una lectura puede tardar cientos de ciclos de CPU.

## 2. Un camino compartido → arbitraje
Si dos CPUs (o una CPU y el disco haciendo DMA) quieren el bus a la vez, no pueden usarlo las dos: los cables llevarían dos direcciones mezcladas. Un **árbitro** decide quién va primero, y el otro espera.

Eso ya da algo interesante: **cada lectura o escritura individual es atómica**, porque el bus hace una transacción por vez. El problema aparece cuando una operación necesita **dos** transacciones (leer y después escribir, como `x++` o el "mirar y marcar" de `=[[Lock variable]]`): entre la primera y la segunda, otra CPU puede agarrar el bus.

## 3. Cómo es atómica una instrucción como TSL
**Forma clásica (la de la slide):** durante el TSL, la CPU activa una señal especial, `LOCK`, que le dice al árbitro "no le des el bus a nadie hasta que termine". Hace la lectura y la escritura, y suelta el bus. Nadie pudo meterse en el medio.

Problema: mientras tanto, **todas** las otras CPUs quedan frenadas, aunque quieran otra dirección que no tiene nada que ver.

## 4. Lo que pasa en CPUs modernas: caches
Para no ir a la RAM todo el tiempo, cada núcleo tiene su propia **cache**: una memoria chiquita y rapidísima con copias de lo que usó hace poco. La RAM se copia a la cache en bloques de 64 bytes llamados **cache lines**.

```
  Núcleo 0          Núcleo 1
 ┌────────┐        ┌────────┐
 │ cache  │        │ cache  │
 └───┬────┘        └───┬────┘
 ════╪═════════════════╪══════ bus / interconexión
            RAM
```

Esto trae un problema nuevo: si los dos núcleos tienen una copia de `lock` y uno la cambia, ¿cómo se entera el otro?

**Cache coherence:** los núcleos se "escuchan" por el bus (*snooping*) y siguen reglas para que nunca haya dos versiones distintas del mismo dato. Versión simplificada:
- Varios núcleos pueden tener una copia **para leer** a la vez.
- Para **escribir**, un núcleo tiene que pedir la línea en **exclusiva**: avisa por el bus, y los demás **tiran (invalidan) su copia**.
- Si otro núcleo quiere leerla después, tiene que pedirla de nuevo, y recibe la versión nueva.

**TSL moderno:** en vez de bloquear todo el bus, el núcleo pide en exclusiva **solo la cache line** del lock, hace la lectura y la escritura adentro de su cache, y mientras tanto **demora** cualquier pedido de otro núcleo por esa línea. Las otras CPUs pueden seguir usando el resto de la memoria libremente. Mismo efecto (nadie se mete en el medio), mucho más barato.

## 5. Consecuencia práctica: el spinlock que satura el bus
Con `while (TSL(&lock) == 1);`, cada TSL es una **escritura** (pone 1 aunque ya valiera 1). Entonces:
1. El núcleo A pide la línea en exclusiva → B pierde su copia.
2. B hace TSL → pide la línea en exclusiva → A pierde su copia.
3. Y así sin parar: la cache line "rebota" entre núcleos y el bus se llena de tráfico, frenando a todos (incluido el que tiene el lock y quiere salir).

**Arreglo: test-and-test-and-set.** Primero girar **leyendo** (lectura normal, sale de la propia cache y no genera tráfico); recién cuando parece libre, intentar el TSL:
```c
while (1) {
    while (lock == 1) ;          // girar LEYENDO: no molesta a nadie
    if (TSL(&lock) == 0) break;  // parece libre: ahora sí, intento atómico
}
```

## Relación con la materia
- Explica la frase de la slide de TSL: *"El bus de memoria se bloquea durante esta instrucción, otras CPUs no pueden interferir."*
- El **store buffer** que rompe a `=[[Peterson's solution]]` está justo entre el núcleo y su cache: las escrituras esperan ahí antes de entrar al sistema de coherencia. Las **barreras de memoria** obligan a vaciarlo.
- El disco y la placa de red también usan el bus para dejar datos en RAM (DMA). Ver `pensamientos/Interrupt handling`.
- Es otra razón por la que el `=[[Busy waiting]]` es caro con varias CPUs: además de gastar CPU, puede saturar el bus.
