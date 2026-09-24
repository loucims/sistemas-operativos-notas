---
titulo: Cambio de contexto
tipo: concepto
unidad: 2 - Procesos
estado: en-progreso
requiere: ["[[Process Control Block]]", "[[Interrupt]]", "[[Dual-mode operation]]"]
habilita: ["[[Scheduler]]", "[[Multiprogramming]]"]
creado: 2026-09-21
tags: [procesos, cpu]
---

# Cambio de contexto

## Definición

Guardar el estado de ejecución del proceso que está usando la CPU y cargar el
del proceso que va a usarla, de modo que cada uno pueda retomar exactamente
donde quedó.

## Qué problema resuelve

Hay más procesos que CPUs. Para que varios avancen "a la vez" hay que sacarle la
CPU a uno y dársela a otro, pero un proceso interrumpido a mitad de una
instrucción no puede perder sus registros ni su program counter: si los pierde,
no puede continuar. El cambio de contexto es lo que hace que la interrupción sea
reversible desde el punto de vista del proceso.

## Cómo funciona

1. Llega una `=[[Interrupt]]` (de reloj, de E/S) o el proceso hace una llamada al
   sistema. La CPU pasa a modo kernel.
2. El SO guarda el contexto del proceso saliente en su `=[[Process Control Block]]`: registros de
   propósito general, program counter, stack pointer, flags, puntero a tablas de
   memoria.
3. El `=[[Scheduler]]` elige el siguiente proceso de la cola de listos.
4. El SO carga el contexto del entrante desde su PCB y actualiza lo que dependa
   del proceso (registro de tabla de páginas, y con eso se invalida parte de la
   `=[[TLB]]`).
5. Se vuelve a modo usuario y la ejecución sigue en la instrucción donde el
   entrante había quedado.

## Por qué importa que sea caro

El trabajo del cambio en sí son microsegundos, pero el costo real es indirecto:
se pierde la localidad. Las cachés y la TLB quedan llenas de datos del proceso
anterior, y el entrante arranca con fallos hasta recalentarlas. Por eso un
quantum demasiado chico degrada el rendimiento aunque mejore la latencia.

## Se apoya en

- `=[[Process Control Block]]` — es el lugar donde se guarda el contexto
- `=[[Interrupt]]` — el disparador más común
- `=[[Dual-mode operation]]` — guardar el contexto requiere privilegios

## Habilita

- `=[[Scheduler]]` — sin poder cambiar de contexto, planificar no significa nada
- `=[[Multiprogramming]]` — la ilusión de paralelismo sobre una sola CPU

## Se confunde con

- **Cambio de modo** (usuario ↔ kernel): pasa en toda llamada al sistema y es
  mucho más barato. Todo cambio de contexto incluye un cambio de modo, pero no
  todo cambio de modo implica cambiar de proceso.
- **Cambio de contexto entre hilos del mismo proceso**: comparten espacio de
  direcciones, así que no hay que tocar la tabla de páginas ni invalidar la TLB.
  Es notablemente más barato.

## Preguntas de autoevaluación

- [ ] ¿Qué se guarda exactamente y dónde?
- [ ] ¿Por qué un cambio entre hilos del mismo proceso es más barato?
- [ ] ¿Qué relación hay entre el tamaño del quantum y el costo acumulado?

## Fuentes

- *(pendiente)*

Volver a `=[[Unidad 2 - Procesos]]`.
