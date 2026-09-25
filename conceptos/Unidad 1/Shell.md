---
requiere:
  - "[[Kernel]]"
habilita: []
creado: 2026-09-25
---
- **Qué problema resuelve:** el usuario necesita una forma de pedirle cosas al sistema ("corré este programa", "listá esta carpeta") sin escribir código.

- **Qué es:** un **programa común** que lee comandos del usuario y los ejecuta. Corre en user mode, como cualquier otro, y usa System Calls para todo. La diapositiva aclara que a veces está implementado en el kernel, pero en general es un System Program. Puede haber varios instalados (bash, zsh…).

- **Cómo ejecuta un comando:**
    - Algunos comandos son **internos**: el shell mismo los resuelve (por ejemplo, `cd`).
    - La mayoría son **solo nombres de programas**: `ls` es un ejecutable aparte. Por eso agregar comandos nuevos no requiere modificar el shell.
    - En Unix, para correr un programa el shell hace `fork()` (se clona), el hijo hace `exec()` (se reemplaza por el programa) y el padre **espera** a que termine. Eso lo ves en detalle en la Unidad 2.

**Con qué se confunde:**
- **Shell vs. kernel:** el shell no tiene privilegios. Es solo la "cara" que ve el usuario.
- **Shell vs. terminal:** la terminal es la ventana; el shell es el programa que corre adentro.