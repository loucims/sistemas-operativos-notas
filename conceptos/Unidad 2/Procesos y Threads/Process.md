---
requiere:
  - "[[Multiprogramming]]"
  - "[[RAM]]"
  - "[[Stack]]"
habilita:
  - "[[Process Control Block (PCB)]]"
creado: 2026-09-25
---
Un programa en disco es un archivo quieto: instrucciones y datos iniciales, nada mas. Pero en `=[[Multiprogramming]]` se quieren tener varios programas corriendo a la vez, y turnarlas en la CPU. 

Se debe tener algo que represente "*esta ejecucion*" en particular, con su 
progreso: por que instruccion va, que valor tiene cada variable, etc.
Y que dos ejecuciones, *aunque sean del mismo programa,* no se pisen entre si.

Eso es el **Process**: un programa en ejecucion, con su contexto propio.
#### Cómo funciona: qué tiene un process

| Qué tiene                    | Qué es                                         | Por qué tiene que ser propio                                   |
| ---------------------------- | ---------------------------------------------- | -------------------------------------------------------------- |
| *Address space*              | Su memoria privada (ver abajo)                 | Si fuera compartida, un proceso te cambia las variables a otro |
| *Program counter*            | La dirección de la próxima instrucción         | Cada ejecución va por un lugar distinto del código             |
| *Registers (incluido el SP)* | Los valores con los que está trabajando la CPU | Si se los pisan, la cuenta que estaba haciendo queda mal       |
| *Recursos del SO*            | PID, archivos abiertos, estado, proceso padre  | El SO tiene que saber qué le dio a quién                       |

**Address space**: así se ordena, desde la dirección mas alta hasta 0:

![[Process 2026-09-25 21.06.34.excalidraw|900]]

#### Con qué se confunde

|          | Program          | Process                                    |
| -------- | ---------------- | ------------------------------------------ |
| Que es   | Archivo en disco | Una *ejecucion o instancia* de ese archivo |
| Cambia?  | No, es estatico  | Si, avanza en el tiempo                    |
| Cuantos? | Uno              | 0, 1 o muchos a partir del mismo programa  |

- **Process ≠ aplicación**: una app puede ser varios procesos. Por ejemplo, un browser moderno suele usar un proceso por pestaña. La slide lo pregunta así: ¿qué pasa cuando hacés doble click en el browser por primera vez y por segunda?
- **Process ≠ thread**: un proceso puede tener varios hilos que comparten el address space. Lo vemos en `=[[Thread]]`.

#### Ejemplo concreto
```c 
//main.c
int contador = 0;          // data
int main() {
    contador++;
    printf("%d\n", contador);
}
```
Lo corrés en dos terminales al mismo tiempo. Pasa esto:
1. El SO crea **dos procesos** a partir del **mismo programa**.
2. El _text_ es el mismo código, pero cada proceso tiene su propio `contador`.
3. Los dos imprimen `1`, no `1` y `2`.
4. Incluso si los dos creen que `contador` está en la dirección `0x4000`, físicamente están en lugares distintos de la RAM. Eso lo hace la **relocatable memory** (la slide la menciona; se implementa con segmentación o memoria virtual, que viene más adelante).