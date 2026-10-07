# Overlays (codigo cargado en tiempo de ejecucion)

## AREA00/X000.BIN (343.888 bytes)
- No es un PS-X EXE: no tiene cabecera. Primer dword = `0x1D` (29), luego una tabla de
  punteros absolutos (`0x800E9ECC`...) que apuntan dentro del propio fichero.
- **Direccion de carga: `0x800E8028`** (fin `0x8013C0F8`). Evidencia: con esa base, los
  punteros de la cabecera caen en codigo valido (`jal`/`j` + `nop`, offset 0x1EA4) y 46 de
  los 218 destinos que MAIN0 llama en esa zona quedan justo tras un `jr ra; nop`
  (con bases vecinas salen 0-4).
- Los otros ~172 destinos que llama MAIN0 caen en zonas de datos de X000: pertenecen a
  otro overlay que se carga en la misma zona (por identificar; otros `X*.BIN`).
- `AREA00/A000.GAM` (178 bytes): cabecera `"GAM\0"` + tablas pequenas. Probablemente
  parametros del nivel, no codigo.

## Importar en Ghidra
Importar `X000.BIN` como **Raw Binary**, lenguaje MIPS 32 little endian,
direccion base `800e8028`.

## Que hace X000.BIN (analizado con su decompilado, 531 funciones)
- Es codigo de comportamiento de objetos del nivel (enemigos/escenario): llama
  constantemente a las funciones de animacion de MAIN0 (`AnimAdvance` 252 veces,
  `AnimLoadDuration` 171, `AnimJump` 135), a `Rand`, `SfxPlay`, `ObjAlloc` y a las tablas
  trigonometricas.
- Llama a 156 direcciones de MAIN0; **71 de ellas no eran funcion en MAIN0** (Ghidra no las
  creo porque en MAIN0 nadie las llama). Lista en `notes/main0_missing_functions.txt`.
  Las mas llamadas: 8003FD78 (64 veces), 8001FD94, 8001E5F4, 8003FACC.
