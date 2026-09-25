---
requiere:
  - "[[RAM]]"
habilita:
  - "[[Kernel]]"
creado: 2026-09-24
---
Cuando prendés la máquina, **la RAM está vacía**: pierde todo al apagarse. El kernel está guardado en el disco, pero para leer el disco hace falta un programa que ya esté corriendo. Es el problema del huevo y la gallina: para cargar el primer programa necesitás un programa.

El nombre viene de "levantarse tirando de los cordones de tus propias botas" (_bootstraps_): arrancar desde nada. De ahí sale "bootear".

#### Cómo funciona, paso a paso

1. **Encendés la máquina y la CPU arranca en una dirección fija.**
    - **Qué hace:** la CPU empieza a ejecutar instrucciones en una dirección de memoria que viene grabada de fábrica.
    - **Por qué:** la CPU siempre necesita saber dónde empezar, y la RAM está vacía. Esa dirección apunta a un chip de la placa madre que **no se borra al apagar** (ROM/flash). Ahí vive el **firmware**: la BIOS en máquinas viejas, UEFI en las modernas.
2. **El firmware revisa el hardware.**
    - **Qué hace:** chequea que la RAM, el teclado y los discos respondan.
    - **Por qué:** no tiene sentido arrancar un sistema si la memoria está rota.
3. **El firmware busca de dónde bootear y carga un programa chico: el bootloader.**
    - **Qué hace:** según el orden configurado (disco, USB…), lee un programa cargador y lo pone en RAM. Por ejemplo, GRUB en Linux o Windows Boot Manager.
    - **Por qué:** el firmware es chico y genérico, y no sabe nada de cada sistema operativo. Le delega el trabajo a un programa que sí sabe.
4. **El bootloader carga el kernel en RAM y salta a él.**
    - **Qué hace:** encuentra el archivo del kernel en el disco, lo copia a RAM y le pasa la CPU.
    - **Por qué:** recién ahora el kernel está en memoria y puede ejecutarse. La CPU arranca en el modo **más privilegiado**, así que el kernel empieza en kernel mode.
5. **El kernel se inicializa.**
    - **Qué hace:** prepara todo lo que vimos hoy:
        - arma la **Interrupt vector table** y carga su dirección en el registro especial,
        - programa el **Timer**,
        - organiza la memoria e inicializa los dispositivos,
        - monta el disco (la diapositiva de xv6 dice: "una vez que arranca el kernel, este monta el disco").
    - **Por qué:** todo esto tiene que estar listo **antes** de que corra cualquier programa de usuario.
    - En el xv6 de la cátedra lo ves en `main.c` (líneas 1224–1237 del booklet), en orden: `kinit`, `kvminit`, `procinit`, `trapinit`, `plicinit`, `binit`, `fileinit`, `virtio_disk_init`, `userinit`.
6. **El kernel arranca el primer proceso de usuario.**
    - **Qué hace:** crea el proceso inicial (`init`, o `systemd` en Linux actual; `userinit` en xv6). Ese proceso arranca los **daemons** (servicios del sistema que corren fuera del kernel) y después el login o el Shell.
    - **Por qué:** a partir de acá el kernel ya no "corre solo". Como dice la diapositiva, **el kernel es guiado por interrupciones**: solo vuelve a ejecutarse cuando hay un interrupt, un trap o una syscall.

#### Resumen: quién carga a quién

|Etapa|Dónde está guardado|Qué carga|
|---|---|---|
|Firmware (BIOS/UEFI)|ROM/flash de la placa madre|el bootloader|
|Bootloader (GRUB…)|disco|el kernel|
|Kernel|disco, y después RAM|el primer proceso (`init`)|
|`init` / `systemd`|disco|daemons, login, shell|

#### Con qué se confunde

- **Bootstrap vs. BIOS:** la BIOS (o UEFI) es el firmware, la primera pieza. El bootstrap es **todo el proceso** de arranque, o el código simple que lo empieza.
- **Bootstrap vs. Loader:** el bootstrap carga el **kernel** al prender la máquina. El loader carga **programas** cuando el sistema ya está andando.