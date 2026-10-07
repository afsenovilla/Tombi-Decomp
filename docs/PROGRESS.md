# Progreso de la descompilacion de Tombi! (PAL espanol, SCES_013.31)

_Generado con `python tools/progress.py` (2026-10-07). No editar a mano._

## Resumen

| Nivel | Progreso | Bytes | Funciones |
|---|---|---|---|
| **C que coincide byte a byte (matching)** | **0.0 %** `[........................]` | 0 / 529048 | 0 |
| Nombradas por nosotros | 1.7 % `[........................]` | 9172 / 529048 | 57 / 1047 |
| Con la estructura TObj aplicada (cobertura, no es avance de C) | 66.1 % `[################........]` | 349920 / 529048 | 450 / 1047 |

"Codigo de juego" = funciones dentro del codigo de los programas analizados, **sin** contar las de la
libreria de Sony (Psy-Q), que Ghidra ya identifica. Esas son 98992 bytes (901 funciones) aparte.

## Desglose por programa

| Programa | Codigo de juego | Nombrado | Tipado (TObj) | Matching | Libreria Psy-Q |
|---|---|---|---|---|---|
| MAIN0.EXE (nucleo del juego) | 231052 B (519 f) | 3.5 % | 49.3 % | 0.0 % | 98952 B |
| X000.BIN (overlay AREA00) | 297996 B (528 f) | 0.3 % | 79.2 % | 0.0 % | 40 B |

## Que NO esta contado (el denominador real es mayor)

- `MAIN1..8.EXE`: comparten ~98 %% del codigo (`.text`) con MAIN0; se tratan como variantes, no se suman.
- `SCES_013.31` (cargador, 651 KB): casi todo es libreria Psy-Q; sin analizar.
- Resto de overlays `X*.BIN` de las 20 areas (solo se ha analizado `AREA00/X000.BIN`).
- Funciones que solo llama un overlay y que Ghidra no reconoce, y overlays aun sin identificar
  (172 destinos de MAIN0 en `0x800E8000+` no caen en X000).
- Funciones fuera del codigo (`482` bytes de falsos positivos de Ghidra en RAM sin contenido).

## Funciones sin nombrar mas grandes (siguientes objetivos)

| Tamano | Direccion | Programa |
|---|---|---|
| 6188 B | `800317c0` | MAIN0.EXE |
| 5740 B | `800f5078` | X000.BIN |
| 5708 B | `8003566c` | MAIN0.EXE |
| 5056 B | `80033b44` | MAIN0.EXE |
| 4360 B | `80051088` | MAIN0.EXE |
| 4088 B | `80100788` | X000.BIN |
| 3908 B | `800fc414` | X000.BIN |
| 3248 B | `80134e5c` | X000.BIN |
| 3152 B | `80133474` | X000.BIN |
| 3132 B | `80029078` | MAIN0.EXE |
| 3124 B | `80130bb0` | X000.BIN |
| 3080 B | `80024ea0` | MAIN0.EXE |
| 2888 B | `8003afb4` | MAIN0.EXE |
| 2672 B | `8010f400` | X000.BIN |
| 2632 B | `800f2b98` | X000.BIN |

## Como se mide

- Unidad: bytes de codigo por funcion (el tamano que da Ghidra).
- *Nombrada*: esta en `notes/names_*.csv` y no es de libreria. *Tipada*: su primer parametro es `TObj *`.
- *Matching*: funciones en `src/*.c` marcadas con `// MATCHING <direccion> <bytes>`; hoy `src/` no existe,
  asi que es 0 %. Todavia no hay toolchain para recompilar y comparar con el original.
- Las cifras de tipado salen del ultimo export de Ghidra (`notes/functions_*.csv`) y pueden ir por detras.
