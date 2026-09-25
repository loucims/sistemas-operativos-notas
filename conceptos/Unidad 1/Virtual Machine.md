---
requiere:
  - "[[Sistema Operativo]]"
habilita: []
creado: 2026-09-25
---
- **Qué problema resuelve:** correr **varios sistemas operativos a la vez** en una sola máquina física, aislados entre sí. Sirve para servidores en la nube, para probar un SO sin instalarlo, o para correr el xv6 de la cátedra adentro de qemu.
- **Cómo funciona:** un programa llamado **hypervisor** le presenta a cada SO "invitado" (guest) una **máquina falsa**: CPU, memoria y discos virtuales.
    - Truco clave, que se apoya en Dual-mode operation: el kernel invitado **cree** que está en kernel mode, pero en realidad corre en **user mode**.
    - Cuando el kernel invitado ejecuta una instrucción privilegiada, eso produce un **trap**. El hypervisor lo atrapa y **simula** el efecto sobre la máquina falsa. Esto se llama **trap and emulate**.
- **Con qué se confunde:**
    - **Virtualización vs. emulación:** virtualizar corre las instrucciones directo en la CPU real (misma arquitectura, rápido). Emular traduce **cada** instrucción de una arquitectura distinta (lento). qemu corriendo el xv6 en tu Mac está emulando.
    - **VM vs. "el SO como máquina virtual":** el programa de la materia dice que el SO _es_ una máquina virtual, en el sentido de la **extended machine** de tu nota de Sistema Operativo: una máquina más fácil de usar que el hardware real. Es otro significado.