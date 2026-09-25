---
requiere:
  - "[[RAM]]"
habilita: []
creado: 2026-09-25
---
- ***Qué problema resuelve:*** el código que escribís no se puede ejecutar directamente. Además está repartido en varios archivos y usa bibliotecas, y hay que llevarlo del disco a la RAM.
- ***Cómo funciona, el recorrido:***

```
main.c ──compiler──► main.o ──linker──► main (ejecutable) ──loader──► programa en RAM
 (fuente)  gcc -c     (objeto    + otros .o  (en disco)        ./main    (listo para correr)
                      reubicable)  + bibliotecas
```

1. **Compiler:** traduce cada `.c` a un **archivo objeto** (`.o`). Es _reubicable_: todavía no sabe en qué dirección de memoria va a quedar.
2. **Linker:** junta todos los `.o` y las bibliotecas en **un solo ejecutable**, y resuelve las referencias entre archivos ("`main` llama a `doble`, que está en otro `.o`").
3. **Loader:** cuando corrés el programa, lo **trae del disco a la RAM**. Le asigna las direcciones finales (**relocation**) y lo arranca.

- **Bibliotecas dinámicas:** los sistemas modernos no meten las bibliotecas adentro del ejecutable. Las cargan cuando hacen falta y **las comparten** entre todos los programas que las usan: se cargan una sola vez. Ejemplos: `libc.so.6` en Linux, las DLL en Windows.
- **Con qué se confunde:**
    - **Linker vs. compiler:** el compiler traduce un archivo; el linker une varios.
    - **Loader vs. Bootstrap:** el Bootstrap carga el **kernel** cuando arranca la máquina; el loader carga **programas** cuando el sistema ya está andando.