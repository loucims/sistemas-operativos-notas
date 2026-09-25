---
titulo: Sistema Operativo
tipo: concepto
unidad: 1 - Introducción
estado: sin-empezar
requiere:
habilita:
  - "[[Kernel]]"
  - "[[System Programs]]"
creado: 2026-09-21
tags: []
---
Sin un OS, un programa tendria que:
- Hablarle al disco con los comandos especificos para el especifico modelo de controlador
- Autogestionar su uso de la memoria y en que parte de la RAM vive
- y confiar en que ningun otro programa le pise la memoria ni se quede con la CPU para siempre

Es una capa de **abstraccion** la cual actua de intermediario entre el hardware y las aplicaciones
sus dos roles son el de

**Extended Machine:** Esconde el hardware detras de abstracciones limpias, por ejemplo un file en vez de sectores de disco, un process en vez de registros sueltos. Gracias a esto los sistemas son **portables**, un IDE o Photoshop no necesita saber que disco usas.
Esta interfaz es la que sirven las llamadas System Calls y deben ser pocas pero muy combinables.

**Resource Manager:** Reparte computo en la `=[[CPU]]`, memoria y dispositivos entre los programas. Decide quien corre y cuando (un policia de trafico) y protege a los programas de los errores de otros programas. (Y de su malicia tambien.)
#### OS != `=[[Kernel]]`
El sistema operativo esta usualmente compuesto de el `=[[Kernel]]` + `=[[System Programs]]` + a veces middleware.

Volver a `=[[Unidad 1 - Introducción]]`
