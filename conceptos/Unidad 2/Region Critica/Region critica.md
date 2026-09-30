---
requiere:
  - "[[Race condition]]"
habilita:
  - "[[Mutual exclusion]]"
creado: 2026-09-27
---
Con `=[[Race condition]]` viste el problema: dos hilos tocan el mismo dato y se pisan. Para arreglarlo, primero hay que **ubicar exactamente dónde** está el peligro. No es todo el programa: es solo el pedazo que toca el dato compartido.

_"La parte de un programa que utiliza una variable compartida se denomina **región crítica**. Dos (o más) programas no deben estar en sus regiones críticas al mismo tiempo."_
#### Cómo funciona
En el ejemplo de la cátedra, la región crítica es solo esto:
```c
void *threadfunc(void *arg) {
    for (int j = 0; j < 1000000000; j++)
        x++;            // ← REGIÓN CRÍTICA: toca x, que es                            compartida
    return 0;
}
```
El loop, `j` y el `return` no son críticos, porque `j` es local (cada hilo tiene la suya en su stack).

Cualquier solución tiene esta forma:
```c
while (1) {
    // ── sección de entrada ──   pedir permiso para entrar
    //    REGIÓN CRÍTICA          tocar el dato compartido
    // ── sección de salida ──    avisar que salí
    //    región no crítica       todo lo demás
}
```
Los siguientes conceptos (`Disabling interrupts`, `Lock variable`, `Strict alternation`, `Peterson's solution`, `Test-and-set`…) son **distintas formas de escribir la entrada y la salida**. Todas se evalúan con los mismos requisitos.

#### Los requisitos de una buena solución (slide U2_3)

| #   | Requisito                | Qué significa                                                                                                                              | Ejemplo de solución que lo viola                                                      |
| --- | ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------- |
| 1   | **[[Mutual exclusion]]** | Nunca dos adentro de la región crítica a la vez                                                                                            | No poner nada en la entrada: vuelve la race condition                                 |
| 2   | **Progreso**             | Si la región está libre y alguien quiere entrar, alguien tiene que poder entrar. No puede quedar todo trabado esperando que otro haga algo | Una "solución" que nunca deja entrar a nadie: cumple el 1, pero no sirve              |
| 3   | **Espera limitada**      | Ningún hilo espera **para siempre** para entrar                                                                                            | Una que siempre le da prioridad a A: si A quiere entrar todo el tiempo, B nunca entra |
| —   | **Sin suposiciones**     | No puede depender de la velocidad de los hilos ni de cuántas CPUs hay                                                                      | "A siempre termina antes de que B llegue, porque es más rápido"                       |

|                        | **Progreso** (2)                                                                | **Espera limitada** (3)                    |
| ---------------------- | ------------------------------------------------------------------------------- | ------------------------------------------ |
| ¿La región está libre? | **Sí**, y aun así nadie puede entrar                                            | No siempre: la van usando otros            |
| ¿Por qué no entrás?    | Porque depende de alguien que **está afuera** (y que ni siquiera quiere entrar) | Porque **siempre** se te adelanta otro     |
| Imagen                 | Un baño vacío, pero la llave la tiene alguien que se fue a su casa              | Una fila donde siempre se te cuela alguien |
^ confusion comun
#### Con qué se confunde
- **La región crítica es código, no el dato**: el dato es `x`; la región crítica son las **instrucciones** que lo tocan (`x++`).
- **No todo acceso a un dato compartido es peligroso**: si **todos** solo leen, no hay race condition. El problema aparece cuando **al menos uno escribe**.
- **Tiene que ser lo más corta posible**: mientras un hilo está adentro, los otros esperan. Si metés código de más en la región crítica, perdés el paralelismo que buscabas con los hilos.