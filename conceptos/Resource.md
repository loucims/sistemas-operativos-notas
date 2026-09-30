---
requiere:
  - "[[Mutual exclusion]]"
habilita:
  - "[[Resource acquisition]]"
creado: 2026-09-30
---
#### Qué problema resuelve
Para definir un deadlock tenés que poder decir "A está esperando **algo** que tiene B". Ese "algo" es el **resource**. Es la pieza base de toda la unidad: sin resource no hay nada por qué pelearse.

#### Qué es?

> _"Cualquier cosa que en un instante determinado de tiempo sólo puede ser utilizada por un solo hilo/proceso."_

La slide da estos ejemplos: dispositivos físicos (impresora, escáner), registros de una base de datos, archivos en disco y memoria principal. Ojo que **no es solo hardware**: un registro de una tabla también es un resource.

Se usa siempre igual (p.26), y esto adelanta el ⭐2:
```
request  → si no está libre, espero (busy waiting o me bloqueo)
use
release
```

Con *semáforos* eso es `down` → usar → `up`. Ya lo conocés.

#### Preemptable vs nonpreemptable
Las slides no desarrollan esta parte, pero **es pregunta del parcial**. Viene de Tanenbaum (_Modern Operating Systems_, cap. 6.1.1).

La pregunta para clasificar un resource es: **¿se lo puedo sacar al proceso a mitad de uso sin romper nada?**

|         | **Preemptable**                                                                                                                        | **Nonpreemptable**                                                                                                                      |
| ------- | -------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- |
| Qué es  | Se le puede quitar al dueño **sin efectos dañinos**. Después se le devuelve y sigue como si nada                                       | Si se lo quitás a mitad de uso, **la operación falla** o queda corrupta                                                                 |
| Por qué | Su estado **se puede guardar y restaurar**                                                                                             | Su estado está "en el mundo físico": no hay snapshot posible                                                                            |
| Ejemplo | **Memoria**: el SO la copia a disco (swap) y después la vuelve a traer. **CPU**: el timer se la saca y el contexto se guarda en el PCB | **Impresora** a mitad de una impresión: te quedan hojas mezcladas. **Grabadora de Blu-ray** a mitad de grabar: el disco queda arruinado |
Hay un detalle: que un resource sea preemptable **depende del contexto, no del objeto**. La memoria es preemptable si hay swap. En un sistema sin swap (Tanenbaum pone de ejemplo un celular) deja de serlo.

**Por qué importa esto:** los deadlocks que nos interesan involucran resources **nonpreemptable**. Si el resource es preemptable, el potencial deadlock se resuelve sacándoselo a uno y dándoselo al otro. Esto es justamente la condición 3 de Coffman, _no preemption_, que vemos en el ⭐4.

#### Con qué se confunde
- **Preemptable resource vs `Preemptive scheduling`**: _preemptable_ es una **propiedad del resource** (se puede quitar). _Preemptive scheduling_ es una **política del scheduler** que aprovecha que la CPU es un resource preemptable.
- **Resource vs `Region critica`**: la región crítica es el **pedazo de código** que toca el resource. El resource es **la cosa compartida**.