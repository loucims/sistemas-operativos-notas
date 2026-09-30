---
requiere:
  - "[[Process]]"
  - "[[Fork]]"
  - "[[Process Control Block (PCB)]]"
habilita: []
creado: 2026-09-26
---
Usando `=[[Fork]]` tenes un proceso nuevo, pero *corre el mismo programa* que el padre. Si la shell quiere correr `ls` un clon de la shell no sirve: hace falta que ese proceso pase a correr *otro* programa

`exec()` hace eso: *reemplaza el programa del proceso que lo llama por otro, leido de un archivo ejecutable*. No crea ningun proceso, es el mismo proceso, con el mismo PID, pero con otro programa adentro.

#### Cómo funciona
```c
execlp("/bin/sleep", "sleep", "30", NULL);
//      archivo       argv[0]  argv[1]  fin
```
Lo que hace el kernel (en xv6, `exec()` en `exec.c`):
1. **Abre el ejecutable** y verifica que sea válido. Si no existe o no es ejecutable → devuelve `-1` y **el programa viejo sigue** como si nada.
2. **Arma un address space nuevo**, con una page table nueva:
    - _text_, _data_ y _bss_, cargados desde el archivo;
    - una _stack_ nueva, con los argumentos (`argv`) adentro;
    - el heap arranca vacío.
3. **Pone el PC en el punto de entrada** del programa nuevo (el arranque, que después llama a `main`) y el SP en la stack nueva.
4. **Tira el address space viejo** y lo libera. *Cambia en el PCB la pagetable del address space a la nueva*
5. Vuelve a modo usuario, y el proceso arranca desde el principio del programa nuevo.

| Se **reemplaza**             | Se **mantiene** (sigue en el PCB) |
| ---------------------------- | --------------------------------- |
| Text, data, bss, heap, stack | PID                               |
| PC, SP y registros           | Parent                            |
| Page table                   | Archivos abiertos                 |
|                              | Directorio actual (`cwd`)         |

#### Por qué no retorna si tiene éxito (⭐)
Cuando llamás a una función, en la *stack* queda la **return address**: "volver acá cuando termine". Pero en el paso 4 el kernel **tiró** el text viejo y la stack vieja. Hay tres razones por las que no puede volver:
- La return address apuntaba a código que **ya no existe**.
- La stack donde estaba anotada **ya no existe**.
- No hay a dónde volver: el programa viejo desapareció.

Por eso, **si `exec()` retorna, es porque falló**. En `fig3.8.c`:
```c
execlp("/bin/sleep","ls","30",NULL);
printf("Terminó");     // ← solo se ejecuta si exec FALLÓ
```

#### Con qué se confunde
- **Exec ≠ crear un proceso**: el PID no cambia. Si querés un proceso nuevo _y_ otro programa, necesitás fork + exec.
- **"Cuando el programa nuevo termina, exec retorna"**: no. Cuando el programa nuevo hace `exit`, **termina el proceso hijo**. No hay vuelta al código viejo. Pero, *el padre sigue, y usando `wait` puede esperar y luego resumir* desde que termina el hijo, ademas devuelve el *exit status*.
- **Windows `CreateProcess()`**: hace fork + exec en un solo paso.

#### Variantes de nombre
`exec` es una familia. La syscall real es `execve`, y las otras son funciones de librería que terminan llamándola:

| Letra | Significa                                                                   |
| ----- | --------------------------------------------------------------------------- |
| `l`   | Argumentos en **lista**: `execl(path, "a", "b", NULL)`                      |
| `v`   | Argumentos en **vector** (array): `execv(path, argv)`                       |
| `p`   | Busca el programa en el **PATH** (podés poner `"ls"` en vez de `"/bin/ls"`) |
| `e`   | Le pasás el **environment** a mano                                          |
