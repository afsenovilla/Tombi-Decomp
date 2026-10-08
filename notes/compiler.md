# Compiler (matching)

> **UPDATE: the game's compiler is the `CC1PSX.EXE` from Psy-Q 4.3** (Win32, run under Wine).
> With the 4.3 CC1PSX + ASPSX 2.86 (4.6), 263 functions match; the DOS CC1PSX 2.7.2.SN.1 we
> used before had a different scheduler (prologue in a different order) and blocked dozens of functions.
> `matchcheck.py` uses it by default (`/opt/psyq/cc43/CC1PSX.EXE`, or `CC1_WINE=...`; `CC1_WINE=0` = DOS).
> CC1PSX 4.4 is worse (6 of 206); ASPSX 2.56..2.86 give the same result (2.86 chosen).

> **Fast native checker: `tools/ncheck.py`** (no Wine or DOSBox): old-gcc `gcc-2.7.2-psx` cc1 + maspsx
> (emulating ASPSX 2.86) + GNU as, installed by `tools/setup_native.sh`. It reproduces 628 of the 636 verified
> functions in seconds. Use it to iterate; `tools/matchcheck.py` (CC1PSX 4.3 via Wine + ASPSX 2.86)
> remains the reference check. Functions ported from psx_tomba include `include/tomba/` headers and
> are verified with ncheck.


**The game is compiled with GCC 2.7.2.SN.1 (Psy-Q, SN Systems)** + ASPSX 2.34 (DOS).
Verified: `MulCos`, `MulNegSin` and `ObjApplyVelocity` come out byte-for-byte identical.
(Psy-Q 4.6 ships GCC 2.95.2 and does NOT match; 4.7 only ships libraries.)

The .EXE files are DOS programs (DJGPP extender): they run under **DOSBox** in headless mode.
- Required files (local, not committed): `CC1PSX.EXE`, `ASPSX.EXE`, `CPPPSX.EXE`, `PSYLINK.EXE` in `/opt/psyq/new` (or `$PSYQ_DIR`).
- `apt install dosbox`.

## Usage
Each function goes in `src/<Name>.c` with the header `// FUNC <addr> <size> [MAIN0|X000]` and an optional `// FLAGS -O2 -G0`.
`tools/matchcheck.py [--mark] [--asm]` compiles (CPPPSX -> CC1PSX -> ASPSX), extracts `.text` from the OBJ,
masks relocations (hi16/lo16/jal) and compares against the game binary.
`--mark` adds `// MATCHING <addr> <size>`, which `tools/progress.py` counts.
Default flags `-O2 -G0`; CPPPSX with `-undef -D__GNUC__=2 -DMIPSEL`.

For fast iteration: `python3 tools/ncheck.py [--score] [--asm] [--mark] src/<Name>.c` (setup: `tools/setup_native.sh`).
