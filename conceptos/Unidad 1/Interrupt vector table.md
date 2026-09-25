---
requiere:
  - "[[Interrupt]]"
  - "[[Kernel]]"
habilita: []
creado: 2026-09-24
---
Cuando llega un interrupt o un trap, la CPU tiene que saltar a *algun* handler del `=[[Kernel]]`. Pero como sabe a cual?

- O el que dispara el evento dice la *direccion* del handler. ("salta a 0xFFFFF80000_1234)
- O el que dispara el evento dice un *numero* ("soy el evento 14") y alguien mas traduce ese numero a una direccion.

 Todos los sistemas usan la ultima (Por que sino, el programa pudiera salta a *cualquier codigo o instruccion*). Usando un numero especifico, el programa solo elije cual de las funciones del Kernel usar. La **Interrupt Vector Table** es justamente esa tabla que hace la traduccion y el menu.
![[Interrupt vector table 2026-09-24 18.35.14.excalidraw|900]]
1. Al arrancar (en el `=[[Bootstrap]]`, el kernel arma la tabla en su propia memoria y carga su direccion en ese *registro especial*. La instruccion que hace esa carga es privilegiada.)
2. Cuando pasa algo, el hardware solo aporta el *numero*.
3. La CPU hace `tabla[numero]` y salta, en un solo paso.

### Interrupt vector table != system call table
Son dos niveles distintos, el trap de una syscall entra por *una entrada dentro de la interrupt vector table*, el handler de syscalls. Adentro, ese handler lee el *numero de la syscall* y lo busca en *otra* tabla, la de las syscalls: `read` `write` `fork`, etc....

### Vector vs handler
El vector es el *numero (o indice)*. El *Handler* es el codigo al que apunta la entrada de la tabla.