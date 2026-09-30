---
requiere:
  - "[[Process]]"
  - "[[Process Control Block (PCB)]]"
  - "[[Stack]]"
habilita:
  - "[[User-level thread]]"
  - "[[Kernel-level thread]]"
creado: 2026-09-27
---
Muchos programas tienen ***varias actividades a la vez sobre los mismos datos***. Por ejemplo, un editor de texto:
- una actividad atiende el teclado,
- otra reformatea el documento,
- otra hace autoguardado cada tanto.

Si cada actividad fuera un ***proceso*** separado habría dos problemas:
1. Cada proceso tiene su address space aislado, así que ***no comparten el documento**.* Habría que copiarlo de un lado al otro.
2. Pasar de uno a otro es un `Cambio de contexto` completo, con cambio de page table incluido: ***caro**.*

Un proceso en realidad son ***dos cosas mezcladas**,* que se pueden separar:

- una **colección de recursos**: address space, archivos abiertos, etc.;
- un **hilo de ejecución**: por dónde va, qué tiene en los registros.
##### Un **thread** es solo la segunda parte. Un proceso puede tener **varios threads** que comparten la primera.

| **Por proceso** (compartido por todos sus hilos) | **Por hilo** (propio de cada uno) |
| ------------------------------------------------ | --------------------------------- |
| Address space (text, data, heap)                 | Program counter                   |
| Variables globales                               | Registros                         |
| Archivos abiertos                                | Stack                             |
| Procesos hijos                                   | Estado (running, ready, blocked)  |
| Señales y handlers, alarmas                      |                                   |
| Info contable                                    |                                   |
|                                                  |                                   |

- **PC y registros**: cada hilo va por una instrucción distinta y hace su propia cuenta.
- **Stack**: cada hilo llama a funciones distintas, así que necesita sus propios frames y return addresses (tu nota `Stack`).
- **Estado**: un hilo puede estar blocked esperando el disco mientras otro del mismo proceso sigue running. (Aunque depende si son `=[[Kernel-level thread]]` o `=[[User-level thread]]`)

Así queda la memoria de un proceso con 3 hilos:
```
 UN solo address space
 ┌──────────────────┐
 │ text             │  ← el código: todos los hilos lo       |                  |    ejecutan
 │ data / bss       │  ← globales: COMPARTIDAS
 │ heap             │  ← malloc: COMPARTIDO
 │        ...       │
 │ stack hilo 3     │  ┐
 │ stack hilo 2     │  │ una stack por hilo
 │ stack hilo 1     │  ┘
 └──────────────────┘
```

***¿Y si una stack crece hasta la de otro hilo?*** Cuando se crea el hilo, se le reserva una zona de ***tamaño fijo*** (en Linux, por defecto, unos 8 MB). Entre stack y stack se deja una ***guard page***: una página marcada en la page table como "sin permiso de acceso".
```
 │ stack hilo 2     │
 │ ▓▓ guard page ▓▓ │  ← sin permisos
 │ stack hilo 1     │  ↓ crece hacia abajo
 │ ▓▓ guard page ▓▓ │  ← si el hilo 1 llega acá...
```
Si un hilo crece de más (por ejemplo, con una recursión infinita), toca la guard page, la MMU genera una `Exception` (_stack overflow_ / segfault) y el ***proceso entero muere***. Es mejor morir que pisar en silencio la stack de otro hilo.

#### Por qué el cambio entre hilos es más barato
Cambiar entre dos hilos del **mismo proceso**:
- guarda y carga PC, SP y registros (como siempre);
- **no cambia la page table**, porque el address space es el mismo;
- las cachés siguen "calientes", porque los datos son los mismos.

#### Modelos de threads
`=[[Kernel-level thread]]` = One-to-one
`=[[User-level thread]]` = Many-to-one

| Modelo           | Qué es                                                                               | Pros                                                           | Contras                                                            | Dónde                      |
| ---------------- | ------------------------------------------------------------------------------------ | -------------------------------------------------------------- | ------------------------------------------------------------------ | -------------------------- |
| **Many-to-One**  | Muchos hilos de usuario → 1 entidad del kernel                                       | Rápido                                                         | Todo lo de user-level: bloqueo, 1 núcleo                           | Green Threads (viejo Java) |
| **One-to-One**   | Cada hilo de usuario = **un hilo del kernel**                                        | Un hilo bloqueado no frena a los otros; usa **varios núcleos** | Crear hilos es más caro; muchos hilos pueden sobrecargar el kernel | **Linux y Windows**        |
| **Many-to-Many** | N hilos de usuario repartidos sobre M hilos del kernel (M ≤ N, acorde a los núcleos) | El más flexible                                                | Difícil de implementar                                             | Poco usado                 |

#### Con qué se confunde
- **Thread ≠ fork**: `fork` **copia** la memoria, y cada proceso cambia su copia. Los hilos **comparten** la memoria, así que lo que escribe uno lo ve el otro.
- **"Cada hilo tiene sus variables"**: solo las **locales**, porque viven en su stack. Las globales y el heap son de todos.
- **Las stacks no están protegidas entre sí**: están en el mismo address space. Un hilo con un bug puede pisar la stack de otro. La protección es entre **procesos**, no entre hilos.