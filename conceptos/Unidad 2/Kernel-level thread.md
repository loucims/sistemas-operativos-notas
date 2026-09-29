---
requiere:
  - "[[Process Control Block (PCB)]]"
  - "[[Thread]]"
habilita: []
creado: 2026-09-27
---
**Kernel-level threads** (lo que usan Linux, Windows y macOS hoy)
- El kernel conoce cada hilo. El PCB se parte en dos:
```
 PCB del proceso (lo compartido)
 ├── address space / page table
 ├── archivos abiertos
 ├── PID, parent...
 │
 ├── TCB hilo 1: PC, SP, registros, estado = RUNNING
 ├── TCB hilo 2: PC, SP, registros, estado = BLOCKED  (esperando el disco)
 └── TCB hilo 3: PC, SP, registros, estado = READY
```
- **Crear un hilo es una syscall**: el kernel arma su TCB (_Thread Control Block_). `pthread_create` en Linux termina llamando a la syscall `clone`.
- El scheduler del kernel **planifica hilos, no procesos** (slide U2_5: "se planifican de la misma forma que los procesos").
- Cada TCB tiene su propio estado. Si A2 se bloquea, A1 y A3 siguen.
- El timer interrumpe **hilos**, así que hay preemption entre hilos del mismo proceso.
- Cambiar de hilo **pasa por el kernel** (trap o interrupción), así que es más caro que en user-level. Pero si los dos hilos son del mismo proceso, igual es más barato que cambiar de proceso, porque no se toca la page table.
- (En Linux, cada hilo es literalmente una entrada más en la process table, que comparte la page table con los otros hilos.)

#### User-level vs kernel-level

| User-level                     | Kernel-level                   |                           |
| ------------------------------ | ------------------------------ | ------------------------- |
| ¿Quién los conoce y planifica? | La librería                    | El kernel                 |
| ¿Dónde están los TCB?          | Memoria del proceso            | Memoria del kernel        |
| Cambio de hilo                 | Llamada a función (muy rápido) | Trap + kernel (más caro)  |
| Syscall bloqueante             | Bloquea **todo** el proceso    | Bloquea **solo ese hilo** |
| Varios núcleos                 | No                             | Sí                        |
| Preemption entre hilos         | No (cooperativo)               | Sí (timer)                |
