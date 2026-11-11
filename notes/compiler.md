# Compiler (matching)

> **UPDATE 2026-11-09: the prototypes use the same CC1PSX as retail.** PrePre 8 (1997-06), the JP demo and the 1998-02-03 beta
> rebuilt with old-gcc 2.6.0 and the CC1PSX of Psy-Q 3.6 (2.7.2.SN.1) and 4.1 (cygnus-2.7.2-970404), PrePre 8 also with Psy-Q 3.5 (GCC 2.6.0) and
> old-gcc 2.6.3: all lose functions and gain none; only gcc 2.7.2 (= Psy-Q 4.0 CC1PSX) reproduces them. PrePre 8 differs in the
> assembler only (ASPSX 2.34, `// ASPSX.PCPX_96084 2.34`). Figures in docs/PREPRE8.md "Matching target". The mkst/esa
> archive also has psyq3.3 (GCC 2.6.0 + ASPSX 2.21), psyq3.5 (GCC 2.6.0 + ASPSX 2.34) and psyq4.5 (ASPSX 2.81).

> **UPDATE 2026-11-08: `bash tools/setup_psyq.sh` installs the reference toolchain in a fresh container** (wine32,
> dosbox, and the Psy-Q binaries from the public archive decomp.me uses, github.com/mkst/esa, into /opt/psyq; never
> committed). The CC1PSX that reproduces the game is the **Psy-Q 4.0** one of that archive (`/opt/psyq/cc40`,
> matchcheck's default now): its "4.3", "4.4" and "4.6" CC1PSX give a different frame for ObjGravityStep. With it
> 126 of 129 sampled MATCHING files pass matchcheck; the 3 others are `// AS gnu` pad-library files (GNU as, ncheck
> only). matchcheck now preprocesses with the native old-gcc `cpp` (DOS CPPPSX only resolves 8.3 header names, so
> gamestate.h and include/tomba/ were silently missing); `NATIVE_CPP=0` restores the DOS CPPPSX.

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
