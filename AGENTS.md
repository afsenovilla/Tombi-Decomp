# AGENTS.md: working on Tombi-Decomp with an AI coding agent

This file is for any coding agent (Claude Code, Codex, Gemini, others). The project rules, the user's preferences
and the build cheatsheet are in [CLAUDE.md](CLAUDE.md); read it first, it applies to every agent. The current state
and the open tasks are in [docs/STATUS.md](docs/STATUS.md); the list of what is left to match, per version, is in
[docs/REMAINING.md](docs/REMAINING.md). This file adds what an agent needs to *match functions*.

## What "matching" means here

Every version of the game (PAL SCES-01331 = reference, JP retail SLPS-01144, USA SCUS-94236, and the demos) is
built from ONE `src/` tree (`tools/versions.py`). A function is done when our C, compiled with the game's compiler,
gives the exact bytes of the disc. Progress: `tools/progress.py` (PAL, README/docs/PROGRESS.md),
`tools/build_report.py --version <v>` + `tools/progress_jp.py` (other versions; ~10 min).

## Checkers

| What | Command | Notes |
|---|---|---|
| PAL, fast | `python3 tools/ncheck.py [--score] [--asm] [--mark] src/X.c` | native gcc 2.7.2 + maspsx, seconds |
| PAL, reference | `python3 tools/matchcheck.py [--mark] src/X.c` | Sony CC1PSX via Wine; install with `bash tools/setup_psyq.sh` |
| JP / other versions | `python3 tools/jpcheck.py --strict --norm FILE PROG TARGETNAME` | then again WITHOUT `--norm` to check relocation names |

- The reference CC1PSX is the **Psy-Q 4.0** one of the public mkst/esa archive (what decomp.me uses); its "4.3",
  "4.4" and "4.6" builds produce different code. The native gcc 2.7.2 gives the same code as CC1PSX 4.0, so the
  permuter on the native compiler searches the same space (no need for a Psy-Q permuter).
- `ncheck --asm` / `matchcheck --asm` write into the repo's `build/`: move or delete those files afterwards.
- JP targets carry the `_slps01144` suffix when splat named them so (`ls build/slps01144/target/PROG/`).

## Install and verify (JP and the other versions)

1. Draft passes `jpcheck --strict --norm` (prints IDENTICAL).
2. Without `--norm`, every relocation name is the retail one or a name `config/<ver>/symbol_addrs_*.txt` maps to
   the same address. PAL-only names (PAL `D_`/`FUN_` addresses, PAL static tables) must be replaced by the retail
   names, or the report scores the function below 100 %.
3. Own file `src/[x0nn/]func_ADDR_<ver>.c` with `// VERSION <SERIAL> <prog> <addr> <size>`, `// FLAGS ...`, the
   body inside `#if defined(VERSION_<SERIAL>)`. Never two `// VERSION` lines for one address (build_report builds
   only one). If you edit a shared PAL file, the PAL part must still pass `ncheck`.
4. Symbol line in `config/<ver>/symbol_addrs_<prog>.txt`; one name per address (splat fails otherwise). **Check
   the size against the next symbol**: a function that is IDENTICAL in jpcheck but 99.x % in the report almost
   always has a wrong size (a missing trailing `jr ra` delay-slot `nop`, or a size that runs over the next
   function, e.g. CdFlush).
5. Rebuild the report and check the function is 100.0 in `build/<ver>/report.json` before committing.

## gcc 2.7.2 tricks that fixed real diffs in this project

Register allocation / scheduling (most of what is left everywhere):
- **Unused extra locals** `int u1 = (int)p; ... int u11 = (int)p;` add pseudos and lift local-alloc's quantity
  limit (fixed a block-copy loop spilling to t3/t4, main 80026B7C).
- **Joint barrier** over two just-loaded values `asm("" : "=r"(k), "=r"(j) : "0"(k), "1"(j));` keeps another
  value live across them, so it lands in a higher register (x10 80119D9C / x14 80118580: `o` in a3).
- **`register T x asm("$N")` on one variable** when the rest follows (printInfoMessage, drawBalloonFrame); then
  `x += d; y = x;` to reuse the same register for a sum.
- An **extra pointer copy** (`q2 = q;`) somewhere else in the function changes allocation priorities.
- **One pointer variable reused** for several temps, where retail clearly does that.
- **Declaration order** of locals; `short`/`unsigned short`/`char` temps; a constant held in an asm-barriered int
  (`{ int k = 2; asm("" : "=r"(k) : "0"(k)); a = k; b = k; }`).
- `addu` operand order: route the sum through a short temp, or use the unsigned-short copy of an operand.
- `asm volatile("" : "=r"(t) : "0"(t))` right after `T *t = TABLE;` hides the constant from cse (it had swapped
  base and index of an addu, D0SPSUB 800E89F8).
- `static __inline__` helpers instead of macros; `a && b` written as early returns.
- `asm("" : "=r"(x) : "0"(x))` barriers: `"+r"` is NOT supported by gcc 2.7.

Shape / codegen:
- Switches: empty `case N:` labels and the case order decide the jump table; read the table from the disc file.
- Scratchpad: one `extern` per address (`D_1F8001C6`), or top-level `asm(".set NAME,0x1f8001xx");` absolute
  symbols, so every access reloads with its own `lui`; `lui/addiu` vs `lui/ori` depends on the form.
- A global whose load gets hoisted above stores: a one-field struct, or `extern T X_[]; #define X X_[0]` (or just
  declare it as an array and write `X[0]`).
- USA (NTSC) vs PAL: screen height 8 lower (0x80 -> 0x78, 0x100 -> 0xe0), play time in 60 Hz frames, PAL
  scratchpad bytes that are RAM globals in USA; USA often equals the JP branch.
- FLAGS: some JP main functions are built without `-fno-strength-reduce`.
- Many "JP-only" functions are a PAL file almost as is (pad word `D_8009F200` with bits x0x100, wider windows,
  one dropped call, swapped sound ids): find the PAL twin by size and callees first.

## The permuter

decomp-permuter in the session scratchpad (`tools/jp_permute_setup.py FILE PROG NAME DIR`, then
`python3 permuter.py DIR --stop-on-zero -j1`). Short runs, hand-finish the best output, reseed. It cannot parse GTE
inline asm. A score above 0 can still be instruction-identical (only relocation names differ).

## Working in parallel

Several agents can work at once (one function set each), on copies outside the repo. Only one process should
edit and commit the repo; agents report a file path and what fixed it. Run background jobs with the tool's
background option, not a subshell `&`; never `pkill -f` (kill by PID).
