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
