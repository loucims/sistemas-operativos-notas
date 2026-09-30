---
requiere:
  - "[[Thread]]"
  - "[[Process Control Block (PCB)]]"
habilita: []
creado: 2026-09-27
---
#### Qué problema resuelve
Dos cosas:
1. **Hilos en un SO que no los soporta.** Los SO viejos solo conocían procesos. Si querías hilos, tenías que armarlos vos, sin ayuda del kernel.
2. **Velocidad.** Cambiar de hilo pasando por el kernel implica un trap, un cambio de modo y el trabajo del scheduler. Si el cambio se hace **sin entrar al kernel**, es muchísimo más rápido.

La idea: que los hilos los maneje **una librería dentro del propio proceso**, en modo usuario. El kernel ni se entera de que existen.
- El kernel ve **un solo PCB** y un solo hilo.
- Si un hilo hace `read()` y bloquea, el kernel bloquea **al proceso entero**, y todos los hilos se frenan. Es la gran desventaja.
Modelo **many-to-one**: muchos hilos de usuario sobre **una** sola entidad que el kernel conoce (el proceso).
#### Cómo funciona
```
 PROCESO (user mode)
 ┌──────────────────────────────────────────┐
 │  hilo 1    hilo 2    hilo 3              │
 │     └─────────┼─────────┘                │
 │         librería de hilos (runtime)      │
 │         ├── thread table (un TCB por hilo)│
 │         └── su propio scheduler          │
 └──────────────────────────────────────────┘
 ──────────────────────────────────────────────
 KERNEL: ve UN solo proceso, UN solo PCB
```
- La **librería** tiene su propia tabla de hilos, guardada **en la memoria del proceso**, no en la del kernel.
- También tiene **su propio scheduler**, que decide qué hilo corre.
- Para cambiar de hilo, el hilo llama a una función de la librería (por ejemplo `thread_yield()`, "cedo el turno"). La librería guarda PC, SP y registros del hilo actual en su TCB, elige otro y carga los de ese.
- **Todo pasa en user mode**: es una llamada a función común, sin trap y sin kernel. Por eso es tan rápido.

|✅ Ventajas|❌ Desventajas|
|---|---|
|Cambio de hilo **muy rápido**: no entra al kernel|Si un hilo hace una **syscall bloqueante** (`read()`), el kernel bloquea **al proceso entero**, con todos sus hilos|
|Funciona en un SO **sin soporte de hilos**|**No aprovecha varios núcleos**: el kernel ve un solo "hilo", así que lo pone en un solo núcleo a la vez|
|Cada aplicación puede tener **su propio algoritmo** de planificación|**No hay preemption entre hilos**: el `Timer` le avisa al kernel, no a la librería. Si un hilo nunca cede el turno, los demás no corren|
