---
requiere:
  - "[[Kernel]]"
  - "[[Trap]]"
habilita:
  - "[[Dual-mode operation]]"
creado: 2026-09-25
---
Un programa en user mode *no puede* tocar el disco, la red ni la pantalla. Pero los programas necesitan leer archivos, mandar datos, crear procesos...

Por lo tanto la `=[[Kernel]]` ofrece una *lista oficia de servicios* (`read`,`write`,`fork`,`exit`....) y una *forma segura de pedirlos*. Eso es una **system call**: el programa *le pide* al kernel que haga algo que el no tiene permiso de hacer.

Casi nunca se hacen las syscalls "a mano". Hay una funcion intermedia de libreria siempre.
```c
count = read(fd, buffer, 100); //codigo en c
// llamada de biblioteca (libc) funcion read()
// es un wrapper, arma el pedido y hace el trap
```
A la funcion de la biblioteca se la llama *API*. (En Linux y macOS, POSIX; en Windows, Win32). Esconde los detalles: numeros, registros, la instruccion de trap.

#### Cómo se pasan los parámetros (según la diapositiva)

| Método                | Cómo                                                                         | Ventaja / límite                                                 |
| --------------------- | ---------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| **Registros**         | cada parámetro en un registro del CPU                                        | el más simple y rápido/ puede haber más parámetros que registros |
| **Bloque en memoria** | se arma una tabla con los parámetros y se pasa _su dirección_ en un registro | no limita la cantidad de parámetros                              |
| **Stack**             | el programa los apila y el kernel los lee de ahí                             | tampoco limita la cantidad                                       |


#### Con qué se confunde
**System call vs. función de la API.** `read()` de la biblioteca es una función normal en user mode. La system call es el **trap que hace adentro**. No toda función de la API hace una syscall: `strlen()` no hace ninguna. Y algunas hacen varias, o ninguna hasta más tarde: `printf()` junta texto y en algún momento llama a `write`.

**System call vs. llamado a subrutina (`call`).** Esta es pregunta de parcial:

|                          | `call` a una funcion                                                | System call (trap)                                                       |
| ------------------------ | ------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| A donde salta            | a cualquier direccion que diga el programa                          | a una entrada fija que decidio el kernel (*por numero*)                  |
| Modo                     | sigue en user mode                                                  | pasa a *kernel mode*                                                     |
| Stack                    | la misma stack del programa                                         | cambia a la *Kernel stack*                                               |
| Quien ejecuta lo llamado | codigo del mismo programa                                           | codigo del kernel                                                        |
| Datos                    | la funcion confia en lo que recibe (se pasan por registros del CPU) | el kernel *verifica* todo lo que recibe (se pasan por registros del CPU) |

### Ejemplo 
#### Paso a paso: `count = read(fd, buffer, 100);`

**1. Tu código llama a `read()` de la biblioteca**
- **Qué hace:** un call común: nota de retorno en la stack y salto a la función.
- **Por qué:** es código normal en user mode. Todavía no se pidió nada al kernel.

**2. La biblioteca arma el pedido en registros**
- **Qué hace:** pone el **número de syscall** de `read` en un registro (en Linux x86-64, `read` es el número 0) y los parámetros en otros registros: `fd`, la dirección de `buffer` y `100`.
- **Por qué:** el kernel necesita saber _qué servicio_ pedís y _con qué datos_. Los registros sirven porque siguen ahí después del salto al kernel, y el kernel los puede leer.

**3. La biblioteca ejecuta la instrucción de trap**
- **Qué hace:** en x86-64 se llama `syscall`. El hardware guarda dónde iba el programa, pasa el mode bit a kernel, cambia el sp a la **Kernel stack** y salta al handler de syscalls, en una dirección que decidió el kernel.
- **Por qué:** es la única puerta de entrada al kernel, y el programa no elige a dónde salta. Es lo mismo que viste en Trap y en Interrupt vector table.

**4. El kernel busca qué servicio le pidieron**
- **Qué hace:** lee el número del registro (0) y lo busca en la **tabla de syscalls**: `tabla[0] → sys_read`.
- **Por qué:** es la tabla de segundo nivel de tu nota de Interrupt vector table. Se usa un número y no una dirección por la misma razón de seguridad de siempre.

**5. `sys_read` verifica antes de hacer nada**
- **Qué hace:** chequea que `fd` sea un archivo que **este** proceso tiene abierto y que `buffer` apunte a memoria **del programa**.
- **Por qué:** el kernel no confía en nada de lo que viene del usuario. Es la misma lección de Kernel stack.

**6. El kernel hace el trabajo**
- **Qué hace:** lee los datos del disco y los **copia** al `buffer`, en la memoria del programa.
- **Por qué:** es lo que el programa no podía hacer solo. Si el disco tarda, el proceso queda esperando; eso lo ves en la Unidad 2.

**7. El kernel deja el resultado en el registro de resultado**
- **Qué hace:** pone la cantidad de bytes leídos, `0` si llegó al final del archivo o `-1` si hubo error.
- **Por qué:** es la misma idea de 📦 en `=[[Stack]]`. El resultado viaja por un registro del CPU.

**8. Vuelta a user mode**
- **Qué hace:** una instrucción de retorno restaura el PC, pone el sp de nuevo en la stack del programa y pasa el mode bit a user. El programa sigue en la instrucción siguiente al trap, adentro de la biblioteca.
- **Por qué:** a diferencia de una Exception, acá el kernel **vuelve** al programa, y vuelve a la instrucción siguiente.

**9. La biblioteca retorna a tu código**
- **Qué hace:** un `ret` común. Tu código copia el resultado en `count`.

```
user mode   tu código ──call──► libc read() ──arma registros CPU──► TRAP
                                                                  │
kernel mode                    tabla[0] → sys_read → verifica → lee disco → copia al buffer
                                                                  │
user mode   count = resultado ◄──ret── libc read() ◄──retorno─────┘
```
