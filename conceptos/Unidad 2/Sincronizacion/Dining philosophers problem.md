#### Qué problema es (slide U3_1, "Los filósofos cenando")

Cinco filósofos alrededor de una mesa redonda. Cada uno tiene un plato de espaguetis, y entre cada par de platos hay **un tenedor**: 5 tenedores en total. Se pasan la vida **pensando** o **comiendo**, y para comer necesitan **los dos tenedores**, el de su izquierda y el de su derecha.
![[Dining philosophers.excalidraw|500]]
> *Filósofo* i usa el tenedor i  y el (i+1) % 5 
#### Qué es `% 5`
`%` es el **resto de la división**. `(i + 1) % 5` quiere decir "el siguiente, pero después del 4 volvé al 0":

| `i` | `i + 1` | `(i + 1) % 5`                   |
| --- | ------- | ------------------------------- |
| 0   | 1       | 1                               |
| 1   | 2       | 2                               |
| 2   | 3       | 3                               |
| 3   | 4       | 4                               |
| 4   | 5       | **0** ← 5 dividido 5 da resto 0 |
Es exactamente el mismo truco del `=[[Circular buffer]]` (`in = (in + 1) % N`). Como la mesa es **redonda**, después del último viene el primero.

#### La solución obvia, y por qué falla (slide "Una mala solución")
Cada tenedor es un `=[[Mutex]]`: primero tomo el derecho, después el izquierdo.

```c
void philosopher(int i) {
    while (TRUE) {
        think();
        take_fork(i);              // derecho
        take_fork((i + 1) % N);    // izquierdo
        eat();
        put_fork(i);
        put_fork((i + 1) % N);
    }
}
```

Si **los cinco** terminan de pensar a la vez:

| #   | Qué pasa                                                                                 |
| --- | ---------------------------------------------------------------------------------------- |
| 1   | Cada uno toma **su tenedor derecho**: F0 toma t0, F1 toma t1, … F4 toma t4               |
| 2   | Cada uno intenta tomar el **izquierdo**… que es el derecho de su vecino, que ya lo tiene |
| 3   | Los cinco se duermen esperando. **Nadie suelta el suyo.**                                |
Cada uno tiene **un** tenedor y espera **otro** que tiene el de al lado, formando un **círculo**. Es un **`Deadlock`**, el mismo patrón que el producer-consumer con los `down` al revés: retener algo mientras esperás otra cosa.

#### Los intentos de arreglo (slides en orden)

|Intento|Idea|Resultado|
|---|---|---|
|**"Reparemos la solución"**|Si el derecho no está libre, **suelto el izquierdo**, espero un rato y reintento|Si todos lo hacen **sincronizados**: todos toman, todos sueltan, todos esperan lo mismo, todos reintentan… Slide: _"Todos pueden correr, pero nadie avanza"_. Nadie está bloqueado, pero nadie come|
|**Aleatorización**|Esperar un tiempo **al azar** antes de reintentar|_"Normalmente funcionará bien"_, pero no está **garantizado**. Hay sistemas que no pueden depender de la suerte|
|**Un mutex global**|Antes de tomar tenedores, tomar un mutex para toda la mesa|**Funciona siempre**, pero come **uno solo** a la vez, cuando podrían comer dos (F0 y F2 no comparten tenedores)|
|**Estados + un semáforo por filósofo** (`phil3.cpp`)|Con un mutex, mirás si **tus dos vecinos** no están comiendo. Si no, comés; si sí, te marcás "con hambre" y dormís. Al terminar, **mirás si tus vecinos tenían hambre** y los despertás|**Funciona** y permite que coman **dos a la vez**. Es la solución clásica de Tanenbaum|
|**Asignación jerárquica**|Numerar los tenedores y tomarlos **siempre del número más chico al más grande**|**No hay deadlock** (aunque sí puede haber inanición). Pregunta 2|
La versión jerárquica es la más simple de escribir:
```c
semaphore tenedor[N];    // todos arrancan en 1

void philosopher(int i) {
    int a = i, b = (i + 1) % N;
    int primero = min(a, b);         // SIEMPRE el de número más chico                                            primero
    int segundo = max(a, b);
    while (TRUE) {
        think();
        down(&tenedor[primero]);
        down(&tenedor[segundo]);
        eat();
        up(&tenedor[primero]);
        up(&tenedor[segundo]);
    }
}
```

Para F0 a F3 no cambia nada (su derecho ya es el más chico). **Solo F4 cambia**: sus tenedores son t4 y t0, así que ahora toma **t0 primero**

#### Con qué se confunde
- **Deadlock ≠ "todos giran sin avanzar"**: en el deadlock están todos **dormidos**, esperando. En el segundo intento están todos **activos** (toman, sueltan, reintentan), pero ninguno progresa. Ese caso se llama **livelock**, y lo vas a ver en la Unidad 3.
- **Sin deadlock ≠ sin inanición**: la asignación jerárquica garantiza que el sistema nunca se traba entero, pero no que **cada** filósofo coma alguna vez.