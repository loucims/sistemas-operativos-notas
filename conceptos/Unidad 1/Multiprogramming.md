---
requiere:
  - "[[Dual-mode operation]]"
  - "[[Kernel]]"
habilita: []
creado: 2026-09-25
---
Los dispositivos son **lentísimos** comparados con la CPU. Órdenes de magnitud aproximados:

| Operación               | Tiempo aprox.      | Instrucciones que la CPU podría ejecutar mientras tanto |
| ----------------------- | ------------------ | ------------------------------------------------------- |
| una instrucción de CPU  | ~1 nanosegundo     | 1                                                       |
| leer de un SSD          | ~100 microsegundos | ~100.000                                                |
| leer de un disco rígido | ~10 milisegundos   | ~10.000.000                                             |

Con **un solo programa** en memoria, cada vez que hace un `read()` y tiene que esperar al disco, **la CPU se queda sin nada que hacer**.

Entonces se implemente la **Multiprogramacion**. Se *mantienen varios programas cargados en memoria a la vez*, asi que cuando uno espera, hay otro listo para usar la CPU.

Mientras un proceso esta haciendo una syscall como `read()`, ya que el disco va a tardar, la kernel *le da la CPU a otro proceso para ejecutar*. Una vez que el disco termina y avisa con un `=[[Interrupt]]`, el handler del disco marca el proceso como *listo para ejecutar* otra vez. Y en algun momento el scheduler termina reanudando la ejecucion del proceso.

#### Cuánto mejora: la fórmula de la cátedra
Si cada proceso pasa una fracción **p** de su tiempo esperando E/S, y hay **n** procesos en memoria, la CPU está ociosa solo cuando **todos** esperan a la vez. La probabilidad de eso es **pⁿ**. Entonces:

**Uso de la CPU total = 1 − pⁿ**

| n (procesos en memoria) | pⁿ (todos esperando) | Uso de CPU |
| ----------------------- | -------------------- | ---------- |
| 1                       | 0,8                  | **20%**    |
| 2                       | 0,64                 | 36%        |
| 4                       | 0,41                 | 59%        |
| 10                      | 0,11                 | **89%**    |
Por lo tanto la *multiprogramacion permite aumentar el uso de la CPU*.

#### Con qué se confunde
**Multiprogramming vs. Multitasking (el concepto que sigue).**

|                              | Multiprogramming                                                                          | Multitasking                                                           |
| ---------------------------- | ----------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- |
| Objetivo                     | que la CPU *no este ociosa* (osea que no este esperando sin nada que hacer, maximiza uso) | que el *usuario* sienta que todo corre a la vez                        |
| Cuando se cambia de programa | cuando el que corre *tiene que esperar*                                                   | *muy seguido*, aunque nadie este esperando (lo fuerza el `=[[Timer]]`) |

#### Ejemplo
Paso a paso, con tres programas A, B y C en memoria:
**1. A usa la CPU**
- **Qué hace:** A ejecuta sus instrucciones normalmente.
**2. A hace `read()`**
- **Qué hace:** entra al kernel con una syscall, como viste recién. El kernel le pide los datos al disco.
- **Por qué:** el disco va a tardar. A no puede seguir sin esos datos, así que **queda esperando**.
**3. El kernel le da la CPU a B**
- **Qué hace:** en vez de volver a A, el kernel elige otro programa **listo** para correr, B, y le pasa la CPU.
- **Por qué:** es el punto central. La espera de A se aprovecha para trabajo de B.
**4. El disco termina y avisa con un Interrupt**
- **Qué hace:** entra el handler del disco, que copia los datos al buffer de A y marca a A como **listo** otra vez.
- **Por qué:** así el kernel se entera de que A puede seguir. Es el problema 1 de tu nota de Interrupt: el dispositivo termina cuando quiere.
**5. En algún momento A vuelve a usar la CPU**
- **Qué hace:** cuando B espera algo, o cuando el kernel lo decida, A retoma justo después de su `read()`.