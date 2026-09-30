---
requiere:
  - "[[Busy waiting]]"
habilita:
  - "[[Peterson's solution]]"
creado: 2026-09-28
---
*Cumple Mutual exclusion, rompe progreso.*
#### Qué problema resuelve
La `=[[Lock variable]]` falló porque **los dos** hilos podían ver "libre" y **los dos** escribían "ocupado".
Strict alternation cambia la idea: en vez de un cartel de libre/ocupado, hay una variable **`turn`** que dice **a quién le toca**. 
Como un solo baño con una sola llave que se van pasando: solo entra el que tiene la llave, y al salir se la da al otro.

#### Cómo funciona (slide U2_3, "Alternancia estricta")
```c
int turn = 0;               // compartida: a quién le toca

// Proceso 0                          // Proceso 1
while (TRUE) {                        while (TRUE) {
    while (turn != 0) ;                   while (turn != 1) ;      //espero                              // espero
    critical_region();                    critical_region();
	turn = 1;                             turn = 0;            
    noncritical_region();                 noncritical_region();
}                                     }
```

- **Entrada**: gira (`=[[Busy waiting]]`) hasta que sea su turno.
- **Salida**: le pasa el turno **al otro**.

#### Por qué esta sí cumple mutual exclusion
En la lock variable, el problema era que los dos **escribían** después de mirar. Acá:

- **Solo el dueño del turno lo cambia**, y lo hace **al salir** de la región crítica. El que espera nunca escribe `turn`: solo lo lee.

No hay *"mirar y marcar"*: no hay ventana para la race condition.

#### Desventajas (slide "Desventajas de la alternancia estricta")
**1. Viola progreso.** Lo que ya viste en Critical section:

| #   | P0                                              | P1                              | `turn` |
| --- | ----------------------------------------------- | ------------------------------- | ------ |
| 1   | entra, sale, `turn = 1`                         |                                 | 1      |
| 2   | región no crítica (corta)                       | región no crítica **muy larga** | 1      |
| 3   | quiere entrar de nuevo → `turn != 0` → **gira** | sigue en su región no crítica…  | 1      |
| 4   | gira… (la región crítica está **libre**)        | sigue…                          | 1      |
P0 no puede entrar aunque nadie esté adentro, porque depende de alguien que está **afuera**. Slide: _"Esto viola nuestro principio de eficiencia."_

**2. Obliga a un orden fijo: 0, 1, 0, 1…** Aunque P0 quiera entrar 10 veces seguidas y P1 una sola vez, tienen que turnarse. Todos van al ritmo del más lento.

**3. Busy waiting**: el que espera gasta CPU girando.

#### Con qué se confunde
- **Strict alternation ≠ lock variable**: la lock variable pregunta "¿está libre?". Strict alternation pregunta "¿me toca?". Esa diferencia es la que elimina la race condition.
- **Cumple mutual exclusion, pero no es una buena solución**: es correcta (nunca hay dos adentro) pero ineficiente. La slide la cuenta entre _"las tres soluciones correctas"_, junto con Peterson y TSL.