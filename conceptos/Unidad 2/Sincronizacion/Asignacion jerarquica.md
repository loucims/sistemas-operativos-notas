---
requiere:
  - "[[Dining philosophers problem]]"
habilita: []
creado: 2026-09-30
---
##### Asignación jerárquica: romper la circular wait

**Regla:** se numeran todos los resources. Un proceso **solo puede pedir un resource con número MÁS ALTO que todos los que ya tiene**.

- Si tengo el 2, puedo pedir el 3, el 4, etc.
- Si tengo el 2 y necesito el 1, **no puedo pedirlo**. Tengo que:
    1. soltar el 2,
    2. pedir el 1,
    3. volver a pedir el 2.

**Por qué funciona:** si un proceso tiene el resource `i` y espera el `j`, siempre es `i < j`. Entonces cada espera **sube** de número:

```
P1 tiene R1, espera R3 → P2 tiene R3, espera R5 → P3 tiene R5, espera ...
       1   <   3            3   <   5              5   <  ...
```

Para cerrar un círculo, alguien tendría que esperar un resource **más bajo** que el que tiene, y eso la regla lo prohíbe. Sin círculo no hay circular wait, y sin circular wait no hay deadlock.

##### Ejemplo: `=[[Dining philosophers problem]]`

5 tenedores numerados del 0 al 4. El filósofo `i` usa `i` y `(i+1) % 5`.

**Sin la regla** (todos toman primero el derecho, después el izquierdo):

|Filósofo|Tiene|Espera|¿Sube?|
|---|---|---|---|
|F0|0|1|✅ 0 < 1|
|F1|1|2|✅ 1 < 2|
|F2|2|3|✅ 2 < 3|
|F3|3|4|✅ 3 < 4|
|F4|4|0|❌ **4 > 0** ← cierra el círculo|

F4 es el único que pide "hacia abajo", y por eso se cierra el círculo: 0 → 1 → 2 → 3 → 4 → 0.

**Con la regla** (cada uno pide primero el de número más bajo), F4 pide **0 y después 4**.  
Si los 5 arrancan a la vez:

|Filósofo|Pide primero|Resultado|
|---|---|---|
|F0|0|Compite con F4 por el 0. Supongamos que gana F0|
|F1|1|Lo toma|
|F2|2|Lo toma|
|F3|3|Lo toma|
|F4|0|Lo perdió con F0 → se bloquea **con las manos vacías**|

Como F4 no agarró nada, **el tenedor 4 queda libre**. F3 lo toma, come y suelta el 3 y el 4. Después F2 toma el 3 y come, y así en cascada. **Nadie queda trabado.**
```c
void philosopher(int i) {
    int primero = min(i, (i + 1) % N);   // el de número más bajo
    int segundo = max(i, (i + 1) % N);   // el de número más alto
    while (TRUE) {
        think();
        down(&fork[primero]);
        down(&fork[segundo]);
        eat();
        up(&fork[segundo]);
        up(&fork[primero]);
    }
}
```
La slide p.44 aclara que esto soluciona el deadlock, **pero puede haber `=[[Starvation]]`**.

##### Costo
- Hay que encontrar **un orden que sirva para todos** los programas.
- Si un programa necesita un resource de número más bajo que uno que ya tiene, tiene dos opciones, y las dos cuestan:
    - **soltar y volver a pedir**: pierde el resource mientras tanto, y quizás lo agarra otro,
    - **pedirlo desde el principio**: lo tiene ocioso mucho tiempo.