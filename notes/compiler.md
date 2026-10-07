# Compilador (matching)

**El juego se compila con GCC 2.7.2.SN.1 (Psy-Q, SN Systems)** + ASPSX 2.34 (DOS).
Verificado: `MulCos`, `MulNegSin` y `ObjApplyVelocity` salen idénticas byte a byte.
(Psy-Q 4.6 trae GCC 2.95.2 y NO coincide; 4.7 solo trae librerías.)

Los .EXE son de DOS (extensor DJGPP): se ejecutan con **DOSBox** en modo headless.
- Ficheros necesarios (locales, no se suben): `CC1PSX.EXE`, `ASPSX.EXE`, `CPPPSX.EXE`, `PSYLINK.EXE` en `/opt/psyq/new` (o `$PSYQ_DIR`).
- `apt install dosbox`.

## Uso
Cada función va en `src/<Nombre>.c` con cabecera `// FUNC <addr> <size> [MAIN0|X000]` y opcional `// FLAGS -O2 -G0`.
`tools/matchcheck.py [--mark] [--asm]` compila (CPPPSX -> CC1PSX -> ASPSX), extrae `.text` del OBJ,
enmascara relocaciones (hi16/lo16/jal) y compara con el binario del juego.
`--mark` añade `// MATCHING <addr> <size>`, que cuenta `tools/progress.py`.
Flags por defecto `-O2 -G0`; CPPPSX con `-undef -D__GNUC__=2 -DMIPSEL`.
