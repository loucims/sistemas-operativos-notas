---
titulo: System Programs
tipo: concepto
unidad: 1 - Introducción
estado: sin-empezar
requiere: ["[[Sistema Operativo]]"]
habilita: []
creado: 2026-09-22
tags: []
---
El Kernel te da mecanismos crudos: las syscalls, como `open` `read` `write` `fork`. Pero con solo un Kernel, la compu es *inusable*. No tenes forma de listar archivos, copiar uno, ver que procesos corren, compilar o loguearte, no tenes ninguna GUI siquiera.

Se podria meter todo ese *adentro* del kernel, pero seria mala idea:
- El kernel seria gigante
- Todo ese codigo correria con *privilegios de hardware*, por lo que un bug en el comando que copia archivos podria tirar abajo toda la maquina

Por lo tanto el sistema operativo viene con programas, los cuales se corren en user mode, sin privilegios, y al instanciarse son un proceso mas. 

Estos son los **System Programs.** Son meramente aplicaciones, sean de CLI, manejo de archivos, comunicaciones, GUI, etc; que viene ya empaquetadas dentro del OS

Algunos ejemplos...

| Categoría                        | Ejemplos (Linux/macOS)                                                                      |
| -------------------------------- | ------------------------------------------------------------------------------------------- |
| Manejo de archivos               | `ls`, `cp`, `mv`, `rm`, `mkdir`                                                             |
| Información de estado            | `ps`, `top`, `df`, Activity Monitor                                                         |
| Edición de archivos              | `nano`, `vim`                                                                               |
| Soporte de lenguajes             | `gcc`, linker, debugger                                                                     |
| Carga y ejecución                | loader (slides 36–37)                                                                       |
| Comunicaciones                   | `ssh`, `ping`                                                                               |
| **Daemons** (servicios de fondo) | `sshd`, `cron`, el servidor de impresión. Arrancan después del kernel en el boot (slide 11) |
| Interfaz de usuario              | **shell** (CLI), el entorno gráfico (GUI)                                                   |
