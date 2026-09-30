---
requiere:
  - "[[Resource acquisition]]"
habilita:
  - "[[Deadlock conditions]]"
creado: 2026-09-30
---
#### Qué problema resuelve
Hasta acá vimos un caso concreto: A y B cruzados. Para poder **detectar**, **evitar** o **prevenir** ese caso (el resto de la unidad), primero hace falta una definición precisa que sirva para **cualquier** caso, con 2 procesos o con 50.

#### La definición
> _"Un conjunto de procesos está bloqueado si cada proceso del conjunto está esperando por un evento que solo otro proceso del conjunto puede provocar."_

Cada parte de la frase dice algo:

|Parte|Qué dice|
|---|---|
|**un conjunto**|Es un grupo de procesos, no uno solo. El problema está en la relación entre ellos|
|**cada proceso… está esperando**|**Todos** los del grupo están bloqueados. Si uno solo puede correr, no es deadlock|
|**un evento**|En esta unidad, el evento es casi siempre **la liberación de un resource** (el `up`)|
|**que solo otro del conjunto puede provocar**|**Nadie de afuera** los puede rescatar. El único que puede hacer ese `up` está adentro, y también está dormido|
La conclusión de la slide: _"Todos los procesos continúan esperando para siempre."_

Aplicado al ejemplo de antes:
```
Conjunto = {A, B}
  A espera el up(resource2) → solo lo puede hacer B → B está dormido
  B espera el up(resource1) → solo lo puede hacer A → A está dormido
  ¿Alguien de afuera tiene resource1 o resource2? No.
  → Deadlock.
```

**El test práctico:** ¿existe **algún** proceso, adentro o (*principalmente*) afuera del conjunto, que pueda correr y provocar el evento que destrabe a alguien? Si existe, **no** es deadlock.

#### Contexto (p.23–24)
- Pasa en **multiprogramación**: muchos procesos compitiendo por **resources finitos**.
- Históricamente fue muy estudiado en SO. Hoy es **más un problema de las aplicaciones** que del SO, sobre todo en **bases de datos**, con locks sobre registros.
#### Con qué se confunde

|                       | ¿Están bloqueados?         | ¿Alguien avanza?                      | ¿Se puede destrabar solo?          |
| --------------------- | -------------------------- | ------------------------------------- | ---------------------------------- |
| **Blocked normal**    | Sí                         | Otros sí                              | **Sí**: alguien va a hacer el `up` |
| **Deadlock**          | **Todos** los del conjunto | Nadie del conjunto                    | **Nunca**                          |
| **`=[[Starvation]]`** | No necesariamente          | **Los demás sí**, uno solo nunca pasa | Podría, pero siempre le ganan      |
| **Livelock** (⭐14)    | **No**: corren             | Nadie progresa                        | Tal vez, por azar                  |
