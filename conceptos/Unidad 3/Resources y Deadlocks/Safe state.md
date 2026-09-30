---
requiere:
  - "[[Deadlock]]"
habilita:
  - "[[Banker's algorithm]]"
  - "[[Deadlock avoidance]]"
creado: 2026-09-30
---
#### Qué problema resuelve
`Deadlock detection` mira **después**: el deadlock ya pasó y hay que pagar el recovery. Lo ideal sería decidir *antes de conceder cada pedido* si es peligroso. Para eso hay que conocer el **futuro**, así que se le pide a cada proceso que *declare de antemano el máximo* de resources que puede llegar a necesitar (p.26).

Con eso, el **estado** del sistema queda definido por tres datos (p.26):
- cuánto **tiene** cada proceso,
- cuál es su **máximo**,
- cuántos resources están **libres**.

#### La definición (p.27–28) ⭐

> _"Un estado es seguro si no está bloqueado y existe un orden de programación en el que todos los procesos pueden completarse, incluso si solicitan todos sus recursos a la vez."_

En criollo: aunque **todos pidan su máximo** (el peor caso), existe **alguna fila** para correrlos de a uno de modo que **todos terminen**. Esa fila es la **secuencia segura**.

#### Cómo se verifica
Es el mismo "simular que terminan" de `Deadlock detection`, pero mirando lo que **podrían llegar a pedir**, no el pedido actual:

1. Calculá **Necesita = Máx − Tiene** para cada proceso.
2. Buscá un proceso con **Necesita ≤ Libres**.
3. Suponé que recibe lo que necesita, **termina** y **devuelve todo**. Quedan **Libres = Libres + Tiene**.
4. Repetí el paso 2. Si terminan **todos**, el estado es **seguro**. Si te trabás, es **inseguro**.

#### Ejemplo seguro (p.30): 10 instancias de un mismo resource
Con *10 instancias*:

| Proceso | Tiene | Máx | **Necesita** |
| ------- | ----- | --- | ------------ |
| A       | 3     | 9   | 6            |
| B       | 2     | 4   | 2            |
| C       | 2     | 7   | 5            |
> Libres = 10 - (3 + 2 + 2) = 3

| Paso | ¿Quién puede? (Necesita ≤ Libres)   | Termina y devuelve | Libres después                       |
| ---- | ----------------------------------- | ------------------ | ------------------------------------ |
| 1    | **B** (2 ≤ 3) ✅ · A (6) ❌ · C (5) ❌ | 2 + 2 = 4          | 3 − 2 + 4 = **5**                    |
| 2    | **C** (5 ≤ 5) ✅ · A (6) ❌           | 2 + 5 = 7          | 5 − 5 + 7 = **7**                    |
| 3    | **A** (6 ≤ 7) ✅                     | 3 + 6 = 9          | 7 − 6 + 9 = **10** ✔️ vuelven los 10 |
La secuencia segura es **B → C → A**, así que el estado es **seguro**.
#### Ejemplo inseguro (p.31)
Desde el mismo estado, **A pide 1 más y se lo dan**. Ahora A tiene 4 y necesita 5, y quedan **Libres = 2**:

|Paso|¿Quién puede?|Libres después|
|---|---|---|
|1|**B** (2 ≤ 2) ✅|2 − 2 + 4 = **4**|
|2|A necesita 5 ❌ · C necesita 5 ❌|**trabado**|
No existe ningún orden, así que el estado es **inseguro**. Un solo resource de más a A lo cambió todo. Por eso ese pedido **no se debería conceder** (esto es `Deadlock avoidance`, ⭐11).

#### Seguro, inseguro y deadlock (p.28–29)
```
┌──────────────────── todos los estados ────────────────────┐
│  ┌──────────┐    ┌──────────── inseguros ────────────┐    │
│  │ seguros  │    │                  ┌──────────┐     │    │
│  │          │    │                  │ deadlock │     │    │
│  └──────────┘    │                  └──────────┘     │    │
│                  └───────────────────────────────────┘    │
└───────────────────────────────────────────────────────────┘
```

- **Seguro ⇒ el deadlock es imposible.**
- **Inseguro ⇒ el deadlock es posible, no seguro.** La p.28 dice que _"incluso podría funcionar si un proceso libera recursos en el momento adecuado"_. El máximo es el **peor caso**, y quizás nadie lo pide.

#### Con qué se confunde
- **Inseguro vs `Deadlock`**: inseguro significa que **no podés garantizar** que todos terminen. Deadlock significa que **ya están** trabados. Todo deadlock es inseguro, pero no todo inseguro es deadlock.
- **Safe state vs `Deadlock detection`**: el algoritmo es el mismo, pero detection usa el **pedido actual** y safe state usa lo que **podría pedir** (Máx − Tiene).


libres = 2
A necesita 5
B necesita 2
C necesita 7

B okay
libres = 4
A necesita 5
C necesita 7

a okay
libres = 10
c necesita 7

c okay
