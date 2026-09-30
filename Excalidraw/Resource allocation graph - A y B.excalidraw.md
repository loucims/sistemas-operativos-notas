---

excalidraw-plugin: parsed
tags: [excalidraw]

---
==⚠  Switch to EXCALIDRAW VIEW in the MORE OPTIONS menu of this document. ⚠==


# Excalidraw Data

## Text Elements
Resource allocation graph: A y B cruzados (U3_2, p.9-12) ^titulo

cuadrado = resource     circulo = proceso
azul, linea llena:  R -> P  = R asignado a P (tiene)
rojo, punteada:     P -> R  = P bloqueado esperando R ^leyenda

R1 ^R1_t

A ^A_t

R2 ^R2_t

B ^B_t

R1 asignado a A ^l1

A espera R2
(paso 3) ^l2

R2 asignado a B ^l3

B espera R1
(paso 4) ^l4

CICLO
A -> R2 -> B -> R1 -> A ^ciclo

Seguir las flechas = seguir la cadena de esperas.
El ciclo vuelve al principio -> deadlock (con 1 instancia por resource). ^nota

%%
## Drawing
```json
{
 "type": "excalidraw",
 "version": 2,
 "source": "https://github.com/zsviczian/obsidian-excalidraw-plugin",
 "elements": [
  {
   "id": "titulo",
   "type": "text",
   "x": 60,
   "y": 20,
   "width": 739.2,
   "height": 30.0,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 1390851129,
   "version": 1,
   "versionNonce": 647892280,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "Resource allocation graph: A y B cruzados (U3_2, p.9-12)",
   "originalText": "Resource allocation graph: A y B cruzados (U3_2, p.9-12)",
   "fontSize": 24,
   "fontFamily": 1,
   "textAlign": "left",
   "verticalAlign": "top",
   "containerId": null,
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "leyenda",
   "type": "text",
   "x": 60,
   "y": 70,
   "width": 524.7,
   "height": 67.5,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 1695753999,
   "version": 1,
   "versionNonce": 207388625,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "cuadrado = resource     circulo = proceso\nazul, linea llena:  R -> P  = R asignado a P (tiene)\nrojo, punteada:     P -> R  = P bloqueado esperando R",
   "originalText": "cuadrado = resource     circulo = proceso\nazul, linea llena:  R -> P  = R asignado a P (tiene)\nrojo, punteada:     P -> R  = P bloqueado esperando R",
   "fontSize": 18,
   "fontFamily": 1,
   "textAlign": "left",
   "verticalAlign": "top",
   "containerId": null,
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "R1",
   "type": "rectangle",
   "x": 120,
   "y": 220,
   "width": 90,
   "height": 90,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "#e7f5ff",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 311111476,
   "version": 1,
   "versionNonce": 404285458,
   "isDeleted": false,
   "boundElements": [
    {
     "type": "text",
     "id": "R1_t"
    },
    {
     "type": "arrow",
     "id": "a1"
    },
    {
     "type": "arrow",
     "id": "a4"
    }
   ],
   "updated": 1,
   "link": null,
   "locked": false
  },
  {
   "id": "R1_t",
   "type": "text",
   "x": 151.8,
   "y": 250.0,
   "width": 26.400000000000002,
   "height": 30.0,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 1570621945,
   "version": 1,
   "versionNonce": 249103478,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "R1",
   "originalText": "R1",
   "fontSize": 24,
   "fontFamily": 1,
   "textAlign": "center",
   "verticalAlign": "middle",
   "containerId": "R1",
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "A",
   "type": "ellipse",
   "x": 440,
   "y": 220,
   "width": 90,
   "height": 90,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "#fff9db",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": {
    "type": 2
   },
   "seed": 922121677,
   "version": 1,
   "versionNonce": 161042649,
   "isDeleted": false,
   "boundElements": [
    {
     "type": "text",
     "id": "A_t"
    },
    {
     "type": "arrow",
     "id": "a1"
    },
    {
     "type": "arrow",
     "id": "a2"
    }
   ],
   "updated": 1,
   "link": null,
   "locked": false
  },
  {
   "id": "A_t",
   "type": "text",
   "x": 478.4,
   "y": 250.0,
   "width": 13.200000000000001,
   "height": 30.0,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 369140571,
   "version": 1,
   "versionNonce": 1862494043,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "A",
   "originalText": "A",
   "fontSize": 24,
   "fontFamily": 1,
   "textAlign": "center",
   "verticalAlign": "middle",
   "containerId": "A",
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "R2",
   "type": "rectangle",
   "x": 440,
   "y": 500,
   "width": 90,
   "height": 90,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "#e7f5ff",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 1796035740,
   "version": 1,
   "versionNonce": 300026768,
   "isDeleted": false,
   "boundElements": [
    {
     "type": "text",
     "id": "R2_t"
    },
    {
     "type": "arrow",
     "id": "a2"
    },
    {
     "type": "arrow",
     "id": "a3"
    }
   ],
   "updated": 1,
   "link": null,
   "locked": false
  },
  {
   "id": "R2_t",
   "type": "text",
   "x": 471.8,
   "y": 530.0,
   "width": 26.400000000000002,
   "height": 30.0,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 1033639717,
   "version": 1,
   "versionNonce": 389609434,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "R2",
   "originalText": "R2",
   "fontSize": 24,
   "fontFamily": 1,
   "textAlign": "center",
   "verticalAlign": "middle",
   "containerId": "R2",
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "B",
   "type": "ellipse",
   "x": 120,
   "y": 500,
   "width": 90,
   "height": 90,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "#fff9db",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": {
    "type": 2
   },
   "seed": 1823296039,
   "version": 1,
   "versionNonce": 253877687,
   "isDeleted": false,
   "boundElements": [
    {
     "type": "text",
     "id": "B_t"
    },
    {
     "type": "arrow",
     "id": "a3"
    },
    {
     "type": "arrow",
     "id": "a4"
    }
   ],
   "updated": 1,
   "link": null,
   "locked": false
  },
  {
   "id": "B_t",
   "type": "text",
   "x": 158.4,
   "y": 530.0,
   "width": 13.200000000000001,
   "height": 30.0,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 531725348,
   "version": 1,
   "versionNonce": 958804058,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "B",
   "originalText": "B",
   "fontSize": 24,
   "fontFamily": 1,
   "textAlign": "center",
   "verticalAlign": "middle",
   "containerId": "B",
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "a1",
   "type": "arrow",
   "x": 215,
   "y": 265,
   "width": 220,
   "height": 0,
   "angle": 0,
   "strokeColor": "#1971c2",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": {
    "type": 2
   },
   "seed": 265695474,
   "version": 1,
   "versionNonce": 1703729685,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "points": [
    [
     0,
     0
    ],
    [
     220,
     0
    ]
   ],
   "lastCommittedPoint": null,
   "startArrowhead": null,
   "endArrowhead": "arrow",
   "startBinding": {
    "elementId": "R1",
    "focus": 0,
    "gap": 5
   },
   "endBinding": {
    "elementId": "A",
    "focus": 0,
    "gap": 5
   },
   "elbowed": false
  },
  {
   "id": "a2",
   "type": "arrow",
   "x": 485,
   "y": 315,
   "width": 0,
   "height": 180,
   "angle": 0,
   "strokeColor": "#e03131",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "dashed",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": {
    "type": 2
   },
   "seed": 212984477,
   "version": 1,
   "versionNonce": 949539217,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "points": [
    [
     0,
     0
    ],
    [
     0,
     180
    ]
   ],
   "lastCommittedPoint": null,
   "startArrowhead": null,
   "endArrowhead": "arrow",
   "startBinding": {
    "elementId": "A",
    "focus": 0,
    "gap": 5
   },
   "endBinding": {
    "elementId": "R2",
    "focus": 0,
    "gap": 5
   },
   "elbowed": false
  },
  {
   "id": "a3",
   "type": "arrow",
   "x": 435,
   "y": 545,
   "width": 220,
   "height": 0,
   "angle": 0,
   "strokeColor": "#1971c2",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": {
    "type": 2
   },
   "seed": 200071089,
   "version": 1,
   "versionNonce": 571981486,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "points": [
    [
     0,
     0
    ],
    [
     -220,
     0
    ]
   ],
   "lastCommittedPoint": null,
   "startArrowhead": null,
   "endArrowhead": "arrow",
   "startBinding": {
    "elementId": "R2",
    "focus": 0,
    "gap": 5
   },
   "endBinding": {
    "elementId": "B",
    "focus": 0,
    "gap": 5
   },
   "elbowed": false
  },
  {
   "id": "a4",
   "type": "arrow",
   "x": 165,
   "y": 495,
   "width": 0,
   "height": 180,
   "angle": 0,
   "strokeColor": "#e03131",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "dashed",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": {
    "type": 2
   },
   "seed": 1243862423,
   "version": 1,
   "versionNonce": 1800188483,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "points": [
    [
     0,
     0
    ],
    [
     0,
     -180
    ]
   ],
   "lastCommittedPoint": null,
   "startArrowhead": null,
   "endArrowhead": "arrow",
   "startBinding": {
    "elementId": "B",
    "focus": 0,
    "gap": 5
   },
   "endBinding": {
    "elementId": "R1",
    "focus": 0,
    "gap": 5
   },
   "elbowed": false
  },
  {
   "id": "l1",
   "type": "text",
   "x": 245,
   "y": 232,
   "width": 148.5,
   "height": 22.5,
   "angle": 0,
   "strokeColor": "#1971c2",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 619570853,
   "version": 1,
   "versionNonce": 505913793,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "R1 asignado a A",
   "originalText": "R1 asignado a A",
   "fontSize": 18,
   "fontFamily": 1,
   "textAlign": "left",
   "verticalAlign": "top",
   "containerId": null,
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "l2",
   "type": "text",
   "x": 500,
   "y": 385,
   "width": 108.9,
   "height": 45.0,
   "angle": 0,
   "strokeColor": "#e03131",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 1324919353,
   "version": 1,
   "versionNonce": 776213900,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "A espera R2\n(paso 3)",
   "originalText": "A espera R2\n(paso 3)",
   "fontSize": 18,
   "fontFamily": 1,
   "textAlign": "left",
   "verticalAlign": "top",
   "containerId": null,
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "l3",
   "type": "text",
   "x": 245,
   "y": 560,
   "width": 148.5,
   "height": 22.5,
   "angle": 0,
   "strokeColor": "#1971c2",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 442620899,
   "version": 1,
   "versionNonce": 806899910,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "R2 asignado a B",
   "originalText": "R2 asignado a B",
   "fontSize": 18,
   "fontFamily": 1,
   "textAlign": "left",
   "verticalAlign": "top",
   "containerId": null,
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "l4",
   "type": "text",
   "x": 20,
   "y": 385,
   "width": 108.9,
   "height": 45.0,
   "angle": 0,
   "strokeColor": "#e03131",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 1599435268,
   "version": 1,
   "versionNonce": 418461139,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "B espera R1\n(paso 4)",
   "originalText": "B espera R1\n(paso 4)",
   "fontSize": 18,
   "fontFamily": 1,
   "textAlign": "left",
   "verticalAlign": "top",
   "containerId": null,
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "ciclo",
   "type": "text",
   "x": 230,
   "y": 380,
   "width": 253.00000000000003,
   "height": 50.0,
   "angle": 0,
   "strokeColor": "#e03131",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 269676600,
   "version": 1,
   "versionNonce": 255985077,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "CICLO\nA -> R2 -> B -> R1 -> A",
   "originalText": "CICLO\nA -> R2 -> B -> R1 -> A",
   "fontSize": 20,
   "fontFamily": 1,
   "textAlign": "left",
   "verticalAlign": "top",
   "containerId": null,
   "lineHeight": 1.25,
   "autoResize": true
  },
  {
   "id": "nota",
   "type": "text",
   "x": 60,
   "y": 630,
   "width": 712.8000000000001,
   "height": 45.0,
   "angle": 0,
   "strokeColor": "#1e1e1e",
   "backgroundColor": "transparent",
   "fillStyle": "solid",
   "strokeWidth": 2,
   "strokeStyle": "solid",
   "roughness": 1,
   "opacity": 100,
   "groupIds": [],
   "frameId": null,
   "roundness": null,
   "seed": 884585952,
   "version": 1,
   "versionNonce": 2132084005,
   "isDeleted": false,
   "boundElements": [],
   "updated": 1,
   "link": null,
   "locked": false,
   "text": "Seguir las flechas = seguir la cadena de esperas.\nEl ciclo vuelve al principio -> deadlock (con 1 instancia por resource).",
   "originalText": "Seguir las flechas = seguir la cadena de esperas.\nEl ciclo vuelve al principio -> deadlock (con 1 instancia por resource).",
   "fontSize": 18,
   "fontFamily": 1,
   "textAlign": "left",
   "verticalAlign": "top",
   "containerId": null,
   "lineHeight": 1.25,
   "autoResize": true
  }
 ],
 "appState": {
  "gridSize": null,
  "viewBackgroundColor": "#ffffff"
 },
 "files": {}
}
```
%%