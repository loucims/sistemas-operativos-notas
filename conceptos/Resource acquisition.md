---
requiere:
  - "[[Resource]]"
  - "[[Semaphore]]"
habilita:
  - "[[Deadlock]]"
creado: 2026-09-30
---
#### Qué problema resuelve
Ya sabés _qué_ es un resource. Falta ver **cómo lo pide y lo devuelve** un proceso. Con un solo resource no hay drama. Con **dos**, el **orden** en que los pedís decide si puede haber deadlock o no.

#### Cómo funciona: request → use → release (p.26)
1. **Request**: pedís el resource. Si está ocupado, esperás, ya sea con busy waiting o bloqueándote.
2. **Use**: lo usás.
3. **Release**: lo devolvés.

La forma natural de implementarlo es **un semáforo por resource** (p.27). `down` es el request y `up` es el release:
```c
semaphore resource1;   // arranca en 1 = libre

void processA(void) {
    down(&resource1);      // request: si está ocupado, me bloqueo acá
    use_resource1();       // use
    up(&resource1);        // release
}
```
Con dos resources es lo mismo, pero con dos `down` antes de usarlos (p.28):
```c
void processA(void) {
    down(&resource1);
    down(&resource2);
    use_both_resources();
    up(&resource2);
    up(&resource1);
}
```

#### Dos procesos: mismo orden → sin deadlock (p.29)
```c
void processA(void) {
    down(&resource1);
    down(&resource2);
    use_both_resources();
    up(&resource2);
    up(&resource1);
}

// ───────────────── processB: MISMO orden ─────────────────

void processB(void) {
    down(&resource1);
    down(&resource2);
    use_both_resources();
    up(&resource2);
    up(&resource1);
}
```

#### Dos procesos: orden cruzado → deadlock potencial (p.30)
```c
void processA(void) {
    down(&resource1);      // A pide 1 y después 2
    down(&resource2);
    use_both_resources();
    up(&resource2);
    up(&resource1);
}

// ───────────────── processB: orden CRUZADO ─────────────────

void processB(void) {
    down(&resource2);      // B pide 2 y después 1
    down(&resource1);
    use_both_resources();
    up(&resource2);
    up(&resource1);
}
```
Seguí esta intercalación, con un context switch justo después del primer `down` de A:

| Paso | Quién corre | Hace               | resource1 | resource2 | Resultado                     |
| ---- | ----------- | ------------------ | --------- | --------- | ----------------------------- |
| 1    | A           | `down(&resource1)` | **A**     | libre     | ok                            |
| 2    | _switch_    |                    |           |           |                               |
| 3    | B           | `down(&resource2)` | A         | **B**     | ok                            |
| 4    | B           | `down(&resource1)` | A         | B         | B se **bloquea** (lo tiene A) |
| 5    | A           | `down(&resource2)` | A         | B         | A se **bloquea** (lo tiene B) |
Así quedan los dos:
```
   A ──tiene──▶ resource1 ◀──espera── B
   A ──espera──▶ resource2 ◀──tiene── B
```

Cada uno tiene lo que el otro necesita, y ninguno va a hacer `up` porque los dos están dormidos. Se quedan así **para siempre**. Es el "abrazo mortal" de Alice con el escáner y Bob con la impresora.

**Por qué "potencial":** si A llega a hacer sus dos `down` antes del switch, todo anda bien. El deadlock depende de **cómo el scheduler intercale** los procesos, igual que una `Race condition`. Por eso puede andar mil veces y colgarse la mil uno.

**Un detalle de la slide p.31:** ahí los `up` aparecen en otro orden y no cambia nada. **El orden de los `up` no importa, porque `up` nunca bloquea.** Lo que decide todo es el orden de los `down`.

#### Con qué se confunde
- **Proceso blocked vs deadlock**: estar blocked en un `down` es **normal**, porque esperás a que alguien haga `up`. En un deadlock, ese alguien **nunca va a llegar**, porque también está esperando.