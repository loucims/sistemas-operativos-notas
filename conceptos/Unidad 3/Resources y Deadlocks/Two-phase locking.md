---
requiere:
  - "[[Deadlock prevention]]"
habilita:
  - "[[Livelock]]"
creado: 2026-09-30
---
#### Qué problema resuelve
Para romper _hold and wait_, `Deadlock prevention` dice "pedí todo de una vez". Pero **¿cómo** pedís varios locks "de una vez", si en código se toman **de a uno**? Two-phase locking es la forma concreta de hacerlo.

#### Cómo funciona (p.42)

> **Fase 1: lockeo.** Intentan adquirir los locks. _"Si cualquier intento falla, liberar todo y comenzar nuevamente."_  
> **Fase 2: uso.** _"Usar cualquier recurso solamente después que se adquirieron todos los locks. Liberar los locks cuando se termina."_

La clave es **cómo** se intenta tomar cada lock: con un `try_lock`, que **no bloquea**. Si el lock está libre, lo toma y devuelve verdadero. Si está ocupado, devuelve falso **al instante**, sin dormirse.

```c
int tomar_todos(lock locks[], int n) {
    for (int i = 0; i < n; i++) {
        if (!try_lock(&locks[i])) {        // ocupado: no me duermo, me                                                 entero al toque
            for (int j = 0; j < i; j++)
                unlock(&locks[j]);         // suelto TODO lo que ya había                                               tomado
            return FALSE;
        }
    }
    return TRUE;                           // conseguí todos
}

// ──────────────── cada proceso ────────────────
void proceso(void) {
    while (!tomar_todos(locks, n))         // FASE 1: reintento hasta                                                   tener todo
        ;
    usar_recursos();                       // FASE 2: uso
    soltar_todos(locks, n);
}
```

#### Por qué rompe _hold and wait_
Un proceso **nunca espera teniendo algo en la mano**. O consigue todo, o suelta todo y vuelve a empezar desde cero. Si nadie espera con algo en la mano, no se puede formar la cadena de esperas.

#### Problemas (p.42)
- _"Seguro, pero ineficiente para muchas aplicaciones."_
- _"Presenta problemas similares a los spin locks."_ Mirá el `while (!tomar_todos(...))`: mientras no consigue todo, el proceso **gira reintentando**. Es `Busy waiting`: gasta CPU sin avanzar.
- Hay que **saber de antemano** todos los locks que vas a necesitar.
- **Uso:** _"Usado en bases de datos"_. Una transacción toma los locks de todos los registros que va a tocar y recién después los modifica.


⚠️ Hay un problema más, que es el puente al próximo concepto. Mirá qué pasa si A y B necesitan r1 y r2, y el scheduler los intercala mal:

|Paso|A|B|
|---|---|---|
|1|toma r1|toma r2|
|2|intenta r2 → falla|intenta r1 → falla|
|3|suelta r1|suelta r2|
|4|toma r1|toma r2|
|…|… se repite para siempre|…|
**Nadie está bloqueado**, los dos corren todo el tiempo, pero **ninguno avanza**. Eso es el ⭐14 `Livelock`.

#### Con qué se confunde
- **Two-phase locking vs "pedir todo de una vez"**: "pedir todo de una vez" es la **idea** para romper hold and wait. Two-phase locking es **cómo se implementa** con locks.
- **Fase 1 vs hold and wait**: en la fase 1 el proceso **sí tiene** algunos locks mientras intenta otros. Pero **no espera** con ellos: si falla, los suelta.
- **En bases de datos**, "two-phase locking" también nombra otra técnica: una fase donde la transacción solo **toma** locks y otra donde solo los **suelta**. Esa sirve para otra cosa (que las transacciones no se pisen), y no evita deadlocks por sí sola. Para la materia, vale lo de la slide.