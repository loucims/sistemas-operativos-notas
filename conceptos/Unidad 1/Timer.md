---
requiere:
  - "[[CPU]]"
  - "[[Interrupt]]"
habilita:
  - "[[Dual-mode operation]]"
  - "[[Kernel]]"
  - "[[Multitasking (time sharing)]]"
creado: 2026-09-25
---
El kernel *no esta corriendo todo el tiempo en la maquina*. (Incluso cuando el scheduler no encuentra nada que hacer, el kernel idle loop solo espera una interrupcion)

El kernel solo recupera la CPU en dos casos
- el programa hace una *syscall* o genera una *exception* (osea un trap de software) 
- llega un *interrupt* de algun dispositivo

Entonces si hubiera un programa que *no pida nada al kernel*, y *no provoque ningun interrupt* como un
```c
while(1) {} // loop infinito: 
// no hace syscalls, no toca ningun dispositivo
```
 La CPU *quedaria en sus manos para siempre*, el kernel nunca volveria a correr, ningun otro programa volveria a correr y la maquina quedaria colgada.

Para esto, esta el mecanismo de **Timer**, el que garantiza que el kernel recupere la CPU cada cierto tiempo.

#### Como funciona

El **Timer** es un *dispositivo de hardware* con un contador adentro.

1. *El Kernel carga el contador*, antes de darle la CPU al programa.
	- Pone por ejemplo 10 milisegundos en el contador del timer, y es como un despertador para asegurarse que alguien lo despierte.
	- Esta instruccion es *privilegiada*.
	
2. El kernel le *pasa la CPU al programa* (en user mode)

3. El reloj fisico (el Clock) va bajando el contador en cada tic de la maquina. (*Pasa en hardware, en parelelo*, el programa no participa y no se puede enterar.)

4. El contador llega a 0 y *el timer genera un interrupt.*
	- Manda un interrupt como cualquier otro dispositivo, con su numero de vector.

5. La CPU entra al kernel. Primero termina la instruccion actual, guarda donde iba el programa, pasa a kernel mode, busca el handler del timer en la `=[[Interrupt vector table]]` y salta ahi, usando la `=[[Kernel stack]]`

6. *El handler del kernel para el timer decide que hacer.*
	- Dependiendo del caso:
		- deja que el mismo programa siga
		- le da la CPU a *otro* programa (`=[[Cambio de contexto]]`)
		- *termina* el programa si se paso del tiempo que tenia asignado

7. El kernel vuelve a cargar el timer y suelta la CPU.

#### Con qué se confunde
- **Timer vs. reloj físico (clock):** el reloj físico es el que hace tic, siempre y a ritmo fijo. El timer es el contador que el kernel carga y que _usa_ esos tics para bajar.
- **Timer vs. reloj de tiempo real (la hora del día):** son cosas distintas. Esto aparece en una pregunta del parcial: _leer_ la hora no hace falta que sea privilegiado; _cambiarla_ sí.