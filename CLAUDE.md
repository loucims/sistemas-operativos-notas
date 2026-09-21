# Sistemas Operativos — vault de estudio

Vault de Obsidian para cursar Sistemas Operativos. Es una materia teórica donde
los conceptos se apilan: casi nada se entiende aislado. El trabajo es
**concepto por concepto**, y el valor está en las relaciones entre ellos, no en
el volumen de notas.

Todo se escribe en **español** (cuerpo de las notas, secciones y frontmatter),
**salvo los nombres de los conceptos**: van en **inglés** siempre que se pueda
(*Kernel*, *System call*, *Context switch*). Al proponer una nota nueva, Claude
propone el nombre en inglés. Algunas notas el usuario las nombra en español a
propósito: respetar el nombre que él le dé y no renombrar lo existente sin preguntar.

## Estructura

| Carpeta | Qué va acá |
|---|---|
| `conceptos/` | Una nota por concepto, plano, sin subcarpetas. Es el grueso del vault. |
| `mapas/` | Un MOC por unidad del programa: ordena los conceptos por dependencia |
| `referencias/` | Una nota por fuente (libro, apunte de cátedra, paper, clase) |
| `practicas/` | Ejercicios, parciales viejos, TPs resueltos |
| `plantillas/` | Plantillas de nota — copiar, no editar al usar |
| `adjuntos/` | Imágenes, diagramas y PDFs |

Notas índice en la raíz: `Inicio.md` (landing) y `Dudas abiertas.md`.

Las carpetas no clasifican por tema: un concepto que aparece en varias unidades
vive igual en `conceptos/` y se enlaza desde todos los mapas que lo usen.

## Convenciones

- **Nombres de archivo**: título con espacios y mayúscula inicial, tal como se
  nombra el concepto al hablar — `Context switch.md`, `Banker's algorithm.md`. Mapas: `Unidad 4 - Memoria.md`. Sin acortar ni abreviar: el
  nombre del archivo es el texto del enlace.
- **Singular**: `Semáforo.md`, no `Semáforos.md`. Excepto cuando el concepto es
  inherentemente plural (`Estados de un proceso`).
- **Enlaces**: `[[wikilinks]]` por título. Enlazar generosamente — un concepto
  sin enlaces entrantes ni salientes es una nota fallada. Un enlace a una nota
  que todavía no existe está perfecto: marca lo próximo a escribir.
- **Tags**: kebab-case, máximo 3 por nota, por tema transversal
  (`procesos`, `memoria`, `concurrencia`, `cpu`, `e-s`, `sistemas-de-archivos`).
  No taguear por unidad — eso ya está en el campo `unidad`.
- **Fechas**: `YYYY-MM-DD` siempre.
- **Adjuntos**: todos en `adjuntos/`, con nombre descriptivo
  (`diagrama-estados-proceso.png`), nunca `Pasted image 2026...`.

## Frontmatter

Obligatorio en toda nota. Campos según el `tipo`:

```yaml
---
titulo: Cambio de contexto
tipo: concepto          # concepto | mapa | fuente | practica | duda
unidad: 2 - Procesos
estado: en-progreso     # sin-empezar | en-progreso | entendido | repasar
requiere: ["[[PCB]]", "[[Interrupción]]"]
habilita: ["[[Planificador]]"]
creado: 2026-09-21
tags: [procesos, cpu]
---
```

- `requiere:` — conceptos que hay que entender **antes** que este.
- `habilita:` — conceptos que se apoyan en este. Es la inversa de `requiere`.
- Los dos campos llevan wikilinks entre comillas, en lista YAML. Mantenerlos
  coherentes: si `A` tiene `habilita: [[B]]`, entonces `B` debe tener
  `requiere: [[A]]`. Al crear una nota, **actualizar también el `habilita:` de
  sus prerrequisitos** — es el paso que más se olvida y es el que sostiene el grafo.
- `estado:` lo maneja el usuario, no Claude. No marcar algo como `entendido` por
  haberlo escrito bien.

## Anatomía de una nota de concepto

Estilo **minimalista**: la nota es texto libre, escrito por el usuario con sus
palabras. No se fuerza la estructura de secciones. Usar
`plantillas/Plantilla concepto.md`, que tiene solo:

1. **Frontmatter** completo (ver arriba) — es lo que sostiene el grafo.
2. **Cuerpo libre** — qué problema resuelve, cómo funciona, con qué se confunde,
   en el orden y formato que el usuario quiera. Encabezados chicos (`####`)
   para destacar una idea, por ejemplo `#### OS != [[Kernel]]`.
3. **Fuentes** — enlace a la nota de `referencias/` + capítulo/slide y página.

Al revisar una nota, chequear igual que se entienda qué problema resuelve el
concepto y con qué se confunde; si falta, sugerirlo, no agregarlo solo.

Cerrar toda nota de concepto con `Volver a [[Unidad N - Nombre]]`.

## Trabajando en este vault

- **Buscar antes de crear.** Si el concepto ya tiene nota, ampliarla; no crear una
  casi-duplicada con otro nombre. Sinónimos frecuentes en esta materia: revisar
  también el término en inglés antes de dar por inexistente una nota.
- **Notas atómicas.** Un concepto por nota. Si una nota crece dos temas, partirla
  y enlazar las mitades.
- **Nada de relleno.** Si no hay información real para una sección, dejarla con
  `*(pendiente)*` en vez de inventar prosa vacía. Una nota honestamente incompleta
  es mejor que una completa y hueca.
- **Al agregar un concepto**: crear la nota, enlazarla desde el mapa de su unidad
  en la posición correcta del orden de dependencia, y actualizar el `habilita:`
  de cada prerrequisito.
- **Dudas**: lo que quede sin resolver va a `Dudas abiertas.md` (y, si amerita
  desarrollo, a su propia nota con `tipo: duda`). No dejar una duda enterrada en
  el cuerpo de un concepto.
- **No reorganizar** carpetas ni renombrar en masa sin preguntar. Renombrar una
  nota rompe enlaces entrantes: si se hace, actualizarlos todos en la misma pasada.
- **No editar `.obsidian/`** salvo que el usuario lo pida. Las notas nuevas se
  crean en `conceptos/` (configurado en `app.json`).
- No hay daily notes.
