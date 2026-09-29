---
requiere:
  - "[[Test-and-set]]"
habilita:
  - "[[Mutual exclusion]]"
  - "[[Mutex]]"
creado: 2026-09-29
---
`Test-and-set` resuelve la mutual exclusion, pero es tosca: **siempre escribe 1**. Solo sirve para armar un lock, y con un lock, si el hilo que lo tiene pierde la CPU a mitad de la región crítica, todos los demás quedan esperando.

CAS es una instrucción atómica **más general**: _"escribí este valor nuevo, pero **solo si** la variable todavía vale lo que yo creo"_. Con eso se puede actualizar un dato compartido **sin lock**.

Slide U2_3, "Instrucciones Atómicas": _"El procesador lee un valor, lo compara con un valor esperado y, si coinciden, escribe el nuevo valor. Todo esto ocurre como una única acción 
atómica. Es la base para construir los mutex en lenguajes como C++, Java o Rust."_
#### Cómo funciona
Lo que hace, escrito como si fuera C:
```c
// TODO ESTO ES UNA SOLA INSTRUCCIÓN ATÓMICA (en x86: LOCK CMPXCHG)
bool CAS(int *dir, int esperado, int nuevo) {
    if (*dir == esperado) {    // ¿sigue valiendo lo que yo leí?
        *dir = nuevo;          //   sí → escribo
        return true;
    }
    return false;              //   no → alguien lo cambió, no toco nada
}
```

**`x++` sin lock, con CAS:**
```c
int viejo;
do {
    viejo = x;                           // 1. leo (lectura normal)
} while (!CAS(&x, viejo, viejo + 1));    // 2. guardo SOLO si nadie lo                                             cambió
                                         //    si falló → vuelvo a leer y                                          reintento
```

Es el mismo leer / sumar / guardar de siempre, pero el guardado **verifica** que el valor no cambió desde que lo leíste. Si cambió, no pisa nada: reintenta con el valor nuevo.

**El caso que antes perdía un incremento:**

| #   | Hilo A                                                  | Hilo B                                   | `x`   |
| --- | ------------------------------------------------------- | ---------------------------------------- | ----- |
| 1   | `viejo = 5`                                             |                                          | 5     |
| —   | _⏰ cambio a B_                                          |                                          |       |
| 2   |                                                         | `viejo = 5`                              | 5     |
| 3   |                                                         | `CAS(&x, 5, 6)` → x vale 5 ✅ → escribe 6 | **6** |
| —   | _⏰ vuelve A_                                            |                                          |       |
| 4   | `CAS(&x, 5, 6)` → x vale **6**, no 5 ❌ → **no escribe** |                                          | 6     |
| 5   | reintenta: `viejo = 6`                                  |                                          | 6     |
| 6   | `CAS(&x, 6, 7)` → ✅ → escribe 7                         |                                          | **7** |
Dos incrementos → 7. No se perdió nada. Con la versión común, el paso 4 habría escrito 6 encima del 6 de B.

**También sirve para armar un lock**, igual que TSL: `while (!CAS(&lock, 0, 1)) ;` ("si está en 0, ponelo en 1").

#### El ejemplo de la cátedra: `incrcas.c`

Ojo: a pesar del nombre, **no usa CAS directamente**. Usa otra instrucción atómica, **fetch-and-add**, que hace "leer y sumar" en un solo paso:
```c
__sync_fetch_and_add(&shared_var_ptr->x, 1);   // x++ atómico, en una sola instrucción
```
Padre e hijo (con memoria compartida en xv6) suman 100 millones de veces cada uno, sin lock, y el resultado da exacto. Para un contador, fetch-and-add es lo más simple. CAS es más general: sirve para **cualquier** operación (`x * 2`, quedarse con el máximo, meter un elemento en una lista…).


#### Con qué se confunde
- **CAS vs TSL**:

|                | TSL           | CAS                                           |
| -------------- | ------------- | --------------------------------------------- |
| Qué escribe    | **Siempre 1** | **El valor que quieras**                      |
| Cuándo escribe | Siempre       | **Solo si** el valor actual es el esperado    |
| Para qué sirve | Armar un lock | Armar un lock **o** actualizar datos sin lock |

- **"Sin lock" ≠ "sin espera"**: en el loop de CAS, un hilo con mala suerte puede fallar y reintentar muchas veces si los otros siempre le ganan. Nunca se traba el sistema entero (siempre alguien avanza), pero un hilo individual puede tardar.
- **No es privilegiada**, por lo mismo que TSL: solo toca memoria del pro