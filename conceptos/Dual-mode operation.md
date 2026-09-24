---
habilita:
  - "[[Interrupt]]"
  - "[[Trap]]"
  - "[[Exception]]"
requiere:
  - "[[Kernel]]"
---
Como el kernel y los programas corren *sobre la misma CPU*, ejecutan instrucciones del mismo repertorio. 
Entonces, para impedir que un programa ejecute instrucciones las cuales harian bypass de toda la seguridad del *OS*, existe la proteccion del **bit de modo del CPU,** implementada en *hardware* 

Bajo este tipo de arquitectura:
- La CPU tiene un **bit de modo**, la cual dice si en este momento esta corriendo codigo de usuario o codigo de Kernel
- Algunas instrucciones estan marcadas como **privilegiadas**, cambiar la page table, apagar interrupts, hacer I/O directo, etc. La CPU las ejecuta *solo si el mode bit dice kernel*. Si un programa en user mode intenta una, la CPU no la ejecuta y dispara una **exception**, osea un trap, y entra el kernel (que normalmente mata al proceso).
- Y con la memoria lo mismo, en user mode, la CPU solo deja acceder a las direcciones que el kernel le *asigno* a ese proceso.

*Como se cambia el bit de modo?*

user -> kernel
A travez de un `=[[Trap]]` (el cual es triggereado junto un syscall, o un exception). El hardware pone el bit de modo en kernel y al mismo tiempo *pone el PC en la direccion que el kernel dejo en la vector table*.

kernel -> user
Usa una instruccion de retorno, que restaura el PC y el modo guardados

Algunas equivocaciones, mode bit != usuario root
y mode switch != context switch