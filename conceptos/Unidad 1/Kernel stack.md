---
requiere:
  - "[[Kernel]]"
  - "[[Stack]]"
habilita: []
creado: 2026-09-24
---
Si llega un interrupt o una syscall y el kernel empieza a ejecutar su handler. Ese handler tambien es codigo: llama a funciones y usa variables locales, asi que necesita *un stack.* 

La opcion comoda seria seguir usando la stack del proceso interrumpido, que ya esta ahi y el stack pointer ya apunta a ella.
Pero esto viene con varios *problemas*

1. El *Stack Pointer no es confiable*. El sp lo controla el programa. Podria apuntar a basura, a memoria que no existe o, peor, a *memoria del kernel*. El kernel, que en kernel mode puede escribir en cualquier lado, apilaria sus datos ahi: generando *un crash del kernel* muy probablemente.
2. La *fuga de informacion*. Al volver a user mode, lo que el kernel dejo en la stack (variables, punteros, datos de otros procesos) seguiria en la memoria del programa. Por lo que el programa podria leerla
3. Hay veces que *no tiene sentido*. Si llega un interrupt del disco por la lectura de *otro* proceso, si reutilizara el stack del proceso actual, *mezclarias el stack* de los procesos, no tiene sentido.

### Por lo tanto, que se hace?

Cada proceso tiene su **propia kernel stack**, *guardada en la memoria del kernel* (importante, no esta en el espacio de memoria del proceso), donde el usuario no puede llegar.

- Al entrar al kernel, el *hardware cambia el sp* para que apunta a la kernel stack *antes* de apilar nada (antes incluso de guardar el PC del programa interrumpido).
- Al volver al user mode, se *restaura el sp original* y el programa sigue con su stack intacta