---
requiere:
  - "[[Region critica]]"
habilita:
  - "[[Busy waiting]]"
  - "[[Disabling interrupts]]"
  - "[[Sleep and wakeup]]"
creado: 2026-09-27
---
Es el principal objetivo de una solucion de region critica: *garantizar que nunca haya dos hilos adentro de la misma región crítica al mismo tiempo*.
Si se cumple, el `x++` de A y el de B no se pueden mezclar: uno hace los 3 pasos completos (leer, sumar, guardar) y recién después el otro.

Genera **atomicidad** en los reads y writes de la region critica.
```c
while (1) {
    // ── sección de entrada ──   pedir permiso para entrar
    //    REGIÓN CRÍTICA          tocar el dato compartido
    // ── sección de salida ──    avisar que salí
    //    región no crítica       todo lo demás
}
```

Se consigue con alguna de las siguientes *implementaciones* de **mutual exclusion**

| Solución                              | Tipo                             | Mientras espera, el hilo…     | Problema principal                       |
| ------------------------------------- | -------------------------------- | ----------------------------- | ---------------------------------------- |
| `Disabling interrupts` ⭐              | Hardware / kernel                | —                             | No sirve en user mode ni con varias CPUs |
| `Lock variable`                       | Software                         | gira en un loop               | Tiene su propia race condition           |
| `Strict alternation` ⭐                | Software                         | gira en un loop               | Viola progreso (lo que acabás de ver)    |
| `Peterson's solution`                 | Software                         | gira en un loop               | Solo para 2 hilos                        |
| `Test-and-set` ⭐ / `Compare-and-swap` | Instrucción atómica del hardware | gira en un loop               | Gasta CPU esperando                      |
| `Semaphore` ⭐ / `Mutex` ⭐ / `Monitor` | Con ayuda del SO                 | **se bloquea** (no gasta CPU) | Más caros de usar (syscalls)             |
#### Con qué se confunde
- **Mutual exclusion ≠ orden**: garantiza que no se superpongan, no **quién va primero**. Hacer que un hilo espere a que otro termine algo es _sincronización_ (la slide U2_3 la separa: _"además de las carreras, otra cuestión es la sincronización"_). Se ve en `Producer-consumer problem`.
- **Mutual exclusion sola no alcanza**: una solución que no deja entrar a nadie la cumple perfectamente. Siempre se evalúa junto con progreso y espera limitada.