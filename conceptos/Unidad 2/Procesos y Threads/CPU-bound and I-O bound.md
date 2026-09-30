---
requiere: []
habilita:
  - "[[Scheduler]]"
creado: 2026-09-27
---
El `=[[Scheduler]]` necesita decidir a quién darle la CPU, y para decidir bien le sirve saber **cómo se comporta** cada proceso. No todos usan la CPU igual: algunos la quieren todo el tiempo, y otros la usan un ratito y enseguida se van a esperar al disco o al teclado.

#### Cómo funciona
Todo proceso alterna entre dos cosas (slide U2_5):
- **CPU burst**: un tramo usando la CPU (calcular).
- **I/O burst**: un tramo bloqueado esperando E/S.

Según qué domina, se clasifica así:
```
 █ = usando CPU     ░ = esperando E/S (blocked)

 CPU-bound:  ████████████████░░████████████████░░██████████████
 I/O-bound:  █░░░░░░░░█░░░░░░░░░█░░░░░░░█░░░░░░░░░█░░░░░░░
```

|                 | **CPU-bound**                                              | **I/O-bound**                                                 |
| --------------- | ---------------------------------------------------------- | ------------------------------------------------------------- |
| Qué hace más    | Calcular                                                   | Leer y escribir (disco, red, teclado)                         |
| CPU bursts      | Largos                                                     | Cortos                                                        |
| Ejemplos        | Comprimir un video, entrenar un modelo, un loop de cálculo | Un editor de texto, un servidor web, `cp` de archivos grandes |
| Lo que lo frena | La velocidad de la CPU                                     | La velocidad del dispositivo                                  |

Según la slide, **la velocidad absoluta no importa; importa la proporción** entre CPU y E/S. Y como las CPUs se hicieron mucho más rápidas que los discos, cada vez más procesos terminan siendo I/O-bound. 

Si cada proceso pasa una fracción **p** de su tiempo esperando E/S, y hay **n** procesos en memoria, la CPU está ociosa solo cuando **todos** esperan a la vez. La probabilidad de eso es **pⁿ**. Entonces:
**Uso de la CPU total = 1 − pⁿ**

### El scheduler deberia siempre priorizar a los *I/O bound*

**Ya que** usan la CPU un ratito y enseguida se bloquean esperando E/S, así que se la devuelven rápido a los CPU-bound. Si los priorizás, mientras el I/O-bound espera al disco la CPU la aprovecha el CPU-bound: **trabajan la CPU y el dispositivo a la vez**. Si priorizás al CPU-bound, el I/O-bound casi nunca llega a pedir su E/S, y el disco queda parado.