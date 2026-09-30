---
requiere:
  - "[[Deadlock conditions]]"
habilita:
  - "[[Two-phase locking]]"
creado: 2026-09-30
---
##### Qué problema resuelve
Es la consecuencia directa del "secreto" de `Deadlock conditions` (p.34): si una de las 4 condiciones *nunca* se cumple, el deadlock es **imposible**. Prevention elige una condición y la rompe *por diseño*, para todos y siempre.

Hay una pregunta del parcial por cada condición, así que las vemos de a una, con **técnica** y **dificultad**.

---

#### 1. Romper _mutual exclusion_ (p.39)

> _"¿En qué consiste? ¿Es posible en general?"_

- *Técnica*: que el resource se pueda **compartir**.
    - Ejemplo de la slide: un archivo que todos abren **solo para leer** se puede abrir a la vez.
    - **Spooling**: los procesos no usan la impresora directamente. Escriben su trabajo en un área del **disco** (el _spool_), que sí se comparte, y **un solo proceso** (el daemon de impresión) usa la impresora real. Ningún proceso "tiene" la impresora, así que nadie puede quedar esperándola con algo en la mano.
- *Dificultad*: **en general no es posible**. La slide dice: _"normalmente la exclusión mutua se necesita o no"_. Si dos procesos escriben el mismo registro a la vez, tenés una `Race condition`. Y no todo se puede spoolear: la tabla de procesos o un registro de una base de datos, por ejemplo.

#### 2. Romper _hold and wait_ (p.41–42)

> _"¿Qué podemos hacer para eliminar la precondición de retención y espera?"_

- *Técnica*: pedir **todos los resources de una sola vez**, al principio. Para pedir un conjunto nuevo, hay que **soltar todo** primero.
    - Variante: `=[[Two-phase locking]]` (⭐13). Primero intentás tomar todos los locks; si alguno falla, soltás todo y reintentás. Recién después los usás.
- *Dificultad*: _"Ineficiente"_.
    - Hay que **saber de antemano** todo lo que vas a usar.
    - Los resources quedan **tomados sin usar** mucho tiempo: agarraste la impresora al principio y la usás recién al final.

#### 3. Romper _no preemption_ (p.40)

> _"¿Cómo se puede eliminar? ¿Qué dificultades presenta?"_

- *Técnica*: una **política** que permita sacarle resources a un proceso. Ejemplo de la slide: si un proceso de alta prioridad quiere un resource, **se lo quita** al de baja prioridad.
- *Dificultad*: _"Cuando se saca un recurso la recuperación suele ser complicada."_ Solo funciona con resources **preemptable**, cuyo estado se puede guardar y restaurar (CPU, memoria). Sacarle la impresora a mitad de un trabajo te deja las hojas mezcladas. Es lo que viste en `Resource`.

#### 4. Romper _circular wait_ (p.43–44)

> _"¿Cómo puede eliminarse la precondición de espera circular?"_

- *Técnica*: asignación jerárquica. **Numerar** los resources y solo permitir pedir resources con **número más alto** que los que ya tenés.
- *Por qué funciona:* cada espera va de un número menor a uno mayor. La cadena siempre **sube**, así que no puede volver al principio a cerrar el círculo.
- *Filósofos* (p.44): con los tenedores en orden numérico, el filósofo 4 pide **0 y después 4**:

```c
void philosopher(int i) {
    int primero = min(i, (i + 1) % N);   // siempre el de número más bajo
    int segundo = max(i, (i + 1) % N);   // después el más alto
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

La slide aclara: _"Solucionamos los bloqueos (pero puede haber inanición)"_.

- *Dificultad*: _"Requiere encontrar la manera de numerar los recursos"_. Tiene que ser un orden que **sirva para todos los programas**, y el orden numérico puede no coincidir con el orden en que naturalmente los necesitás.


#### Resumen 

|Condición|Técnica|Dificultad|
|---|---|---|
|Mutual exclusion|Spooling / compartir|En general no se puede|
|Hold and wait|Pedir **todo de una vez**|Ineficiente: saber todo de antemano, resources ociosos|
|No preemption|**Quitarle** los resources al que los tiene|Recuperarse es complicado; solo sirve con resources preemptable|
|Circular wait|**Ordenar** los resources numéricamente|Encontrar un orden que sirva para todos|

#### Con qué se confunde
- **Prevention vs `Deadlock avoidance`**: ya lo cerraste. Prevention es una regla fija que hace **imposible** una condición. Avoidance deja las 4 condiciones posibles y **esquiva** el ciclo pedido por pedido.
- **Spooling vs "compartir el resource"**: con spooling la impresora **sigue siendo exclusiva**, pero la usa **un solo proceso**, el daemon. Lo que se comparte es el spool en disco.