Hay dos situaciones donde el que necesita al kernel es el propio programa que esta corriendo:

1. **Lo necesita a proposito**. Quiere leer un archivo, pero en user mode no puede tocar el disco (`=[[Dual-mode operation]]`). Tiene que haber una manera de delegar a que el kernel lo haga.
2. **Hizo algo que no puede resolver**. Dividio por cero, intento una instruccion privilegiada o toco memoria que no es suya. La CPU no puede seguir con esa instruccion en modo usuario, y alguien con autoridad (el kernel) tiene que decidir que hacer

Por lo tanto esta el **Trap**.

Un **trap** es una transferencia de control al kernel **provocada por la instrucción que se está ejecutando**. También se lo llama _interrupción de software_ o _excepción_.

| Tipo             | ¿Intencional?                                                        | Ejemplos                                                                         | Después del handler, el kernel…                        |
| ---------------- | -------------------------------------------------------------------- | -------------------------------------------------------------------------------- | ------------------------------------------------------ |
| `=[[System Calls]]` | Sí: el programa ejecuta una instrucción trap (`syscall`, `int 0x80`) | `read`, `write`, `fork`                                                          | Vuelve a la **instrucción siguiente** con el resultado |
| `=[[Exception]]`    | No: la CPU detecta un error al ejecutar                              | División por cero, instrucción privilegiada en user mode, acceso a memoria ajena | Casi siempre **mata al proceso**                       |

#### Cómo funciona

Usa **el mismo mecanismo que un interrupt**. Lo único que cambia es quién lo dispara:
Instruccion actual -> La CPU detecta "trap" entonces:
- Se guarda el PC (donde estaba el programa)
- mode bit -> kernel
- numero del trap -> *vector table* -> direccion del handler
- salta al handler del kernel
- handler hace lo que corresponde
- Instruccion de retorno: restaura el PC y mode bit -> user (o no vuelve nunca si mato al proceso)

