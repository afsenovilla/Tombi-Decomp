# Mapa de memoria de MAIN0.EXE (importado con ghidra_psx_ldr)

| Bloque | Inicio | Fin | Tamano | Permisos | Con datos |
|---|---|---|---|---|---|
| .rdata | 80010000 | 800163F3 | 0x63F4 | R | si |
| .text | 800163F4 | 800778EF | 0x614FC | R X | si |
| .data | 800778F0 | 80098C43 | 0x21354 | R W | si |
| .sdata | 80098C44 | 80098FFF | 0x3BC | R W | si |
| .sbss | 8009BC48 | 8009BC7F | 0x38 | R W | no |
| .bss | 8009BC80 | 800A3FDF | 0x8360 | R W | no |

Los huecos (80099000, 800A3FE0-801FFFFF) son RAM sin contenido. Ademas estan los
bloques de hardware (1F800000+) y `GTEMAC` (20000000, macros GTE).

## Funciones (notes/functions_main0.csv)
- 1.822 en total. 153 son macros GTE en `20000000-20000263` (no son codigo del juego).
- 1.343 estan dentro de `.text`: 881 con nombre (librerias Psy-Q) y 462 `FUN_` sin nombre.
- 326 `FUN_` estan en `800E8214-80134E60`: RAM sin contenido en el EXE. **No son falsos
  positivos**: son codigo de **overlay** que el juego carga de disco en tiempo de ejecucion.
  43 funciones de MAIN0 llaman a 218 destinos distintos de esa zona. Habra que identificar
  que archivo del disco se carga ahi (candidatos: `X000.BIN`... en `AREAxx`).
- El codigo del juego por descompilar son esas 462 funciones (~208 KB).

## Relacion con la comparacion entre MAINx
`.text` casi no cambia entre MAIN0..8 (1-3%); lo que cambia (20-70%) es `.data`
(desde 800778F0). Es decir, el codigo es comun y cada MAINx lleva sus datos.
