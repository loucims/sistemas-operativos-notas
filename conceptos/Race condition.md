---
requiere:
  - "[[Thread]]"
habilita: []
creado: 2026-09-27
---
Los hilos **comparten** las globales y el heap. Es la forma más fácil de comunicarse: uno escribe y el otro lee. Pero ahí aparece el problema: **si dos hilos tocan el mismo dato al mismo tiempo, el resultado puede salir mal.**
#### El ejemplo de la cátedra: `region_critica_incr.c`
```c
volatile unsigned x = 0;            // GLOBAL: compartida por los 2 hilos

void *threadfunc(void *arg) {
    for (int j = 0; j < 1000000000; j++)   // mil millones de veces
        x++;
    return 0;
}

int main() {
    pthread_t th[2];
    for (int i = 0; i < 2; i++)
        pthread_create(&th[i], NULL, threadfunc, NULL);  // crea 2 hilos
    for (int i = 0; i < 2; i++)
        pthread_join(th[i], NULL);                       // espera a los 2
    printf("%u\n", x);
}
```
_(El original cuenta el loop hacia atrás, de K−1 a 0. Es lo mismo: mil millones de vueltas.)_
Dos hilos, cada uno suma 1 mil millones de veces. Debería imprimir **2.000.000.000**. En la práctica imprime **menos**, y **un número distinto cada vez que lo corrés**.

Por qué: `x++` no es un solo paso:
En C se ve como una sola cosa, pero la CPU lo hace en **3 pasos**:
1. **Leer**: copiar `x` de la RAM a un registro.
2. **Sumar**: registro = registro + 1.
3. **Guardar**: copiar el registro de vuelta a `x` en la RAM.

Y cada hilo tiene **sus propios registros** (tu tabla de `Thread`). Así que si el scheduler cambia de hilo entre el paso 1 y el 3, pasa esto:

|#|Hilo A|Hilo B|`x` en RAM|registro A|registro B|
|---|---|---|---|---|---|
|1|lee x||5|5||
|—|_⏰ timer: cambio a B_|||||
|2||lee x|5|5|5|
|3||suma 1|5|5|6|
|4||guarda|**6**|5|6|
|—|_⏰ vuelve A_|||||
|5|suma 1||6|6||
|6|guarda||**6**|6||

Se hicieron **dos** `x++`, pero `x` pasó de 5 a **6**. Se perdió un incremento: A trabajó con un valor **viejo** de `x`, que había leído antes de que B lo cambiara.

En una máquina con varios núcleos es peor: A y B corren **al mismo tiempo** de verdad, y no hace falta ni siquiera un cambio de contexto para que se mezclen.

Qué hace `volatile`
- **Evita optimizaciones agresivas:** Impide que el compilador almacene el valor de la variable en un registro de la CPU o lo suprima asumiendo que no ha cambiado

- **Fuerza accesos reales a memoria:** Cada operación de lectura o escritura sobre una variable `volatile` genera de forma obligatoria una instrucción de acceso a la memoria RAM o al registro de E/S correspondiente. 


#### Por qué es tan difícil de encontrar
El resultado depende de **en qué instrucción exacta** cae el cambio de hilo, y eso depende del timer, de la carga de la máquina, de cuántos núcleos hay… La mayoría de las veces el cambio cae en otro lado y todo sale bien. Por eso la slide dice: _"las carreras causan bugs que son difíciles de reproducir"_.

#### Con qué se confunde

- **"Si es una sola línea de C, es atómica"**: no. `x++` son 3 pasos, y el cambio de hilo puede caer entre cualquiera de ellos.
- **`volatile` no lo arregla**: el ejemplo lo usa y falla igual. `volatile` solo obliga al compilador a leer y escribir la RAM cada vez (en vez de dejar `x` en un registro), pero no hace que los 3 pasos sean indivisibles.
- **Race condition ≠ "a los procesos no les pasa"**: entre procesos pasa lo mismo si comparten algo (un archivo, memoria compartida). Con hilos es más común porque comparten todo por defecto.