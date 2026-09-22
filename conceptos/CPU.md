---
titulo: CPU
tipo: concepto
unidad: 1 - Introducción
estado: sin-empezar
requiere: ["[[RAM]]", "[[Arithmethic Logic Unit]]"]
habilita: ["[[Kernel]]", "[[Interrupt]]", "[[Dual-mode operation]]", "[[Cambio de contexto]]"]
creado: 2026-09-22
tags: [cpu]
---

La [[RAM]] guarda datos e instrucciones, pero no hace nada, es un deposito. Hace falta algo que tome instrucciones de a una y las ejecute (sumar, comparar, leer o escribir memoria, saltar). Y eso es la CPU. Todo lo que hace el [[Sistema Operativo]] termina siendo instrucciones que la CPU ejecuta

**Como funciona**
Contiene piezas

- **Registers:** Son variables dentro de la CPU implementadas con flip-flops, tan rapidas que se pueden leer y writear en un solo cyclo de clock.
	- **Program Counter (PC):** Contiene la direccion de memoria de la proxima instruccion
	- **Stack Pointer:** Contiene la direccion del tope del [[Stack]] en memoria
	- **Instruction Register:** Contiene la instruccion actual que esta decodeando y ejecutando la CPU
	- **Accumulator (ACC):** Contiene los resultados inmediatos de las operaciones aritmeticas y logicas de la ALU ([[Arithmethic Logic Unit]]) (Hoy en dia ya no existen, se usa el GPR que elijas)
	- **General-Purpose Registers (GPRs):** Datos de trabajo flexibles para programadores, pueden contener adresses o data.
	- **Flag Registers:** Contiene informacion sobre el resultado de la ultima operacion, lo usa todo el mundo. (Zero, carry, sign, overflow) 
	- **Control Registers:** Contiene informacion que configura la CPU. **Solo se tocan en modo privilegiado**
	Hay algunos registros los cuales pueden ser tocados por procesos y otros que solo el   Kernel puede tocar.

- **ALU (Arithmethic Logic Unit):** El circuito que hace las cuentas y las comparaciones

- **Control Unit:** El circuito que *decodifica* la instruccion y dirige a las demas piezas

- **Clock**: Un pulso periodico que marca el ritmo de velocidad del CPU, cada paso ocurren en un tick

- **Mode Bit:** Un bit de estado del CPU, y es lo importante al OS, la CPU sabe en que modo esta, y segun eso deja o no ejecutar instrucciones privilegiadas. Es lo que permite el [[Dual-mode operation]]
##### **El ciclo** (lo unico que hace la CPU, para siempre, mientras tenga corriente):
![[CPU 2026-09-22 00.00.10.excalidraw|900]]
