# Matching guide (for working autonomously)

Goal: write C in `src/<Name>.c` that, compiled with the original toolchain, produces **exactly**
the bytes of the function in the game. Verification: `tools/matchcheck.py` (masks relocations).

## Toolchain (already configured in matchcheck)
- **CC1PSX from Psy-Q 4.3** (GCC 2.7.2.SN.1, Win32 via Wine): `/opt/psyq/cc43/CC1PSX.EXE`, default flags `-O2 -G0`.
- CPPPSX 2.7.2 (DOSBox) for preprocessing; **ASPSX 2.86** (Wine) for assembling.
- The DOS CC1PSX we used at first has a different scheduler: do not use it (`CC1_WINE=0` for tests only).

### Fast native checker (`tools/ncheck.py`)
- old-gcc `gcc-2.7.2-psx` cc1 + maspsx 2.86 + GNU as, no Wine or DOSBox. Install with `tools/setup_native.sh`
  (old-gcc in `/opt/oldgcc`, maspsx in `/opt/maspsx`, `binutils-mipsel-linux-gnu`).
- Runs maspsx with `--expand-div` (signed `/` and `%` get ASPSX's `break 7/6` checks). Reproduces 989 of the 994
  verified functions in seconds: **use it to iterate**
  (`python3 tools/ncheck.py [--score] [--asm] src/<Name>.c`).
- `tools/matchcheck.py` (CC1PSX 4.3 via Wine + ASPSX 2.86) remains the **reference check**.
- Functions ported from psx_tomba include `include/tomba/` headers and are verified with ncheck.

## Work cycle
1. `python3 tools/fn.py <addr>` → assembly (capstone) + Ghidra decompilation.
2. Write `src/<Name>.c`. Mandatory header `// FUNC <addr> <size> [MAIN0|X000]`; optional `// FLAGS -O2 -G0 ...`.
   Objects: `#include "TOBJ.H"` (include/tobj.h). Globals: `extern T DAT_xxxxxxxx;` (the name does not matter).
3. Iterate with `python3 tools/ncheck.py src/<Name>.c [--score] [--asm]`, then confirm with
   `WORK=/opt/psyq/w_<id> WINEPREFIX=/opt/wine python3 tools/matchcheck.py src/<Name>.c [--asm]`
   (`--asm` leaves the .s in `build/`; `--score` gives the distance, 0 = identical). One WORK dir per agent.
4. On `MATCH`: `... matchcheck.py --mark src/<Name>.c` (adds `// MATCHING addr size`); delete `src/wip/<Name>.c` if it existed.
5. If it does not match after ~6 genuinely different attempts: leave the best one in `src/wip/<Name>.c` (without MATCHING) and move on.

## Before starting a function
- **Check the size**: Ghidra cuts functions short when they have a jump table or a `j` to the epilogue. The real end is the
  `jr $ra` followed by the next `addiu $sp,-N`; put the real size in `// FUNC` (FUN_80121fe0: 312, not 144).
- Fragments (starting with an epilogue, using `$sN` without loading it, a stray `j`, <8 B) are not matchable: skip them.
  `notes/todo_match4.csv` lists the pending game functions with splat's boundaries (better than Ghidra's).
- **splat boundaries can also be wrong**: it cuts at jump tables (read the table from the binary: for X000 the file offset
  is `addr - 0x800E8028`) and sometimes merges several small functions. If the next entry is the rest of the same
  function, match the whole range (func_80122C10 = 172 + 600 B; func_8011D9D8 = 856 B).
- **Brute force scheduling differences**: when only instruction order differs, a small script that tries scalar `X`
  vs array `X[]` per global plus permutations of the statements involved, scored with `ncheck --score`, usually
  finds the match in seconds (func_8002AC68, func_8004C1EC).
- Old wips written for the DOS compiler (char* and hand-computed offsets): it is usually worth rewriting them from scratch
  with TObj/switch; remove `-fno-delayed-branch` and `-g` from their FLAGS.

