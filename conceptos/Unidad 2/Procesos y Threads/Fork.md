---
requiere:
  - "[[Process]]"
  - "[[Process Control Block (PCB)]]"
habilita:
  - "[[Exec]]"
creado: 2026-09-26
---
Todo proceso tiene que nacer de algun lado. En Unix, *la unica forma de crear un proceso nuevo es que otro proceso lo cree*. El primero (`init`, hoy `systemd`) lo arma el kernel al arrancar, y todos los demas descienden de el.

`fork()` es la forma de *clonar al proceso que la llama*. De esta manera no hace falta pasarle mil parametros, el hijo *hereda todo* (archivos abiertos, directorio,  variables). 
Y si despues queres que corra otro programa, lo cambias con `=[[Exec]]`.

#### Cómo funciona: lo que ve el programador
Una sola llamada, **dos retornos**, uno en cada proceso:

|               | **Que devuelve `fork()`** |
| ------------- | ------------------------- |
| En el *padre* | El PID del hijo (>0)      |
| En el *hijo*  | 0                         |
| Si falla      | -1                        |
Y por heredar, lo principal que hereda cada proceso son los archivos abiertos, el directorio y mas importante, el **PC y los registros del CPU** exceptuando el de resultado (para el PID). 
```c
pid = fork();
if (pid == -1)       { /* error */ }
else if (pid == 0) { /* soy el HIJO  */ }
else               { /* soy el PADRE, pid = PID del hijo */ }
```
Por eso *el codigo resume en el nuevo proceso hijo desde la misma instruccion que el padre*.

(Aunque no sabes *quien va a correr primero*, si el padre o el hijo. Lo
decide el `=[[Scheduler]]`)

#### Cómo funciona: lo que hace el kernel
Es lo que pide la pregunta del parcial. En xv6 (`fork()` en `proc.c`, del booklet):
1. **Busca un slot `UNUSED`** en la process table (`allocproc`). Le asigna un **PID nuevo** y le reserva su kernel stack. Estado: `USED` (new).
2. **Copia la memoria** del padre a memoria nueva, con su **page table propia** (`uvmcopy`). Mismo contenido, distinto lugar físico.
3. **Copia los registros** del padre (el trapframe). Por eso el hijo arranca en el mismo punto.
4. **Cambia un solo registro en el hijo**: el registro de resultado queda en `0`. Por eso `fork()` le devuelve 0.
5. **Hereda archivos abiertos y directorio** (`filedup`, `idup`).
6. Anota `parent = padre`.
7. Pone al hijo en **`RUNNABLE`** (ready).
8. Al padre le **devuelve el PID del hijo**.

#### Con qué se confunde
- **Fork ≠ Exec**: fork **crea** un proceso nuevo con el **mismo** programa. Exec **no crea** nada: cambia el programa del proceso actual.
- **"El hijo arranca desde `main`"**: no. Arranca justo después del `fork()`, con todo lo que el padre ya había hecho.
- **"Padre e hijo comparten las variables"**: no. Tienen **copias**. Lo que cambia uno, el otro no lo ve. Lo que sí comparten son los archivos abiertos.
- **Windows no tiene fork**: usa `CreateProcess()`, que crea el proceso y carga el programa en un solo paso (tu `main.c` de U1).