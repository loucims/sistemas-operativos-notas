---
requiere:
  - "[[Deadlock detection]]"
habilita: []
creado: 2026-09-30
---
#### Qué problema resuelve
`Deadlock detection` te dice **que** hay deadlock y **quiénes** están en él, pero los procesos siguen colgados. Recovery es cómo **destrabarlos**. La idea de fondo es simple: **alguien del ciclo tiene que soltar un resource**. Como ninguno lo va a soltar por su cuenta, porque todos están dormidos, el SO los **obliga**.

#### Los 3 métodos (p.15)

| Método                              | Cómo funciona                                                                                                                                                            | Qué perdés                                                      | Cuándo sirve                                                                   |
| ----------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------- | ------------------------------------------------------------------------------ |
| **Preemption** (quitar el resource) | Le sacás el resource a uno y se lo das a otro del ciclo                                                                                                                  | Si el resource es nonpreemptable, rompés lo que estaba haciendo | _"Frecuentemente imposible"_. Solo funciona con resources que se pueden quitar |
| **Rollback** (revertir)             | Guardás el estado de cada proceso periódicamente (**checkpoints**). Si hay deadlock, volvés a la víctima al checkpoint **anterior a tomar el resource**, y así lo suelta | El trabajo hecho **desde el checkpoint**                        | **Bases de datos**: la transacción se deshace y se reintenta                   |
| **Matar procesos**                  | Matás procesos del ciclo **hasta que se rompa**                                                                                                                          | **Todo** el trabajo de la víctima                               | _"Crudo pero simple"_: siempre se puede                                        |
Cada método rompe el ciclo haciendo que **alguien suelte un resource**, por la fuerza:
- **Preemption**: se lo sacás.
- **Rollback**: lo hacés retroceder a antes de tenerlo. Es una preemption "prolija", porque restaura el estado.
- **Matar**: al morir, el proceso libera todo.

Esto conecta con tu respuesta de la base de datos en `Resource`: el **rollback** es lo que vuelve **preemptable** a un lock que no lo era.
#### Matar: ¿todos o de a uno? (p.21)

| Opción                                    | Ventaja                              | Desventaja                                                                                 |
| ----------------------------------------- | ------------------------------------ | ------------------------------------------------------------------------------------------ |
| **Todos** los del deadlock                | Rápido y seguro: el ciclo desaparece | Perdés el trabajo de **todos**                                                             |
| **De a uno**, hasta que se rompa el ciclo | Perdés menos                         | Después de cada muerte hay que **volver a correr la detección** para ver si sigue el ciclo |
#### ¿A quién elegir como víctima? (p.20–21)
Se elige **minimizando el costo**. La slide p.21 da estos criterios:

- **Prioridad** del proceso.
- Cuánto **lleva corriendo** y cuánto **le falta**.
- Cuántos **resources usó**, y cuántos **necesita** para terminar.
- Cuántos procesos **habría que matar** en total.
- Si es **interactivo** (hay un usuario esperando) o **batch**

⚠️ **Riesgo (p.20): `Starvation`.** Si siempre gana el criterio de "menor costo", **el mismo proceso** puede ser la víctima una y otra vez, y no termina nunca.

#### Con qué se confunde

- **Recovery vs `Deadlock prevention`**: prevention rompe una condición **antes, para siempre y para todos**. Recovery rompe **este** ciclo, **después** de que pasó.
- **Rollback vs matar**: rollback conserva el trabajo **hasta el checkpoint**. Matar pierde **todo**.