## Recipes by symptom (GCC 2.7.2 / CC1PSX 4.3)

### Order of global loads relative to stores (the most common)
gcc 2.7 decides aliasing via MEM_IN_STRUCT: a **scalar global** at a fixed address never conflicts with an
"in struct" access (`o->field`, array), so the scheduler **hoists** its load above those stores.
- The game reads the global **after** the stores → declare the global as an array and use `X[0]`
  (`extern int X[];`), or as a struct member (`extern G DAT; DAT.y`), or symbol+offset. This also makes
  a **store** to a global keep its order. (If the array is used 2+ times in the block, its address goes into a register.)
- The game **hoists** it → keep the scalar global and access the object through struct fields (TObj), not `*(short*)(p+off)`.
- Conversely: stores with `char*` casts/raw offsets make gcc reload globals after each store; with TObj
  fields they stay in a register (ObjSpawn).
- For sequential reads through a global pointer use `*DAT_ptr++` (`p[1]` counts as in-struct and gets hoisted).

### Accesses with the address in a register (`lui r; addiu r,lo; lhu 0(r)`)
- A single such access = **volatile at the point of use**: `*(volatile unsigned short *)&DAT` (a volatile extern is not enough),
  or `extern volatile unsigned short DAT[]` + `DAT[0]` (controller at `DAT_8009d670`). A literal address gives `lui+ori` and does not work.
- Two such reads that are not merged = `volatile unsigned short *k = &DAT;` used twice, assigned inside the branch where it is used.
- Base in an s-reg for several fields across calls = local pointer (`unsigned short *g = DAT; g[0]/g[1]`,
  `int *g = &G;` with the first read early in the source).
- Scratchpad struct with base in an s-reg (`lui/addiu sym+0x30`, the rest as `s1-0x30`): `extern S DAT_1f8000c0;` and fields.
- Two `lw` of the same field with no store in between: make ONLY the second read volatile (`*(T *volatile *)&s->f` or
  `*(volatile int *)&o->f`).

### Operand order (`addu`, `or`)
- ASPSX expansion: **literal** address `*(int *)(k + 0x801fd80c)` → `addu at,reg,at`; **extern symbol** → `addu at,at,reg`.
- `x << 2` instead of `x * 4` puts the base first; `(char *)o + idx + 0x64` versus `&arr[idx]`; base as `int`
  and `(T*)(x + code) + 1` keeps the source order. `a + (b+0x10)` ≠ `(a+b)+0x10`.
- `or` in the wrong order: use a different variable as the destination (`m = btn & 0xff0f; btn = a | m;`).
- Pattern `sll; addiu K; addu D` = `(int)&((struct{char pad[K]; int a[1];}*)D)->a[i]`.
- With CC1PSX 4.3, `int + (signed char)field` comes out with swapped operands and the source order does not fix it.

### Parameters (visible in the prologue)
- `move sN,a0` in the delay slot of the first `jal` = `unsigned char` parameter. Duplicate copies (`move s6,s1`),
  `move v1,a1 ... move s1,v1`, or `sll rX,aN,16` without `sra` = `short` parameter (write `x << 16` at the use).
- If `a0`/`a1` is not loaded for a call but still holds the parameter, the function **receives that argument**:
  pass it (`ObjSetAnimFromTable(o)`) and the register allocation gets fixed.
- `move a3,a0; move t0,a1` at the start of a leaf = an expanded `static __inline__` (or a0/a1 used as temporaries).
- An `int` parameter with `(short)` only at the use avoids the early extension. `char *` is read with `lbu`: use `signed char *` for `lb`.
- Address constant passed to a function: `extern char DAT_x[]` (not the literal).

### Branches, switch and shared tails
- `slti`/`bltz` on a value = comparison tree of a **switch**. Separate cases that go to the same body
  ≠ `case 1: case 2:`; add an impossible `case` (`case 99: break;`) or an empty `case 1: break;` to rebuild the tree.
  A 4-case sparse switch that is a jump table in the game: add a dummy `case N:` to force a tablejump.
