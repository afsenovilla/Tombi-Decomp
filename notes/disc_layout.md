# Disco: Tombi! (PAL, version en espanol)

- Ejecutable principal: `SCES_013.31` (651.264 bytes, fecha 1998-06-19)
- SHA-1 del `.bin`: `ec50074b6fd98f611bae11e3de68d6e76ffdd6d4` (sin verificar contra Redump)
- `SYSTEM.CNF`: `BOOT = cdrom:\SCES_013.31;1`, `TCB = 4`, `EVENT = 10`, `STACK = 80200000`

## Directorios (extraidos con dumpsxiso 2.30)
`AREA00`..`AREA19`, `EXE`, `MOVIE`, `SOUND`, `SYS`, `SYSTEM`, `ZZZ`

`AREAxx` contiene `*.GAM`, `*.WFM` (fuentes `CFNT*`/`MFNT*`), `*.BIN` (`X000.BIN`...) y `*.000`.
`EXE/` esta pendiente de revisar: posibles overlays de codigo.

## EXE/MAIN0..8.EXE (analizados con tools/psxexe_info.py)
- Los 9 miden 563.200 bytes (0x89000 + cabecera de 0x800), se cargan en `0x80010000` y terminan en `0x80099000`.
- Mismo punto de entrada `0x8006C044`, salvo `MAIN5` (`0x8006C02C`).
- Solo el 57,6% de los bytes es identico en los 9. La parte variable crece hacia el final:
  ~1-3% distinto hasta `0x8006F800`, 20-70% desde `0x8007F800`.
- `MAIN6` y `MAIN8` son casi iguales (0,3%). `MAIN5` es el mas distinto (34-41% frente al resto).
- `SCES_013.31` se carga tambien en `0x80010000`, asi que parece un cargador que
  sustituye su propia imagen por uno de los `MAINx` (por confirmar).
