---
titulo: Kernel
tipo: concepto
unidad: 1 - Introducción
estado: sin-empezar
requiere: ["[[Sistema Operativo]]", "[[CPU]]"]
habilita: ["[[System Calls]]"]
creado: 2026-09-22
tags: []
---

El [[Sistema Operativo]] tiene que hacer cumplir ciertas reglas,
que un proceso no lea la memoria de otro, que nadie se quede con la CPU.

Por lo tanto, es necesario una pieza de software con **privilegios que los demas no tienen**, que este **siempre cargada** y que sea **la unica que toca el hardware directamente**. Esa pieza es el kernel.

Como funciona:
- Se carga en el **[[Bootstrap]]** y queda como **residente en memoria** hasta que se apague la maquina (eso es parte del rato que tarda en bootear la compu)
- Corre con **privilegios de hardware** que la CPU le da, los procesos corren sin esos privilegios
- Los **procesos** tienen su memoria (instructions, data, stack). Hay muchos procesos pero un solo kernel.
- Y es **Event-Driven**, el kernel no esta siendo ejecutado 24/7 en el CPU en un loop (confirmar esto!!). El kernel solo entra cuando pasa algo. (Por ejemplo un syscall)

##### Kernel != [[Root]]
Root es un ***usuario*** con mas permisos *dentro de las reglas del kernel*. Un programa corriendo como root sigue sin privilegios de hardware.

##### Kernel != Proceso
El kernel no es un proceso mas en la lista. Es el codigo que crea y administra los procesos, y que corre en **nombre de ellos** cuando hacen una **syscall**