- **Shared tail** (`j L` with the value in the delay slot and a common `sh/sw`) = the full statement written in
  **each branch** (cross-jumping): `if (a) o->x = 0xf0; else o->x = 0x50;`, a full call per branch, division in each branch.
- A case that jumps into the middle of another (with `sw` in the delay slot of the `j`) = `goto common` with a local assigned in both.
- Fallthrough without `j` = case without `break`. Identical consecutive blocks = duplicated code in the source.
- `bgez; negu; j; sra` = `x < 0 ? -x >> 16 : x >> 16` (shift inside each branch).
- `addiu` in the delay slot of a branch before the if = the full expression is computed before the if.
- `if (call() && inline_attr(o)) return 1;` for a call==0 that jumps straight to the next check.
- Temporaries shared between cases end up in t0/a3: declare them local to the block.

### Loops
- gcc rotates `for(;;){...; if(--n<1) break; ...}`. If the game has the test at the top and a `j` to the start: `loop:` + `goto loop`.
- `lh v0; move sN,v0` with a test on v0 and then `sll v0,n,16; bnez` = local `short n` with `while (n != 0)`.
- Loop pointer whose init comes after the hoisted constants = `e = &arr[i];` inside the loop; `-fno-strength-reduce` removes surplus givs.
- Two consecutive loops with "the same" pointer in different registers = two different variables.

### CSE, constants and types
- Unnecessary `andi x,0xffff` before a bit test = `t % 32 == 0`; use `(t & 0x1f) == 0` if it does not appear.
- A subtraction that gcc folds (`y-(v-8)` → `(y+8)-v`): temporary `unsigned short b = v - 8;` only in that branch.
- Field read after an `sh` of a constant chosen by a condition = two stores in if/else.
- `x ? 2 : 1` into a field = `if (c) *p = 2; else *p = 1;`. 0/1 results without `xori/andi` = inline returning `short`.
- Read the field into an `int` temporary before a truncated subtraction to keep `lhu`; `x %= 10` reuses the quotient of `short q = x/10`.
- Negative `char` constants: `*(signed char *)&o->field = -30;`.
- Hex values like `0x9e-0x74` form a single pp-number because of the `e-`: write the decimal value.
- `addPrim` with masks `0xff000000/0xffffff` = bitfield `struct {unsigned addr:24; unsigned len:8;}`.
- Same global read with `lhu` and with `lh`: two externs with different types and names (the name does not matter).

### Prologue, epilogue and frame
- With CC1PSX 4.3 the epilogue `j $31; addu $sp` (filled delay slot) does come out when only `$ra` is saved;
  with saved s-regs (library 0x8006xxxx) it cannot be reproduced: leave those in wip.
- If the game loads a global before `addiu sp` or hoists a constant: store them in locals before the if (`n = g; c = 2;`).
- Frame without saves: an unused `char pad[16];` (16 B) or `char pad;` (8 B). 0x38/0x10 frames without saves are usually inlines.
- Declare locals at the start of the block (in the middle of a block gcc 2.7 has been seen to drop a statement).
- Some functions need `// FLAGS -O1 -G0` (base-reg `sw x,0(v0); sw y,4(v0)`) or `-O2 -G0 -fno-schedule-insns`.

### More recipes (third batch of agents)
- **Inlines everywhere**: copied box/collision code is a `static __inline__` helper; writing it as an inline fixes
  register allocation and tail merging (8012C8EC, 80029CB4). `addu v0,zero,zero` in the delay slot of each failing
  branch followed by `bnez v0` = inline returning a boolean (8012ED60). An inline returning 0/1 whose last test copies a
  variable first = a nested inline, e.g. `fin(r, D - y + 0x30)` with `short r` in the caller. The OT-insertion helper is
  an inline with 5 parameters like `FUN_8004fdc8(a, b, c, d, e)` (fixes the frame size).
- **Loops**: when the game does not hoist constants, write `label: ... if (n) goto label;` instead of do/while; a constant
  the game does hoist = a local assigned before the label (func_800491F0 170 -> 22).
