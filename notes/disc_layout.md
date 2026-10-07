# Disco: Tombi! (PAL, version en espanol)

- Ejecutable principal: `SCES_013.31` (651.264 bytes, fecha 1998-06-19)
- SHA-1 del `.bin`: `ec50074b6fd98f611bae11e3de68d6e76ffdd6d4` (sin verificar contra Redump)
- `SYSTEM.CNF`: `BOOT = cdrom:\SCES_013.31;1`, `TCB = 4`, `EVENT = 10`, `STACK = 80200000`

## Directorios (extraidos con dumpsxiso 2.30)
`AREA00`..`AREA19`, `EXE`, `MOVIE`, `SOUND`, `SYS`, `SYSTEM`, `ZZZ`

`AREAxx` contiene `*.GAM`, `*.WFM` (fuentes `CFNT*`/`MFNT*`), `*.BIN` (`X000.BIN`...) y `*.000`.
`EXE/` esta pendiente de revisar: posibles overlays de codigo.
