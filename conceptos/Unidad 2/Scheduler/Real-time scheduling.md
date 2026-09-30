### Qué problema resuelve
Hasta ahora las métricas eran promedios: "que en promedio se espere poco". En real-time eso no alcanza. Lo que importa es que cada tarea termine **antes de su deadline**. El slide lo dice así: *"un proceso tardío puede ser un proceso inútil".*

Ejemplo: el sensor de un auto detecta un choque. Si el airbag se infla 2 segundos tarde, *que el promedio sea bueno no sirve de nada.*

### Hard vs soft

|                    |Qué pasa si se incumple un deadline|Ejemplos|
|---|---|---|
| **Hard** real-time |Falla del sistema. El deadline **debe** cumplirse.|Aviónica, control industrial, airbag|
| **Soft** real-time |Se degrada la calidad, pero se tolera de vez en cuando|Streaming de video o audio, videojuegos|
Los dos tipos necesitan **conocer de antemano cuánto CPU requiere cada tarea**. Sin eso no se puede garantizar nada.

### Eventos periódicos: la fórmula de schedulability
Muchas tareas real-time se repiten cada cierto tiempo, por ejemplo leer un sensor cada 10 ms. Cada tarea _i_ tiene:

- **Cᵢ**: cuánto CPU necesita cada vez que ocurre.
- **Pᵢ**: cada cuánto ocurre (su período).

La idea: **Cᵢ / Pᵢ es la fracción de CPU que esa tarea se come.** Si una tarea necesita 3 ms cada 10 ms, ocupa el 30% de la CPU, para siempre.

Entonces el sistema es *schedulable* si la suma de todas esas fracciones entra en una CPU:

```
  m
  ∑  Cᵢ / Pᵢ  ≤ 1        (la "utilización" total no puede pasar del 100%)
 i=1
```
Si da más de 1, estás pidiendo más CPU de la que existe y algún deadline se va a perder, sí o sí.

**Ejemplo del slide:**

|Tarea|Período P|CPU C|C/P|
|---|---|---|---|
|T1|10|3|0,30|
|T2|10|1|0,10|
|T3|15|8|0,53|
|**Total**|||**0,93 ≤ 1 → schedulable**|
### Eventos aperiódicos
Son los *que no ocurren con regularidad,* como una alarma de incendio. La regla de fondo es la misma: *la carga total tiene que ser menor que la capacidad.* Pero no se puede calcular con una fórmula simple, y por eso el slide dice que son más difíciles de diseñar.

### Convivencia con procesos comunes
Algunos sistemas tienen ambos tipos. La estrategia del slide: **las tareas real-time tienen prioridad sobre todas las comunes**. Entre ellas se planifican con RR, o incluso non-preemptive. Es priority scheduling con real-time en la cola de arriba, igual que la clase "Tiempo real" de Windows que vimos.

### Con qué se confunde

| Confusión                     | Aclaración                                                                                                                                             |
| ----------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| "Real-time = rápido"          | No. Real-time significa **predecible**: cumplir el deadline. Una tarea que tarda 1 segundo pero siempre cumple su deadline de 2 segundos es real-time. |
| Soft real-time = "no importa" | Sí importa, pero incumplir de vez en cuando no es catastrófico.                                                                                        |
| ∑ C/P ≤ 1 garantiza todo      | Es la condición de capacidad. Hay que sumar el costo de los context switches, y para eventos aperiódicos no alcanza.                                   |