- **Scratchpad addresses** used several times: declare an extern symbol (`extern short D_1F80019E;`) instead of a literal,
  which gcc CSEs into a register with `li/ori`; relocations are masked so the name does not matter.
- **Same global, two externs**: a second scalar name makes gcc reload it after field stores; `[0]` loads stay after field
  stores while scalars hoist; read into temps before the first store to fix the order of two loads (func_80134404).
- **Duplicated case tails** kept separate in the game = the full stores written in every case, not a variable stored after
  the switch. A comparison result computed per case and branched once (`slti; j L`) = a `short` flag + `goto tail`.
  A case that falls into another case's final store: invert the `if` in the other case so the store comes last.
- **Shared tail that is a call**: the full call is written in each branch. One store breaking cross-jumping: use a local
  copy of the pointer for that store only (`S *p = o; p->f = x;`).
- **Extra `move` copy** = an extra pseudo: copy into a temp right before use, give an inline parameter a narrower type,
  assign a ternary to an `int` before passing it as `short`, or declare the variable at function scope (8004E41C).
- **Player struct `D_8009C330`**: a local `p` only for the stores that reuse the old load; later stores as
  `((PL *)D_8009C330)->field`; raw byte stores where the game reloads the pointer. A `sb` through a global pointer
  forces a reload afterwards: order statements so each `sb` closes its group.
- **Small things**: `abs()` gives `bgez; nop; negu` exactly. `case 0 ... 12:` gives `slti 13; bltz`. An 8-byte frame with
  no saves can come from an explicit `(short)` cast on an expression. Pad frames are inconsistent: try several sizes.
  `int + (signed char)field` with swapped operands: declare the int operand `short`. A loop rereading a flag set by an
  interrupt (`D_8009BCDC`) is `extern volatile int`. `(short)(o->d30 - 2) + r` stops gcc reassociating a constant.
- **Hand-written GTE routines** (80022ABC, 80022630): `gte_*` asm macros, `stsxy3` with fixed offsets, the list parameter
  advanced as `long *f; n = *f++`, and `addPrim` as `t = *ot; *ot = p; p->tag = t | len;`.

### Matching debt (workarounds to revisit)
Functions that match only thanks to a compiler hint rather than plain C; keep them to a minimum:
- `register int r asm("$17")`: func_800428C0. `__asm__ volatile("nop")` before a loop: func_8006A200.
- Constant loaded as the address of a symbol (`D_31FFFF`): func_8005D4EC.
- Second extern name for the same global: func_8002A0FC, func_8002A258 (and others, see the source comments).
- Asm-only thunk: thunk_FUN_8011a628. Files holding several small functions merged by splat: func_8011D204 (6),
  func_8011F288 (wip, 3).

## Git rules (several agents work at the same time)
- Only `master`, no branches or PRs. Small commits every 3 matches.
- The index is shared: **`git commit -m "..." -- <your paths>`** (a plain `git commit` takes whatever others have staged).
- `git pull --rebase origin master`; if the conflict is only in `src/wip` or `notes`: `git rebase --abort` and
  `git merge -X theirs origin/master`. Do not use `git stash -u` (there are untracked files from other agents).
- Use your own temporary directory (`/tmp/<id>`): the session scratchpad is shared.
- Do not rewrite `include/tobj.h` (create `include/<name8>.h`). Never commit the SDK, game binaries or raw decompilations.

## History (resolved)
- ASPSX 2.34 (DOS) emitted `ori` for `li`; ASPSX 2.86 is used instead. Versions 2.56/2.77/2.81 add nothing.
- The DOS CC1PSX placed `addiu sp` after the first global load; CC1PSX 4.3 fixes it (with the array recipe).
- `tools/permute.py` (random mutations, CPU only) got 1 hit out of 32 cases: only useful for type differences.
- `tools/ncheck.py` (native old-gcc + maspsx) replaces Wine/DOSBox for iteration; matchcheck is still the final check.
