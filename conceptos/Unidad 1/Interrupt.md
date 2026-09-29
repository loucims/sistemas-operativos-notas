---
requiere:
  - "[[Dual-mode operation]]"
  - "[[CPU]]"
habilita:
  - "[[Interrupt vector table]]"
  - "[[Timer]]"
---
Al hacer el loop infinito de fetch -> decode -> execute del CPU, trae dos problemas

1. Los dispositivos *terminan su trabajo cuando quieren* (el disco termina de leer, llega un paquete de red, apretas una tecla, etc). Por lo tanto la CPU se tiene que enterar de alguna manera. (Se podrian hacer awaits o whiles con mas ciclos del CPU, eso se llama **polling**, pero es ineficiente)
2. Mientras corre un programa, *el kernel no esta corriendo*. Por lo que hace falta un mecanismo para que el kernel recupere el control. 

Entonces el mecanismo que existe dentro del loop del CPU, es el **interrupt**, el cual es manejado por el **Interrupt Controller**.  Se mandan **IRQ** (Interrupt Request) a este y luego se le comunica al CPU que hay algo que hacer en las interrupciones. (*La interrupcion mas comun es el clock del sistema*)
![[Interrupt 2026-09-23 18.54.45.excalidraw]]
1. El disco termina y activa su linea hacia el controller

2. El controller activa el unico cable INT hacia la CPU, el bit de interrupcion

3. Al terminar su instruccion actual, chequea el cable y ve el 1, y le pregunta al controller quien fue. El controller le pasa un *numero*, el *vector*

4. La CPU usa ese numero como *indice* en la `=[[Interrupt Vector Table]]`, una tabla que armo el Kernel al arrancar, donde cada posicion tiene la direccion de un *handler*
```
   vector 0  → handler de "división por cero"
   ...
   vector 46 → handler del disco
```
5. Salta a ese *handler* en modo kernel. El handler lee los registros del disco para saber el detalle: que sector termino, si hubo error, etc.


(En maquinas modernas con PCIe, muchos dispositivos ya no usan un cable dedicado eso si, mandan un mensaje por el bus con el numero adentro (se llama MSI). Aunque el resultado es el mismo)
*Ejemplo de la ejecucion con instruccion:*
```
programa usuario:  instr 1 → instr 2 → [llega interrupt] ·············· → instr 3 → ...
                                         │                              ↑
hardware:                                ├ guarda el PC (dónde iba)     │
                                         ├ mode bit → kernel            │
                                         └ salta al handler del kernel  │
kernel:                                     handler atiende el disco ───┘ (instrucción de retorno: restaura PC y mode bit)
```

