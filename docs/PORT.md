# Native port plan (PC first, Android later)

This is the plan for a native build of Tombi! made from the matched C in `src/`, plus a report on the first
prototype in `port/`. The matching build (`tools/build_full.py`) is not touched. The port only **reads** `src/` and
the matching build's outputs, and it never ships game data. The player supplies their own disc image.

## 1. Status of the prototype (milestone M0)

`port/` holds a CMake project. It compiles **all MAIN0 and X000 game C plus the SYSTEM overlays (1724 files)** for
x86 (i386) Linux. It links them with a small runtime and Psy-Q replacements, and the result runs headless from boot
through the title screen into gameplay in AREA00 (see "Milestone M1" below):

```
python3 tools/build_full.py -j 4                # once: the port takes retail addresses from it
cmake -S port -B build/port -G Ninja && cmake --build build/port -j 8
build/port/tombi --frames 500                   # from the repo root; reads game/MAIN0.EXE + game/disc/Tombi.bin
#   options: --exe PATH --disc PATH(.bin raw 2352, .cue naming the .bin, or .iso) --frames N
#            --realtime [--hz 50|60] (paced; 50 = PAL default, 60 = NTSC timing) --autostart N -v [-v]
#            --pad-script FRAME:BUTTONS[:LEN[xCOUNT/PERIOD]],...  --dump-frames DIR --dump-every N --pipe-frames FIFO
#            --no-interp --interp ADDR --trace-interp
#            --window [--viewer PATH]   SDL2 window + keyboard/controller (needs build/port/tombi-view)
#            --save FRAME:PATH --load PATH   save states (same binary)
#            --save-dir DIR   memory cards tombi_mc0.mcd / tombi_mc1.mcd (default: next to the disc image)
#            --menu FRAME:N   debug: open menu screen N during gameplay (4 = save screen, 5 = load screen)
#            -v -v prints every pad read ("[pad] frame F: game reads MASK"), -v -v -v the player x,y every vblank
build/port/tombi --frames 3600 --dump-frames shots --dump-every 100 \
    --pad-script 1000:start,1300:cross:3x45/30,3200:right:400   # title -> new game -> intro dialogue -> walk
```

What it does today, measured on the PAL Spanish disc:

1. It maps PS1 RAM and loads the user's `MAIN0.EXE` image, which supplies all retail data and BSS layout. It
   opens the disc image.
2. It runs the game's `main()` and the whole init sequence: `initGlobalState`, `initGraphics`, `SoundInit`,
   `MemCardInit`, `joypadInit`, `initTasks`, `startTask`, and the VBlank event.
3. It enters the main loop. Tasks run as coroutines (`ThreadTableRun` → `ChangeTh`).
4. The boot task `func_80019244` (states 9→10→11→0→1) runs to completion. The game loads `SYS/LD0SYS.BIN`, the
   boot logo `SYSTEM/A09400.GAM`, `SOUND/SND_INI.WVD`, and `SYSTEM/A0000x.GAM`, all through its own `cdLoadTask`
   and our sector reader. The boot logo fades in and out over about 450 frames, about 1,500 GPU packets per
   100 frames walk through the ordering tables, and the LZ decompressors and VRAM uploads run.
5. **The intro movies are skipped** (no MDEC yet): `StGetNext` hands `MovieDecodeFrame` a sector header whose
   frame number is past the end, so `moviePlayerTask` takes its own "movie finished" path (`CdMix`,
   `StUnSetRing`, `exitTask`). The four boot movies are skipped this way (`--play-movies` restores the old
   wait-forever behaviour, for MDEC work).
6. **The title screen runs natively** (frame ~800): `titleSequenceTask` loads `SYSTEM/O0PTSUB0.BIN` and calls
   its entry points, which are now the overlay's own C (overlay set, §2.2). M0 stopped here.
7. **Start** (`--pad-script 1000:start`) starts a new game: `gameMainTask` loads AREA00 (`X000.BIN` and the area
   data, ~1100 sectors), the first room renders at frame ~1150 with its animated objects, the intro dialogue
   ("Hey, you with the pink hair!") runs and advances with Cross, the HUD appears, and the pad moves Tombi
   (walking right scrolls the camera). The run is deterministic (same inputs, same frames).

### Milestone M1: title screen and AREA00 gameplay

What made it work (all in `port/`, `src/` untouched, the matching build stays byte-identical):

- **Overlay set** (`gen_port.py`): `PORT_PROGS` now also lists `O0PTSUB0 G0OVER D0SPSUB D0SPSUB2 D0SPSUB3 I0NFO
  I0NFO2 I0NFO3` (tools/sysovl.py). Their C is compiled with every function the file defines renamed
  `<PROG>_<name>` (they overlap X000's addresses) and listed in `port_ovlmap` with the overlay's header id (the
  first word of the file: 0x31..0x38; X000 and all area files have 0x1D). `port_lookup` dispatches a call into
  0x800E8028..0x80140000 to the resident SYSTEM overlay's table when that id is at 0x800E8028, else to X000.
  MAIN0's calls into the overlay region are call thunks, so they dispatch with no change to MAIN0 code.
  Asm-only overlay functions can be written by hand as `<PROG>_<name>` in `port/runtime/ovl_*.c` (they join
  the table automatically): `O0PTSUB0_func_800E9F5C` (title menu glyphs) is the first.
- **MIPS interpreter fallback** (`runtime/mips_interp.c`, ~300 lines): any retail code address without native
  code (asm-only MAIN0 functions, unmatched overlay functions, per-area code of X001..X019) runs as the retail
  machine code where it lies in RAM. A jal to native code calls it with cdecl ($a0-$a3 plus four stack words);
  Psy-Q code without a shim is never interpreted (it would poke hardware) but stubbed. In the AREA00 run it
  handles `func_800416B8` (wall probe, ~300 calls per 3,000 frames). `--interp ADDR` forces an address through
  it, which checks a hand translation (the title glyphs render identically both ways); `--no-interp` turns it
  off. Overlay code that the game loaded is interpreted correctly by construction, so this is also the
  fallback for area overlays until their own C is compiled in (see "Missing" below).
- **Hand translations of hot asm-only MAIN0 functions** (`runtime/main0_handwritten.c`, from m2c drafts checked
  against the asm): `func_8002CF40` (message box object, 0xD10 bytes) and `func_800406E8` (floor probe).
  Remaining asm-only MAIN0 code: `func_800416B8` (1520 bytes), `func_8004AE7C` (1496), `func_80038184` (180),
  plus a few library-side ones (`func_8006A718`, `func_8006BAF8`, `func_8006BC08`).
- **Register-carried arguments, automatic** (`gen_port.py forward_short_calls`): a call passing fewer arguments
  than the callee's definition gets `PORT_ARG(i)` = the caller's own incoming argument i (110 call sites). This
  is what the R3000 code does when the caller leaves $aN untouched, and it works through chains of dispatchers
  because on i386 the slot is what the caller's caller pushed. Where $aN holds something else the fix goes in
  `port/fixes.txt` first (the pass then sees a full call): `updateAreaScreenEffect` (passes `&PLAYER`) and the
  cat-2/cat-4 collision callbacks of `collideCat2WithCat4Objects` ($a1 = the other object). Other wrong guesses
  will show up as crashes in other areas; the SIGSEGV handler now prints the native return addresses on the stack
  (`addr2line -f -e build/port/tombi ...`).
- Generator fixes uncovered by the newer src/: symbol assignments in top-level asm (`FILE_LINK_SIZES =
  FILE_LINKS + 4`), address-named `static` tables (moved into C by the data work) bound to their retail address,
  data names inside the overlay region, a GTE macro temp-name clash; `loadTIM` writes through a NULL CLUT rect
  (address 0 on the PS1; fixes.txt).
- `--pad-script` scripted pad input; `--pipe-frames` live raw-video output (below).

Screenshots of this run (PNG dumps): the title (frame 1000), the first room (1500), the intro dialogue (1300,
1800), the HUD (3000) and Tombi after walking right (3500).

**Missing / next:**

- **Sound**: libsnd/libspu are stubs (`SsSeqPlay`, `SsUtKeyOnV`, `SpuSetKey`, reverb ...), so no audio; nothing
  waits on sound status in AREA00, so gameplay is unaffected.
- **Area overlays other than X000**: per-area code (0x80115EA4..) runs in the interpreter when
  `GAME.selectedArea != 0` (calls dispatched through `port_lookup`). Native X000 shared-prefix code that calls a
  per-area function by name is still linked straight to X000's copy; the full fix is the plan below (compile
  each X0nn's own functions with an `X0nn_` prefix, route those names through thunks, track the resident area,
  e.g. from the load record or `GAME.selectedArea`).
- The decompiled-but-asm-only functions above (interpreted today), MDEC movies (skipped).
- The rest of the register-carried audit: `port/tools/arity_check.py` still lists the call sites; each guess
  that is wrong in some area needs a fixes.txt line.

**Video (M2 step 1, done):** `shims/gpu.c` is a software GPU. `DrawOTag` walks the OT like GPU DMA and decodes
every GP0 packet into an emulated 1024×512 VRAM: flat/gouraud/textured triangles and quads (top-left fill rule),
rectangles and sprites, lines and polylines, fill and VRAM copy, 4/8/15-bit texture pages with CLUTs, texture
windows, the four semi-transparency modes, modulation, drawing area/offset and the mask bit. `PutDrawEnv` applies
clip/offset/tpage and the `isbg` clear. The displayed frame is the `DISPENV.disp` rectangle (15 or 24 bit).

```
build/port/tombi --frames 900 --dump-frames shots --dump-every 25 [--dump-vram]   # PNG per 25th vblank
```

The boot logo ("Sony Computer Entertainment Europe / Presents") renders correctly, including its fade in/out.
The PNG writer is self-contained (stored deflate) because the runtime has no libc/zlib.

**Live view without a window toolkit:** `--pipe-frames PATH` writes every displayed frame as raw RGB24 on a
fixed 320x256 canvas, so any raw-video player is the window:

```
mkfifo /tmp/tombi.rgb
ffplay -f rawvideo -pixel_format rgb24 -video_size 320x256 -framerate 50 /tmp/tombi.rgb &
build/port/tombi --realtime --pipe-frames /tmp/tombi.rgb --pad-script 1000:start
```

**The window (M1, done): `--window`.** The game binary is i386 and freestanding (this host has the i386
`libSDL2-2.0.so.0` runtime but no 32-bit libc/crt or SDL headers), so the window is a separate host-native
process, `port/tools/view/tombi_view.c`, built as `tombi-view` when CMake finds SDL2 (`libsdl2-dev`); without
SDL2 the game still builds headless. `tombi --window` forks the viewer with three pipes: every displayed frame
(`"TBFR"`, width, height, RGB24 of the DISPENV area) on its stdin, the s16le 44.1 kHz audio stream on fd 3
(played through `SDL_QueueAudio`), and one u16 pad state per frame back on fd 4 (non-blocking, merged with a
`--pad-script` if any). `--window` implies `--realtime` (50 Hz); closing the window ends the game. Keys:
arrows, X/Z/S/D = Cross/Square/Triangle/Circle, Q/W/A/E = L1/R1/L2/R2, Return = Start, Backspace = Select,
F = 2x/3x, Escape = quit; an `SDL_GameController` maps to the same buttons. Verified under Xvfb (title screen,
Start and Cross through xdotool). `--viewer PATH` names another viewer binary.

```
build/port/tombi --window                    # title -> new game with the keyboard
build/port/tombi --window --load s.state     # from a save state
```

**Memory cards (M1, done): `shims/card.c`.** Two 128 KB raw card images, `tombi_mc0.mcd` and `tombi_mc1.mcd`
(the layout every emulator reads: 16 blocks of 8 KB, block 0 = header + directory), kept in memory and written
through to `--save-dir` (default: the directory of the disc image). A missing file starts as a formatted card.
The save/load screen (`D0SPSUB2`) uses the BIOS card file system, so the shim provides the BIOS event table
(`OpenEvent`/`TestEvent`/`DeliverEvent`, the card being the only event source), `_card_info`/`_card_load`/
`_card_clear` that complete at once by raising the "I/O end" event of both the SwCARD and HwCARD classes, and
`open`/`read`/`write`/`lseek`/`close`/`erase`/`format`/`firstfile`/`nextfile` on `bu00:` / `bu10:` (create must fail
when the file exists: the game's writer creates, closes, then reopens for writing). The game saves at save
points (`openMenuScreen(4)` from the "¿Guardar DATOS?" message box); `--menu FRAME:N` opens that screen at a
vblank for tests. Verified headless: a save written from the save screen
(`--menu 2000:4 --pad-script 1000:start,1300:cross:3x45/30,2200:cross:3,2300:left,2400:cross:3`) creates
`BESCES-01331TOMBA-00` (1 block, "SC" title frame), and the title's Continue (`--pad-script
1000:right,1050:cross,1450:cross:3`) lists it as "Lv.1 Village of All Beginnings".

**Pacing: `--realtime`, `--hz 50|60`.** One vblank every 1/hz s from the host clock (default 50, PAL; 60 gives
NTSC timing: the game logic runs 20% faster, as on an NTSC console). The audio mix takes 44100/hz samples per
vblank and the sequencer 1/hz s, so music keeps its tempo. `--window` implies `--realtime` and honours `--hz`.
Measured: 600 frames take 12.0 s at 50 and 10.0 s at 60.

**Save states: `--save FRAME:PATH`, `--load PATH`** (`runtime/state.c`). A state is PS1 RAM, the scratchpad and
every writable mapping of the static binary (the runtime's globals, software VRAM, SPU, GTE and the coroutine
stacks of `threads.c`), taken and restored only from the main loop's vblank wait on the main thread, where the
process stack holds the same frames in every run. After the copy the command line is parsed again and this
run's file descriptors (disc, pipes, viewer) are put back; the vblank count continues from the saved frame,
so pad scripts of a continued run use the same frame numbers as the run that saved it. A hash of the
executable refuses states of another build (a state restored into a rebuilt binary put variables at the wrong
addresses). States are 16 MB. They make experiments cheap: a 300-frame probe from a state takes a few
seconds instead of replaying the minutes of boot, title and loading, and `-v -v` logs the player every 10
frames.

The GL backend (VRAM as a texture, CLUT lookup in the shader) is step 2 and reuses the same GP0 decoder.

### How the prototype is built (`port/`)

| Piece | Role |
|---|---|
| `tools/gen_port.py sources` | Picks the src files of MAIN0+X000 (game code plus pure Psy-Q prim helpers) and writes filtered copies to `build/port/gen/src`. The filter translates the per-file `gte_*` asm macros into C calls on the software GTE, drops MIPS register pins and asm statements, fixes old-gcc lvalue casts, and applies `port/fixes.txt`. It also writes `funcmap.c` (retail address → native function), the per-file renames (the same extern name means different addresses in different files, exactly as `build_full.py` resolves them), and the include trees (`HEADERS tomba24`, DOS upper-case names). |
| `tools/gen_port.py link` | After compiling, resolves every undefined name against the matching build. A name for a native function becomes a linker alias. A name for retail data becomes `name = 0x8009xxxx;` (also for data a C file defines, so native code and the retail image share one copy). A name for code we lack becomes a weak asm **call thunk** that asks the runtime at call time and logs it as a stub. `port/shims` override thunks by name or by address (`FUN_80063ffc` → `SetGeomScreen`). |
| `runtime/sys_linux_i386.c` | A freestanding runtime (raw syscalls, printf, 64-bit divide helpers). It maps one 2 MB RAM object at 0x80000000, 0x00010000 (KUSEG mirror) and 0xA0000000, plus the scratchpad and I/O page. Its **SIGSEGV trampoline** catches a call that lands on a retail address (function pointers stored in retail data) and redirects EIP to the native function, which is transparent with cdecl. Its SIGFPE handler applies the R3000 divide-by-zero results. |
| `runtime/threads.c` | BIOS `OpenTh/ChangeTh/CloseTh` as coroutines on native stacks (tiny i386 context switch). |
| `runtime/gte.c` | Software GTE: register file, RTPS/RTPT/NCLIP/AVSZ3/4/NCCS/NCCT/MVMVA, flags. |
| `runtime/main_port.c` | A C translation of `main()` (0x800163F4). It is the only game function still in asm, because Ghidra named it like library code. It also contains the loader and the command line. |
| `shims/gpu.c` | libgpu and a software GPU: OT walk, GP0 decoder and rasteriser into emulated VRAM, display readout, PNG frame dumps (`--dump-frames`). |
| `shims/psyq.c`, `pad.c`, `card.c`, `libc.c` | libapi/libetc (VSync pacing, callbacks), libgte matrix helpers, libcd (reads sectors by LBA from the user's .bin/.cue/.iso; the game seeks with its own file table), libpad (fills the PadInitDirect buffers from host input), the memory card (BIOS events and card file system on two .mcd images), and the libc subset. MDEC (movies are skipped) is a silent stub; sound is `sound.c`. |
| `tools/inventory.py`, `tools/arity_check.py` | The inventory below and the list of "register-carried argument" calls. |

The prototype is i386 because the game's structures hold 32-bit pointers. The build machine has no 32-bit libc,
so the runtime is freestanding (no SDL in this milestone). A host with `gcc-multilib` and SDL2 for i386 drops
`-ffreestanding -nostdlib` and links SDL normally.

## 2. Architecture

```
src/*.c (matching C, unchanged) ──gen_port.py filter──> build/port/gen/src ──host cc──┐
port/shims  (Psy-Q API on the host)  ─────────────────────────────────────────────────┤──> tombi (one binary)
port/runtime (memory, threads, GTE, CD, loader) ──────────────────────────────────────┘
user's disc image (.bin/.cue/.iso or extracted dir) ──read at run time──> RAM image + CD sectors
```

Rules:

- The matching build stays authoritative. The port never edits `src/`. Changes the port needs go through the
  filter, `port/fixes.txt` (exact text replacements per file, which fail loudly when the source changes) or
  `#ifdef PORT` blocks, used only when a fix is needed in the shared headers. Other agents keep renaming
  functions; the port follows them, because it resolves names by retail address.
- Ownership of data is "retail image first". The user's MAIN0.EXE is loaded as the data segment. Every C global
  that exists in the retail image is linked to its retail address, so layout, initial values and pointers
  between data stay exact. This is what makes the port work with decompiled C that still has hundreds of
  `D_8009xxxx` externs.
- There is one native function per retail function, and `funcmap` is the bridge. Any retail code address
  (function pointer in data, task entry, overlay jump table) is resolved through it. The SIGSEGV trampoline is
  the safety net in the prototype. For release builds, replace it with explicit translation at the few places
  that store or call code pointers: the 102 explicit `(*fp)()` calls plus the tables in data. Those tables can
  be patched once at load, because `funcmap` knows every address. This also removes the dependency on
  POSIX signals (Android-safe).

### 2.1 32-bit and fixed-address assumptions

| Assumption | Count | Handling |
|---|---|---|
| Pointers stored in 32-bit fields (structs, tables in retail data, `int` casts) | pervasive | **Phase 1 (now): a 32-bit host** (i386 Linux/Windows, armeabi-v7a Android), so pointers are 32 bits wide as on the PS1. **Phase 2: 64-bit.** Keep all game memory in a 4 GB arena at a fixed base and replace pointer fields with 32-bit offsets (`PSXPTR(T)` macros in the headers, as PSY-Z does with `OT_TYPE`), or build with `-mx32` / an ILP32 ABI where available. Android Play requires arm64, so phase 2 is needed for the store, not for sideloading. |
| Hard-coded RAM addresses (`0x80xxxxxx` literals) | 4998 literals (0x8013: 1582, 0x8001: 832, 0x8012: 463, 0x8011: 384 ...); the 1032 `0x803Cxxxx` are constants, not addresses | RAM is mapped at its PS1 address (`MAP_FIXED`), so the literals stay valid. This is free on 32-bit Linux. On Windows use `VirtualAlloc` at 0x80000000 with `/LARGEADDRESSAWARE`. On Android, mmap at a fixed address is allowed but can collide; reserve early in `JNI_OnLoad`. |
| Scratchpad 0x1F800000 (`SPAD`, `TASK_C`, ...) | 913 literals plus named externs | Mapped at its address (16 KB including the I/O page, which is plain memory). |
| KUSEG / KSEG1 aliases (`ptr & 0xFFFFFF`, OT links) | e.g. `compactOrderingTable` | The same RAM object is mapped three times (memfd). Addresses below 0x10000 (BIOS area) are impossible, which is why `initTasks` reads the TCB table through `port_bios_tcb_table()` (fixes.txt). |
| Struct layout: `long long` / `double` alignment differs (i386 aligns 8-byte types to 4 inside structs, MIPS to 8) | rare | Audit with `STATIC_ASSERT_SIZEOF` on the shared structs (TObj and others) in the port build. |
| Semantics of the original compiler | everywhere | `-funsigned-char -fwrapv -fno-strict-aliasing -fno-delete-null-pointer-checks -O1`. Division by zero is trapped and emulated. |
| **Register-carried arguments**: a call passes fewer arguments than the callee reads, relying on `$a0-$a3` still holding a value | `arity_check.py`: 90 suspicious short calls in 60 files (some false positives, e.g. local names reused) | Fix each in `port/fixes.txt` (already done: `initLighting`→`applyLighting(o)`, `startTask(i, fn)`). This is the most important correctness audit; also check the opposite (callee declares fewer parameters than callers pass, like `startTask`). |
| Inline MIPS asm | 171 files: GTE macros (translated automatically), 3 top-level asm bodies (`_padChkVsync`, `FUN_8006bac4`, `CdRemove`: shimmed), register pins and `asm("")` barriers (dropped) | Done by the filter. |

### 2.2 Overlays

Every area overlay (X000..X019) and the SYSTEM overlays load at **0x800E8028**. X000 and all 16 area overlays
share their first 188 KB (about 300 object handlers), identical at the same addresses. The per-area code
starts at 0x80115EA4.

Plan:

- Link everything into one binary. The shared prefix is compiled once (X000's copy). Each overlay's own functions
  are compiled with a per-area prefix: the filter adds `#define func_80116000 X003_func_80116000` for every
  function the file defines that collides with another overlay. This works like the existing per-file renames
  in `gen_port.py`.
- **Dispatch tables per overlay.** `funcmap` becomes `{X000: [...], X001: [...], ...}`. The runtime tracks which
  overlay is resident: hook the overlay loader, since the file id comes from the load record, or compare the
  29-entry header table at 0x800E8028 against each overlay's table. `port_resolve_call` and the call thunks look
  up the current overlay first, then MAIN0. MAIN0 calls 332 distinct targets in the overlay region (`func_8011xxxx`
  externs). They are already compiled as thunks (398 in the prototype), so they turn into per-area dispatch
  without touching MAIN0 code.
- The overlay's data must still come from the file. The game's own CD load copies the overlay to 0x800E8028, so
  its data (tables, strings) lands where the native code expects it. The code bytes in RAM simply go unused.
- **Status (M1): done for the PAL variant-0 files.** The SYSTEM overlays are now split, largely matched and compiled into the port as an overlay set with per-overlay dispatch (§1 "Milestone M1"); the paragraphs below are the original plan. **Was the blocker:** `SYSTEM/O?PTSUB0.BIN` (options, 13 KB), `D?SPSUB*.BIN` (sub-screens),
  `G?OVER.BIN`, `I?NFO*.BIN` (inventory/info), plus the X1nn..X8nn language variants. These are about 91 MAIN0
  call targets. They need the same splat + matching treatment as the areas, or at least non-matching C for the
  port. Until then the title/options/menu screens cannot run natively. The prototype stops right there. A stopgap
  is a tiny MIPS interpreter for just these overlays (calls back into native code through `funcmap`), but
  decompiling them is cleaner.
- **O0PTSUB0 (title / options), first split done:** `config/o0ptsub0.yaml` (now part of the matching build, see tools/sysovl.py) splits
  it with `python3 -m splat split config/o0ptsub0.yaml --disassemble-all` into `asm/o0ptsub0/`. It is small:
  a 0x50-byte header (0x34 followed by 18 handler addresses into the overlay), then **27 functions, about
  3,300 instructions** (`func_800E8078` .. `func_800E9F5C`). MAIN0 reaches it through `func_800E8A14`,
  `func_800E8A2C`, `func_800E8A5C`, `func_800E8A94`, `func_800E972C` and `func_800E9D08`. Its outgoing calls are
  only MAIN0 helpers that the port already has natively (`playSFX`, `playSFXAndSetNote`, `keyOnSFX`,
  `updateSound`, `GetTPage`, `GetClut`, `SetDrawMode`, `addTilePrim`, `MulNegSin`/`MulCosDup`, `PadGetState`,
  `PadInfoAct`, `SsSetStereo/Mono`, `loadButtonConfig`) plus its own two most-called helpers `func_800E97F4`
  (25 calls) and `func_800E9798` (19). Next steps: (1) add it to the X-overlay workflow (symbol file
  `config/symbol_addrs_o0ptsub0.txt`, `[0x50, c, ...]` subsegments per function, m2c drafts, matching against
  `O0PTSUB0.BIN`; the language variants O1..O8 differ only in text, ±100 bytes); (2) for the port it only needs
  to compile, so non-matching C is enough at first; (3) give `gen_port.py` an "overlay set" so these
  functions are compiled with an `O0PTSUB0_` prefix and dispatched while that file is resident (§2.2 dispatch
  tables), since its addresses overlap the X000 functions. The same recipe then covers `G?OVER.BIN` (3 KB,
  game over), `I?NFO*.BIN` (inventory/info, 39-138 KB) and `D?SPSUB*.BIN` (sub-screens, 61-175 KB).

### 2.3 Psy-Q replacements

| Library | Game uses (see inventory) | Port design |
|---|---|---|
| libgpu | `GetClut` 109, `AddPrim` 87, `GetGraphType` 77, `SetSemiTrans` 63, `SetDrawMode` 62, `LoadImage` 41, `SetSprt`, `DrawOTag`, `ClearOTagR`, `PutDrawEnv`/`PutDispEnv`, `Move/Store/ClearImage` | The pure prim helpers (`AddPrim`, `SetPoly*`, `GetTPage`, `GetClut` ...) are compiled from their matched Psy-Q C as they are. **Renderer M2 (step 1 done, `shims/gpu.c`):** `DrawOTag` walks the OT (already done) and decodes the GP0 packets (POLY_F/FT/G/GT 3/4, SPRT, TILE, LINE, DR_MODE/TPAGE/AREA/OFFSET, fill/copy). Step 1 is a software rasteriser into the 1024×512 VRAM array, bit-exact and simple, shown with SDL as a texture; that is enough for PC. Step 2 is an OpenGL ES 3 / GL 3.3 backend: VRAM as an R16UI texture, palette lookup in the shader, semi-transparency modes via blending or framebuffer fetch, scaled internal resolution, widescreen by widening the projection (the game already has `initWideDisplayEnv`). The PCSX/Duckstation hardware renderers are the reference for texture-page and CLUT invalidation. |
| libgte | 171 files with GTE macros; `RotMatrix*`, `MulMatrix0`, `ApplyMatrix*`, `SetRot/TransMatrix`, `rsin/rcos/ratan2`, `SquareRoot0` | Software GTE in `runtime/gte.c` (done for the ops used). Later: an optional precise path that keeps sub-pixel float vertices next to the integer SXY (PGXP-style) for the GL renderer, plus the interpolation hooks (§3). |
| libspu / libsnd | `SsSeq*` (12/9/9/6...), `SsUtKeyOn/OffV`, `SsVabFake*`, `SpuSetReverb`, `SpuRead`... (library code is in MAIN0; much of it is matched Psy-Q C) | Emulate the **SPU**, not libsnd. Compile the matched libsnd/libspu C (the sequencer stays exact) and replace only the lowest layer (`_spu_*` register writes, SPU-RAM DMA) with a 24-voice software SPU: ADPCM decode, ADSR, pitch, reverb, mixed at 44.1 kHz into an SDL audio callback. The game's VAB/WVD data goes into a 512 KB SPU-RAM array. |
| libcd | `CdControl/F/B`, `CdRead`, `CdReadSync`, `CdSync`, `CdMix`, `CdRead2`/`St*` (movies) | Done for data: seeking uses the game's own table (`FILE_LINKS`, LBA+150 in BCD), and sectors are read from `.bin` (raw 2352) or `.iso`. Add CUE parsing, CHD later, and an "extracted folder" mode (LBA→file map built from the ISO directory at startup). XA audio and STR streaming go through the same reader. |
| MDEC (libpress) | `DecDCTin/out/vlc` (movies) | Software MDEC (IDCT + YUV→RGB) feeding the `St*` ring. `MovieDecodeFrame` is where the prototype spins. In the meantime a "skip movies" option should report the end of the stream. |
| libpad | `PadInitDirect`... (the game links Sony's driver as source: 30 `_pad*`/`pad*` files) | Done: the driver files are left out and the buffers are filled each VBlank. Host input comes from SDL2 GameController and keyboard; Android uses SDL's controller support plus an on-screen touch overlay. |
| Memory card | `InitCARD`, `StartCARD`, `_bu_init`, `_card_*`, `open/read/write` on `bu00:` | Back the BIOS file calls with a 128 KB `.mcd` file per slot (same format as emulators, so saves are portable). |
| libapi / BIOS | events, threads (`OpenTh/ChangeTh` ×6), critical sections, `SysEnqIntRP`, `FlushCache` | Done (coroutines, VSync event). |
| libc | `memcpy` 51, `memset` 22, `sprintf`, `printf`, `rand` | Done (`shims/libc.c`); with a real libc, map onto it. |

## 3. Timing and 60 fps

The game runs its logic in tasks woken by `ThreadTableRun` once per main-loop pass. The pass waits on the
VBlank counter (`0x1F8001E8 < 0x1F8001EA`), so on PAL it runs at 50 Hz or 25 Hz.

1. **M1: correct pacing (done).** Run the main loop at 50 Hz (PAL) or 60 Hz (NTSC timing option) from a host timer
   (`--realtime --hz 50|60`), with input sampled per VBlank. The game logic stays untouched.
2. **M5: interpolated rendering at the display rate.** Keep the simulation at its native tick and render extra
   frames in between:
   - Snapshot what the renderer needs after each logic tick. These are object positions and rotations (TObj
     position/velocity fields, `objectsOverlap`/`AnimAdvance` users), the camera/section scroll
     (`initSectionCamera` state), and the 2D layer offsets.
   - Hook the points where the game turns those into GTE input: `SetRotMatrix`/`SetTransMatrix` per object,
     the sprite position writes before `AddPrim`. Tag every primitive with (object id, vertex index). The port
     controls `SetTransMatrix`, the gte_* macros and the prim helpers, so this needs no game-code change.
   - Then, for a render at fraction t between ticks N−1 and N, rebuild the OT with interpolated matrices and
     positions. The cheaper variant interpolates the screen-space vertices of primitives tagged with the same
     key, which is what PGXP-based "60 fps" hacks do. Teleports (key not found, or delta above a threshold)
     snap instead of blending.
   - Animated sprite frames and UI do not interpolate. Only geometry and scroll positions do.
   - Pause, menus and the boot logo run at the native rate.

   Doing this inside the renderer abstraction (§2.3 GL backend) keeps it out of `src/`.
3. A "true 60 Hz logic" mode (halved per-tick constants) is not planned. It would change gameplay and touch
   hundreds of functions.

## 4. Android

- Use SDL2's `android-project` template (Gradle + NDK, `SDL_main`). `port/CMakeLists.txt` becomes an
  `add_library(main SHARED ...)` target. Rendering uses GLES 3, audio the SDL callback (AAudio/OpenSL inside SDL).
- The ABI is **armeabi-v7a first**, because it keeps the 32-bit pointer model of phase 1. On 32-bit ARM Linux,
  mapping 0x80000000 works when reserved early. arm64-v8a needs phase 2 (§2.1).
- Replace the signal-based SIGSEGV trampoline with load-time pointer translation, and use the host libc
  instead of the freestanding runtime.
- **Assets come from the user's own disc.** A first-run screen uses the Storage Access Framework to pick a
  `.bin/.cue`, `.iso` or `.chd` the user dumped. It is verified by SHA-1 of `EXE/MAIN0.EXE` (bec8dc5f...) and of
  the overlays, and copied or indexed into app storage. The APK ships no game data, no Sony SDK code and no
  BIOS. All Psy-Q functionality is reimplemented in `port/`.

## 5. Inventory

Generated by `python3 port/tools/inventory.py --md` (the counts include the 16 area overlays):

- Game C files: MAIN0 799, X000 523, and 304 to 546 per area overlay (7519 distinct files). There are 339 Psy-Q
  C files; the port keeps about 45 pure ones and replaces the rest.
- Psy-Q functions in MAIN0: 644 (305 without C). 107 are called directly from game C (table below). More are
  reached through Psy-Q internals that the port replaces wholesale.
- Inline asm or GTE macros: 171 files (36 distinct gte_* macros. Some names have different store offsets in
  different files, so they are translated per file). There are 3 top-level asm function bodies.
- Hard-coded addresses: 4998 `0x80xxxxxx` and 913 scratchpad literals. There are 102 explicit calls through
  function pointers, plus the object handler tables in retail data.
- **Overlay entry points:** every overlay starts with `0x1D` followed by a table of 29 absolute pointers
  (`0x800E9ECC`, ...) to `jal`/`j` entry stubs at file offset 0x1EA4. MAIN0 calls 332 distinct addresses in
  0x800E8028..0x80140000. 241 of them are function starts in at least one area overlay; the rest belong to the
  shared header or to the SYSTEM overlays.
- One game function is still in asm only: `main` (0x800163F4, 1396 bytes, translated by hand in
  `port/runtime/main_port.c`).
- Register-carried argument calls: 90 candidates (`port/tools/arity_check.py`).

Psy-Q functions called directly from game C (calls, programs):

| Function | Calls | Programs |
|---|---:|---|
| `GetClut` | 109 | MAIN0 X000 X001 X002 X003 X004 X006 X009 X010 X013 X017 |
| `AddPrim` | 87 | MAIN0 X000 X001 X002 X003 X004 X005 X006 X008 X009 X010 X011 X013 X014 X016 X017 X018 X019 |
| `GetGraphType` | 77 | MAIN0 X000 X001 X002 X003 X004 X005 X006 X008 X009 X010 X011 X013 X014 X016 X017 X018 X019 |
| `SetSemiTrans` | 63 | MAIN0 X006 |
| `SetDrawMode` | 62 | MAIN0 X000 X001 X002 X003 X004 X005 X006 X008 X009 X010 X011 X013 X014 X016 X017 X018 X019 |
| `rcos` | 56 | X001 X004 X006 X009 X010 |
| `rsin` | 56 | X001 X006 X009 X010 |
| `memcpy` | 51 | X000 X001 X002 X003 X004 X005 X006 X008 X009 X010 X011 X013 X014 X016 X017 X018 X019 |
| `LoadImage` | 41 | MAIN0 X002 X003 X004 X009 X010 X014 X016 |
| `SetRotMatrix` | 40 | MAIN0 X004 |
| `SetTransMatrix` | 40 | MAIN0 X004 |
| `SetDispMask` | 30 | MAIN0 |
| `ApplyRotMatrix` | 26 | MAIN0 X000 X004 X006 X014 |
| `RotMatrix` | 23 | MAIN0 X000 X003 X006 X009 X014 |
| `SetSprt` | 22 | MAIN0 |
| `memset` | 22 | MAIN0 |
| `MulMatrix0` | 20 | MAIN0 X000 X006 X014 |
| `SquareRoot0` | 18 | X003 X006 X009 X010 X014 |
| `EnterCriticalSection` | 15 | MAIN0 |
| `ExitCriticalSection` | 15 | MAIN0 |
| `RotMatrixX` | 13 | MAIN0 X000 X006 X009 X014 |
| `ratan2` | 13 | X006 X009 X010 |
| `RotMatrixY` | 12 | MAIN0 X000 X006 X009 |
| `SsSeqSetVol` | 12 | MAIN0 |
| `EnableEvent` | 9 | MAIN0 |
| `MoveImage` | 9 | X009 X010 |
| `OpenEvent` | 9 | MAIN0 |
| `SsSeqClose` | 9 | MAIN0 |
| `SsSeqStop` | 9 | MAIN0 |
| `ClearImage` | 7 | MAIN0 |
| `RotMatrixZ` | 7 | X006 X014 |
| `ApplyMatrixLV` | 6 | X000 X003 |
| `ApplyMatrixSV` | 6 | MAIN0 X006 X009 |
| `ChangeTh` | 6 | MAIN0 |
| `SetSprt8` | 6 | X006 |
| `SsSeqOpen` | 6 | MAIN0 |
| `SsSeqPlay` | 6 | MAIN0 |
| `SsSetMVol` | 6 | MAIN0 |
| `SetDrawLoad` | 5 | MAIN0 |
| `SsUtKeyOffV` | 5 | MAIN0 |
| `CdMix` | 4 | MAIN0 |
| `GetDrawEnv` | 4 | MAIN0 |
| `OpenTh` | 4 | MAIN0 |
| `PutDispEnv` | 4 | MAIN0 |
| `PutDrawEnv` | 4 | MAIN0 |
| `SsUtSetReverbDelay` | 4 | MAIN0 |
| `csqrt` | 4 | MAIN0 |
| `printf` | 4 | MAIN0 |
| `start` | 4 | X001 X009 |
| `CdControl` | 3 | MAIN0 |
| `CdControlF` | 3 | MAIN0 |
| `ChangeClearRCnt` | 3 | MAIN0 |
| `CloseTh` | 3 | MAIN0 |
| `DecDCTout` | 3 | MAIN0 |
| `DrawSync` | 3 | MAIN0 |
| `FlushCache` | 3 | MAIN0 |
| `SetDefDispEnv` | 3 | MAIN0 |
| `SetDefDrawEnv` | 3 | MAIN0 |
| `SetGeomScreen` | 3 | MAIN0 |
| `SsUtKeyOnV` | 3 | MAIN0 |
| `StoreImage` | 3 | MAIN0 |
| `SysDeqIntRP` | 3 | MAIN0 |
| `sprintf` | 3 | MAIN0 |
| `CdControlB` | 2 | MAIN0 |
| `ClearOTagR` | 2 | MAIN0 |
| `DecDCTin` | 2 | MAIN0 |
| `DecDCTinSync` | 2 | MAIN0 |
| `DecDCToutCallback` | 2 | MAIN0 |
| `DecDCTvlc` | 2 | MAIN0 |
| `DrawOTag` | 2 | MAIN0 |
| `GetGp` | 2 | MAIN0 |
| `InitCARD` | 2 | MAIN0 |
| `OpenTIM` | 2 | MAIN0 |
| `ReadTIM` | 2 | MAIN0 |
| `SetBackColor` | 2 | MAIN0 |
| `SetColorMatrix` | 2 | MAIN0 |
| `SetFarColor` | 2 | MAIN0 |
| `SetShadeTex` | 2 | MAIN0 |
| `SpuQuit` | 2 | MAIN0 |
| `SpuSetReverb` | 2 | MAIN0 |
| `SsUtSetReverbFeedback` | 2 | MAIN0 |
| `StClearRing` | 2 | MAIN0 |
| `StFreeRing` | 2 | MAIN0 |
| `StGetNext` | 2 | MAIN0 |
| `StUnSetRing` | 2 | MAIN0 |
| `StartCARD` | 2 | MAIN0 |
| `StopCARD2` | 2 | MAIN0 |
| `_bu_init` | 2 | MAIN0 |
| `_patch_card2` | 2 | MAIN0 |
| `bzero` | 2 | MAIN0 |
| `callObjectInteraction` | 2 | MAIN0 |
| `CdRead` | 1 | MAIN0 |
| `CdReadSync` | 1 | MAIN0 |
| `CdSync` | 1 | MAIN0 |
| `GetTPage` | 1 | MAIN0 |
| `RotTransPers` | 1 | MAIN0 |
| `SpuGetAllKeysStatus` | 1 | MAIN0 |
| `SpuIsTransferCompleted` | 1 | MAIN0 |
| `SpuRead` | 1 | MAIN0 |
| `SpuSetTransferStartAddr` | 1 | MAIN0 |
| `SsIsEos` | 1 | MAIN0 |
| `SsUtChangePitch` | 1 | MAIN0 |
| `SsVabClose` | 1 | MAIN0 |
| `SsVabFakeBody` | 1 | MAIN0 |
| `SsVabFakeHead` | 1 | MAIN0 |
| `SysEnqIntRP` | 1 | MAIN0 |
| `callback` | 1 | MAIN0 |

## 6. Milestones and effort

Estimates are for one experienced developer and assume the decomp keeps its current state.

| # | Milestone | Content | Effort |
|---|---|---|---|
| M0 | Headless prototype (**done**) | All MAIN0+X000 C compiled for the host, retail-image data model, thunks, coroutine tasks, software GTE, CD reads, pad buffers; boots to the title overlay | done |
| M1a | Title + AREA00 (**done**) | SYSTEM overlay set with dispatch, MIPS interpreter fallback, automatic register-carried arguments, scripted pad, raw-video pipe; title screen, new game, first room, dialogue, walking | done |
| M1 | Window + input + pacing (**done**) | SDL2 window in a host-native viewer process (`--window`), keyboard and game controller, 50/60 Hz pacing (`--hz`), audio through SDL; save states (`--save`/`--load`); `.cue` sheets as `--disc`; memory cards as 128 KB `.mcd` files | done |
| M2 | Software renderer | GP0 decoder + rasteriser (textured/gouraud/semi-transparent/dither), display area from DISPENV; the boot logo and AREA00 visible | 2–3 weeks |
| M3 | SYSTEM overlays | decompile (or non-matching C) O?PTSUB0, D?SPSUB*, G?OVER, I?NFO*; MDEC + STR movies or skip | 3–6 weeks |
| M4 | All areas | per-area symbol prefixing + overlay dispatch, fix register-carried calls and other ABI issues found by playing every area; correctness audit vs. an emulator (RAM diff per frame on the same inputs: compare the native build to PCSX/Duckstation traces) | 3–5 weeks |
| M5 | Sound | software SPU + matched libsnd, XA/CD audio | 2–3 weeks |
| M6 | GL renderer + enhancements | GLES3/GL3 backend, internal resolution, widescreen, PGXP-style precision, **interpolated 60 fps** | 3–4 weeks |
| M7 | Android | SDL android project, armeabi-v7a, SAF disc import, touch controls; then the 64-bit pointer model for arm64 | 2 weeks (+3–4 weeks for 64-bit) |

**Main blockers today:**

- The SYSTEM overlays are compiled in (O0PTSUB0 25/27 matched, the rest largely); their missing functions run
  in the MIPS interpreter.
- MDEC/movies, SPU and the renderer are not written yet.
- The register-carried argument calls are forwarded automatically; wrong guesses must be fixed per area.
- The 32-bit pointer model rules out arm64 until phase 2.

## 7. Lessons from other projects

| Project | What it does | What we take |
|---|---|---|
| [PsyCross / Psy-X](https://github.com/OpenDriver2/PsyCross) (MIT; used by [REDRIVER2](https://github.com/OpenDriver2/REDRIVER2), grew out of the TOMB5 PC port's "emulator" layer) | Psy-Q-compatible headers; software libgte; libgpu over OpenGL with **emulated PSX VRAM**; **PGXP-Z** (precise GTE vertex cache, perspective-correct, Z-buffer); SPU over OpenAL (ADPCM, "SPU-AL"); libcd on BIN/CUE; SDL2 window and input; Emscripten target | This is the closest match to our design: game C unchanged, the Psy-Q API reimplemented. Our M2/M6 renderer follows the same split (VRAM texture + GP0 decoding + optional precise vertices). **Option:** link PsyCross as an external dependency (fetched by CMake, never vendored) instead of writing `shims/psyq.c` further. Its libgpu works at the API level, so our OT walk would be replaced by its `DrawOTag`. Check how its ordering tables and pointer widths fit our retail-image data model before switching. |
| [PSY-Z](https://github.com/Fancy2209/psyz) (spin-off of [sotn-decomp](https://github.com/Xeeynamo/sotn-decomp)'s psxsdk, reusing Sozud's psy-q-decomp) | A drop-in Psy-Q replacement for Linux/macOS, **32- and 64-bit**. It favours API compatibility over hardware accuracy. 64-bit needs `u_long*` OT pointers changed to an `OT_TYPE*` | For phase 2 (64-bit), adopt the same `OT_TYPE` convention in our headers behind `#ifdef PORT`, and reuse its approach of building the decompiled Psy-Q C itself where it is hardware-free (we already do this for about 45 prim helpers). |
| [sotn-decomp `src/pc`](https://github.com/Xeeynamo/sotn-decomp/tree/master/src/pc) | A PC build of the decomp from the same C. Each overlay is linked as its own unit with stubs (`ovl_stubs.c`, `stubs.c`), with platform files (`io.c`, `pc.c`) beside the game code | Confirms the "same src/, separate build, stub what is missing" approach. Stub every missing symbol automatically and report it; our generated thunks do this. |
| Wipeout Phantom Edition (fan source port of the PS1 Wipeout; [report](https://www.retrorgb.com/wipeout-phantom-edition-a-new-source-port-for-the-1995-original.html)) | Uncapped render rate with the simulation **decoupled from rendering through interpolation** | The model for M6: fixed logic tick, interpolated render state. Our variant hooks the GTE matrix and prim layer instead of rewriting the game's draw code. |
| Parasite Eve II PC (closed source, [news](https://ixbt.games/en/news/2026/09/14/spustia-27-let-modder-perenosit-parasite-eve-ii-na-pk-s-ulucsennoi-grafikoi-i-podderzkoi-vysokogo-razreseniia.html)) | Reported to recompile the PS1 code to a native Windows executable with higher-resolution assets | No code to study. It shows that static recompilation is an alternative where C is missing, for example for our SYSTEM overlays as a stopgap. |
| PS4/PS5 PS1 Classics 60 Hz patch ([report](https://www.techradar.com/news/sonys-ps-plus-classics-fix-has-only-made-things-worse)) | Converted PAL to 60 Hz by blending frames, which caused ghosting | A warning: never blend frames. Interpolate state, not images. |

No third-party code is copied into this repository. Any of these libraries would be fetched at configure time
under its own licence (PsyCross is MIT).

## Milestone M2 (in progress)

Done:
- **Area overlays native**: `gen_port.py area_plan` compiles X001..X019 (prefix `X0nn_`); functions whose bytes equal
  X000's run X000's code; X000 functions that differ in some area are renamed `X000_<name>` and reached through
  dispatch thunks. The resident area is recognised by two signature words (`port_areas`), looked up in
  `port_areamap`. Short calls are forwarded by the callee's address and by the widest definition across areas /
  SYSTEM overlays. AREA01 (debug select) runs with 0 interpreted calls; AREA00 frames unchanged.
- **Asm-only MAIN0 functions** `func_800416B8`, `func_8004AE7C`, `func_80038184` in C (`runtime/main0_handwritten.c`).
  `--check ADDR` runs each call interpreted and native on the same state and compares RAM/scratchpad/return
  (func_800416B8: 1182 calls, 0 differences; the other two are not reached in the tested runs, 80038184 has no caller).
- **Sound, first step**: `shims/sound.c` software SPU (ADPCM, ADSR, pitch, 24 voices) + high-level libsnd (VAB from
  SsVabFakeHead, SEQ sequencer, SsUtKeyOnV effects); `--dump-audio out.wav`. Title music matches tools/audio.py in pitch.
- `--poke FRAME:ADDR:VALUE[:SIZE]` (debug area select: `--poke 900:1F8001B4:1`), `-v` prints area/section/player x,y.
- The BIOS kernel area (KUSEG 0..0xFFFF) is mapped to the RAM object (page 0 needs root / mmap_min_addr 0), so NULL
  reads see zeros as on a PS1 (fixes area 0 section 3).

- **Every room of the disc runs from the debug select** (96 of 96: all `ROOM_COUNTS` sections minus the 13 empty-load-list
  rooms, which crash in the real game too, see tools/psxemu/rooms.py `DOOR_ROOMS`). Contact sheet of frame 2000 of each
  run: the sweep script pokes `GAME.selectedArea/Section` (0x8009C960/2) at frame 1250 and presses Circle at 1300.
  Some sections are redirected by the game itself (AREA19 sections 0/1 load AREA02 0/1, as in tools/psxemu; 4:5, 4:12,
  4:17, 9:6 continue into another section). What was wrong, all in the generator/runtime:
  - AREA02 section 0 and AREA05/08 section 0 really load `I0NFO.BIN` / `I0NFO2.BIN` at the overlay base (the
    emulator does the same). MAIN0 calls those overlays' functions by the X000 name of the same address
    (`drawCategory7Objects` -> 0x800EC710), so X000 functions that share a start address with any SYSTEM overlay
    function are now "varying" and dispatched (gen_port.py `area_plan`).
  - `LoadClut`/`LoadClut2` were stubs (AREA02 backdrop drawn with CLUT 0).
  - Typed data aliases in area C (`D_1F8002DCx`, `DAT_1f800310p`, 41 names) were linked as call thunks; they now
    resolve to their address (AREA14).
  - The forwarding pass dropped forwarded arguments on `else f(...)` / `return f(...)` lines (its prototype rewrite
    took them for declarations), and per-file renamed callees (`#define fa fa__8013839C`) now get the arity of their
    address (AREA10 `updateL2Type1FObject` -> leaf butterfly).
  - Area dispatch entries named `X0nn_<unit name>` instead of the name the C file defines (`func_801167A0.c` defines
    `area6ScriptOp`): the thunk resolved to itself and looped forever (AREA06 hang).
  - Data that area C defines inside the overlay region is now bound to its retail address like MAIN0 data (X004's
    6-entry koma pig animation table was indexed past its end: garbage frames, a runaway sprite loop wrote 450 KB of
    primitives over the overlay; AREA04 sections 5, 6, 11).
- **Real-time audio**: `--pipe-audio FIFO` writes raw s16le stereo 44.1 kHz per vblank. FIFOs are opened read-write
  (no open-order deadlock with players) with 1 MB buffers, SIGPIPE is ignored, and audio is produced before video
  each vblank. Live: `ffplay -f s16le -ar 44100 -ac 2 a.fifo` next to the video `ffplay`; with `--realtime` the game
  is paced at 50 Hz, without it a blocking audio player paces the game. One ffmpeg process reading both FIFOs
  deadlocks (it interleaves inputs by timestamp and blocks on audio while the video FIFO is full), so use one
  consumer per FIFO (recording: two ffmpeg processes, then mux).
- Debug helpers: `--dump-ram FRAME:PATH` (2 MB RAM + 1 KB scratchpad, same layout as a tools/psxemu dump, for RAM
  diffs against the emulator), `--dump-packets FRAME` (every GP0 packet of that vblank), and the fault report names
  the interpreted function and pc.

Pending:
- **Walking AREA00 -> AREA02 (with one progress flag poked).** Sections 0/1/2 of AREA00 are one map
  (updateA00ScrollController: sec0 -> sec1 at x>=0x4ED,y<-0x95; sec1 exit 0 at x>=0xA46,y>=-0x122; sec1 exit 1 (the tree
  hollow) at x>=0x938,y>=-0x64 only when `0x8009CFED == 1`; sec1 -> sec2 at x>=0x9DD,y<-0x18F; sec2 exit 1 at
  y<-0x559,x<0x88C on plane 0x1C2); sec3 (elder's house, FUN_801175a4) exit 1 at x>=0x12B,y<-0xDB, exit 0 at
  x>=0x157,y>=-0x38. The exits fire as soon as the position test passes (tryStartItemInteraction needs no button).
  This run starts in the elder's house from the debug select, plays the elder's dialogue and event (the game drops
  Tombi into sec2), walks down to the blue egg in sec1, climbs the trunk, grabs the egg (lands on it) and throws it,
  and walks into the hollow; the hollow's flag (set by I0NFO3 `func_800E86AC`, i.e. later in the game) is poked:
  ```
  build/port/tombi --frames 6500 -v --poke 900:1F8001B4:1 --poke 4000:8009CFED:1 --pad-script 1000:start,1200:down,\
  1220:right:2x3/8,1350:circle,1900:cross:3x40/30,2300:cross:3x40/30,2700:right:400,3100:cross+right:30x8/60,\
  3600:right:100,3750:cross:25,3760:right:30,3850:square:5x20/40,3900:cross:25x10/50,4450:right:400
  ```
  The door transition loads AREA02 section 3 at frame 4563 (its I0NFO3 overlay, the same files as the debug select)
  and shows the "CLEAR 2000" event banner.
- **Why the hollow needs the flag (checked, no poke-free way through it).** `0x8009CFED` is written in one place,
  I0NFO3 `func_800E86AC`, the section handler of AREA02 section 3: that room is the underground tunnel network
  (I0NFO3 is its overlay: no jumping, the pad walks Tombi along the tunnels, a mine cart at the east end). Its
  five exits are position tests on the player (`D_800A6038`): exit 0 at x 0x448..0x478, y -0x474..-0x460
  (-> AREA02 sec 4), exit 1 at x<0x99, y -0x4FE..-0x47E (-> AREA02 sec 5), **exit 2 at x<0x99, y -0x2E8..-0x268
  sets the flag and leads to AREA00 sec 1 spawn 2, i.e. out of the hollow**, exit 3 at x 0x65F..0x6A7, y
  -0x2D0..-0x2BC (-> AREA01 sec 3), exit 4 at x 0x667..0x6AF, y -0x4B0..-0x497 (-> AREA04 sec 11). So the hollow
  is a one-way shortcut that the game opens from the tunnel side, later in the story. The tunnels to exits 1/2/4
  are themselves gated by event flags in I0NFO3 `func_800E847C` (`D_8009CE21`, `D_8009CE24`, `D_8009CE34`: one-way
  slopes at x 0x2FC..0x36F, 0x3B6..0x3E9, 0x510..0x5AB), so from the debug select only exit 3 is open. The
  spawn/exit tables are `D_80079A48[area][section]` (8 bytes: x, y, type, next area, section, spawn;
  `python3 port/tools/exits.py AREA [X0nn.BIN]` prints them); AREA00's are in X000 at 0x801384A8.
- **AREA00's other exits, measured in the port** (all three with the debug select only, `--poke 900:1F8001B4:1`):
  sec 1 exit 0 (x>=0xA46 at ground level, -> AREA00 sec 5 -> AREA10) is behind the hollow wall: walking right
  stops at x=2360 (`hollow0` run). sec 0 exit 0 (x>=0x4A2, -> sec 4, the white tower) is behind the purple bushes
  at x=1100 and the "fog" villager at x~1000 (his dialogue triggers on contact). The way up to sec 1 (x>=0x4ED,
  y<-0x95) is the hut: running jump from x~965 onto the lower bush (1100,-99), jump onto the balcony with the
  pink villager (1095,-147); the next ledge is the collision plane-1 floor at x 1096..1176, y -212 (player y
  -232), 7 px above Tombi's jump (peak -225 from the balcony); not solved (grab, bush bounce or another object).
  **The 7 px are not a port bug** (checked this round, see "Pad input and jump arc" below): the port's jump arc is
  the emulator's frame by frame, so the real game cannot reach the ledge with a plain jump either.
  sec 2 (above sec 1, reached by the trunk at x~2290 and the ledge with the white egg) has no static floor between
  y -500 and the house platform at -1360 (x 2064..2688): the climb is on objects (stone pillars at (2909,-432),
  a beam at (3040..3125,-619), a net, a vine); thorns at x>=3119 hurt; not solved either. `port/tools/colmap.py`
  draws a section's static floors from a `--dump-ram` image (per depth plane; objects are not in it).
- **AREA00 sec 0 from a new game, no poke (round 4): the hut ledge is reached, the way on is the "Clear the Fog" event.**
  Mechanics read in src/ and checked in the port: a bite grab needs Tombi *falling* (`wac`==1, set at the jump's peak
  in playerStepJump or when velY>0x400) and overlapping the object from above (`objectBoxOverlapSideFacing` < 3):
  komaPigContact / playerContactCat2Type08 then set `wac`=2, step 14 (playerStepBitten, Tombi rides the object), any
  other contact with a pig hurts him. Cross while holding = step 15 (playerStepBittenJump, the normal jump table
  `DAT_80115468[u]`, u = `GV2.bb3` + 4*dash, u=0 here: velY -976 -> 76 px); a *new* Cross/attack press during the jump
  throws the object (substep 2 freezes Tombi for the throw animation, no height gain; the throw also happens by itself
  near the floor) and the pig becomes a layer-1 projectile that kills what it hits. Holding without jumping eats the pig
  after ~30 frames. Type 0x0e objects (cat-2 type 14: the apple on the tree at (853,-236)) give velY -0x600 in
  playerBittenJumpType0E. The red bird at the hut is cat-2 type 8 (playerContactCat2Type08): touched from the side it
  grabs Tombi (step 0x40 playerStepGrabbedStruggle) and lifts him 1 px/vblank at x 1181 to y -393, then drops and hurts
  him; >10 new d-pad presses within 255 vblanks break free (the carry does not pass the x=1200 scroll wall). The hut
  balcony girl is a koma pig subtype 9 (type 2): she hurts on contact, so she is grabbed and thrown first.
  The ledge above the balcony (plane 1, y -212) is reached with **Up+Cross** (the lane jump into the background plane)
  from x 1164: Tombi lands at (1168,-232). Walking right onto the roof reaches x 1182, y -286, and the fog event starts
  (`func_80116818` state 0: x>=0x49E, y<-0xE7 in section 0 -> `D_8009C966`=1, event 2 started, "CLEAR THE FOG 500"
  banner, frame 5480). The route (pad script, 50 Hz vblanks; save states replay it exactly, so it was built from
  states and then run from frame 0):
  ```
  build/port/tombi --frames 5760 -v -v -v --pad-script 1000:start:4,1504:cross:3x4/33,1736:select:3,1859:cross:3x18/43,\
      2700:right:150,2860:right+cross:30,2890:right:60,3010:right+cross:3,3062:cross:15,3080:cross:3,3160:right:300,\
      3160:cross:30x5/60,3610:right:100,3720:cross:3x40/25,4710:left:120,4840:right:60,4870:cross+right:40,4960:cross:40,\
      5010:cross:3,5020:right:30,5046:cross:30,5130:right:40,5175:up+cross:30,5210:right:100,5320:right+cross:30,5360:right:300
  ```
  Start, opening dialogue; walk to the first tree (x 227) and jump over it with Cross held (2860); fall onto the pig at
  x 338 (3010: grab at 3056), jump straight up holding it (3062) and throw it right (3080: it clears the pig on the
  stone pillar at (429,-140)); run right with jumps (the pillar is flipped onto, step 7/11, and the hill after it);
  the hooded villager's dialogue at x 998 (3720: Cross taps); back to x 929 and a running jump onto the lower bush
  (4870, swing on the hook, step 6), Cross onto the balcony (4960, Tombi at (1096,-147)); hop onto the girl (5010,
  grab at 5037), Cross (5046: jump + auto throw), walk to x 1164, Up+Cross (5175) onto the plane-1 ledge (1168,-232),
  Right+Cross (5320) onto the roof at (1182,-286) -> fog event. `--poke 900:1F8001B4:1` is not a movie skip: it opens
  the debug area select (the movie is skipped by Start). Sheets: scratchpad `port_r4/route_ledge.png` (grab, throw,
  balcony, ledge) and `port_r4/route_fog.png` (the event banner).
  **What gates sec 1:** updateA00ScrollController keeps `subtype`=1 (scroll clamped at x 1200, `animFrame`=0x4B0) until
  `D_8009C966`==3 or `GCD.bcda6`==0xFF; only then does x>=0x4ED, y<-0x95 switch to section 1. `D_8009C966` becomes 3 at
  the end of the fog event (`func_80116818` state 4), which waits in state 1 for `GAME.area00_eventControl & 1` and
  `GVARS.b30`==3; `GVARS.b30`=3 is set by playerStepUseItem when **item 4** is used (`DAT_8009c941`==4), and the tornado
  (a00TornadoStep, spawned by `FUN_801228bc` while `bcda6`!=0xFF) also needs eventControl bit 1 to leave `b30`=1. No
  writer of eventControl bit 1 is in src/ (it comes from an overlay / event script), so the item and the flag are the
  next things to find; the port runs every step of the route without a fault.
- **Walking AREA02 -> AREA01 (no progress flag poked):** from the debug select into AREA02 section 3, the tunnels
  lead to exit 3 and AREA01 section 3 at frame 2747:
  ```
  build/port/tombi --frames 3400 -v --poke 900:1F8001B4:1 --poke 1250:8009C960:2:2 --poke 1250:8009C962:3:2 \
      --pad-script 1000:start,1300:circle:3,1600:right:500,2110:up+left:340,2455:up+right:300
  ```
  AREA01 sec 3 (autumn village behind the hut) continues right to a cliff at x=2014 that Tombi climbs to y=-939;
  its exit 1 (x>=0x7FC, y>=-0x293, -> AREA01 sec 4) and exit 2 (x>=0xB4B after x>=0x821, -> AREA09) are beyond it;
  not reached yet.
- Talk button crash fixed on the way: `playerTryTalkToNpc` reads its counter through a second name of the same
  address (`D_1F80019Es`), which gcc kept in a register; such names are now declared volatile by the generator.
- Sound: reverb, libsnd details (voice priorities, NRPN loop counts); an SDL audio callback once there is a window.

### Pad input and jump arc (checked)

- **Scripted pad delivery is exact.** `readPadPort` runs once per vblank from `updatePadInput` (the game's main task,
  `ThreadWaitFrames(1)`), and `port_pad_vblank` fills the PadInitDirect buffer before the vsync callback of the same
  vblank, as the libpad driver does in the vblank interrupt. `-v -v` prints each read; a press held N vblanks is read N
  times in the title menu, in the memory card slot screen (title -> LOAD -> slot) and in the in-game save screen:
  ```
  build/port/tombi --frames 1700 -v -v --pad-script 1000:right:3,1050:start:3,1300:right:3,1400:left:1,1500:right:8,1600:left:8
  build/port/tombi --frames 2100 -v -v --menu 1700:4 --pad-script 1000:start:3,1300:cross:3,1600:cross:3,1800:right:3,1900:left:1,2000:right:8
  ```
  Right/Left of 3, 1 and 8 vblanks each move the slot cursor (frame dumps checked). The earlier observation (a 3-frame
  press not registering in the save prompt) was not reproducible; the one real hole was in `--window`: the viewer
  sampled the key state once per frame, so a tap shorter than a frame was never sent, and the game kept only the last
  sample drained in a vblank. Now the viewer latches a key or controller button pressed since its last sample
  (`tapped`), and `port_window_poll_pad` ORs every sample drained in the vblank into that vblank's state. Checked under
  Xvfb with xdotool (`Xvfb :77 &`, `DISPLAY=:77 build/port/tombi --window --realtime -v -v ...`, then `xdotool key Right`,
  `xdotool keydown Left; sleep 0.2; xdotool keyup Left`): a 12 ms tap is read once, a 200 ms hold 10 times, a 30 ms
  tap 3 times.
- **Jump arc = emulator.** `python3 port/tools/jumpcheck.py out.txt` plays, in tools/psxemu, a new game, the opening
  dialogue (`capture.TALK`) and three jumps in AREA00 sec 0 (standing; running; Cross held 30 vblanks) and logs the
  player x,y per frame. The same inputs in the port (Start at vblank 1000, the emulator's script offsets added):
  ```
  build/port/tombi --frames 3340 -v -v -v --pad-script 1000:start:4,1504:cross:3x4/33,1736:select:3,1859:cross:3x18/43,\
      2693:cross:3x4/33,2915:cross:3,3008:right:40,3048:right+cross:3,3051:right:90,3171:right:40,3211:right+cross:30,3241:right:60
  ```
  Both give y = -52,-56,-60,-63,-67,-70,-74,-77,-79,-82,-84,-86,-88,-90,-91,-92,-93,-94 (4 frames) ... -51,-52 for the
  standing jump (peak 42 px, 37 vblanks in the air), the identical running arc (-48 -> -90, x 123 -> 221), and with Cross
  held 66 px (-53 -> -118) onto the first tree at x 242; the emulator's trace only differs by single-frame glitches of
  its own (it slices vblanks by instruction count). So the port's physics (software GTE, fixed point, vblank pacing)
  match, and the sec0 hut ledge (85 px above the balcony vs the 78 px full jump) is not reachable by jumping in the
  real game either; the legit AREA00 route still needs another mechanic (a grab, a bounce or an object) or the sec2
  object climb. Not solved this round.
- **Save states replay exactly (checked).** The earlier observation (a run continued from a `--save` state taken in sec 0
  at frame 3160, Tombi idle at x 227, "does not walk on" when Right is held from 3180) was a misread, not a bug: the
  fresh run does not walk on either. At x 227 Tombi stands against the first tree (x 242) after the running jump, so
  Right alone never moves him; the jumpcheck run gets past it with the Cross-held jump at 3211. Verified with the
  fresh run (`--save 3160:s.state --dump-ram 3161:fresh.ram`) against `--load s.state --dump-ram 3161:load.ram`, same
  `--pad-script` (the jumpcheck prefix up to `3051:right:90`, then `3180:right:200`, and again with `3171:right:200`):
  the 2 MB RAM + scratchpad images at 3161 are byte-identical (`cmp`), and the `-v -v -v` x,y trace and the `-v -v`
  pad-read trace of frames 3161..3400 are identical (240 lines, `diff` empty). There is nothing game-observable outside
  the state: every writable mapping of the binary is in it (VRAM, SPU, GTE, coroutine stacks, pad edge state, CD
  position), and the runtime takes no anonymous mappings besides the PS1 memory.
- **Player sprite missing in the I0NFO rooms (fixed).** AREA02 sec 3 (underground tunnels) showed no Tombi, and
  `--dump-packets` had his two textured quads collapsed to a point. The black regions of that room are the room itself
  (tools/psxemu renders the same black; the rest of the frame, VRAM and the GP0 packet list match packet for packet).
  Cause in the generator: `gen_port.py` dropped every empty asm with operands as a register-allocation hint, but
  `asm("" : "=r"(out) : "0"(in))` with two different names is a move (the old gcc's `move`), and I0NFO/I0NFO2/I0NFO3
  `func_800EAD48` (the overlay's player draw, `drawPlayer` type 1..3) computed its slice table pointer from the
  uninitialised output. Tied outputs are now emitted as `out = in;` (`tied_asm_moves`); it also gives
  `func_8006B62C` (control/index) and one `clearObj = obj` their values. Checked: port frame 1800 of
  `--poke 900:1F8001B4:1 --poke 1250:8009C960:2:2 --poke 1250:8009C962:3:2 --pad-script 1000:start,1300:circle:3`
  against the emulator (`capture.base_state`, area 2 section 3, +140 vblanks): Tombi drawn at the same place; the
  jumpcheck trace in AREA00 is unchanged.

## Windows build (tombi.exe, tombi-view.exe, tombi-port.exe)

The same freestanding i386 binary, cross-compiled with mingw-w64 on the Win32 API; `port/runtime/sys_win32_i386.c`
is the counterpart of `sys_linux_i386.c` (same `port_*` interface, picked by `port/CMakeLists.txt` from the platform).

```
sudo apt-get install gcc-mingw-w64-i686                 # cross compiler (Ubuntu); wine32 to try the result
curl -LO https://github.com/libsdl-org/SDL/releases/download/release-2.30.11/SDL2-devel-2.30.11-mingw.tar.gz && tar xzf SDL2-devel-2.30.11-mingw.tar.gz
cmake -S port -B build/port-win -G Ninja -DCMAKE_TOOLCHAIN_FILE=$PWD/port/cmake/mingw-i686.cmake \
      -DPORT_SDL2_MINGW=$PWD/SDL2-2.30.11/i686-w64-mingw32
ninja -C build/port-win                                  # -> build/port-win/tombi.exe, tombi-view.exe (needs SDL2.dll next to it)
wine build/port-win/tombi.exe --frames 600 --exe game/MAIN0.EXE --disc game/disc/Tombi.bin
python3 tools/port_launcher.py                          # the launcher (Tk, tools/i18n.py); uses build/port-win/*.exe on Windows
```

- **Symbols.** The game is compiled with `-fno-leading-underscore -mno-ms-bitfields` so the generated tables (`syms.ld`,
  `thunks.S`, `funcmap.c`) are identical on both hosts; the few kernel32/winmm imports are declared in the runtime with
  their decorated names (`_CreateFileA@28`). `gen_port.py link` skips those undefined names and, for mingw, emits the call
  thunks as plain globals linked last (a COFF weak symbol with a body is not a definition for the mingw ld) and no `.type`.
- **Memory.** One 2 MB pagefile-backed section (`CreateFileMapping`) mapped with `MapViewOfFileEx` at 0x80000000 and
  0xA0000000 (the image is linked `--large-address-aware` and based at 0x10000000), the scratchpad/I/O page with
  `VirtualAlloc` at 0x1F800000. The KUSEG mirror at 0x00010000 is mapped whole when Windows left the range free, else
  every free 64 KB block of it (the rest reads the host's own data there: a warning says how much is missing). Page 0
  cannot be mapped on Windows: accesses below 0x200000 that fault are emulated in the exception handler by copying the
  instruction into an executable scratch page with its memory operand rewritten to the KSEG0 address of the same byte
  (one exception per access, so only the stray NULL-pointer reads go this way).
- **Calls to retail addresses.** The RAM is `PAGE_READWRITE` and the image `--nxcompat`, so a call into it is an access
  violation with EIP = the PS1 address; `AddVectoredExceptionHandler` rewrites EIP to the native function (the same
  trampoline as the Linux SIGSEGV handler); `EXCEPTION_INT_DIVIDE_BY_ZERO` gets the R3000 result. `__chkstk_ms` (stack
  probes for frames over 4 KB) is provided in asm; `port/runtime/sys_common.c` holds the printf and the 64-bit division
  helpers shared by both runtimes. The coroutines of `threads.c` are plain stack-switching asm and work unchanged.
- **The viewer.** `tombi_view.c` builds for both: on Windows it reads the frames from `GetStdHandle(STD_INPUT_HANDLE)`
  and gets the audio/pad pipe handles as `--audio-handle N --pad-handle N` (fds 3 and 4 cannot be inherited by number);
  `port_spawn` in the Win32 runtime makes the three pipes with `CreatePipe` and runs it with `CreateProcess`. New viewer
  options, passed through `tombi --viewer-arg X` (repeatable): `--scale 1..6`, `--fullscreen` (F11 toggles it), `--keys
  up=Up,cross=X,...` (SDL key names per PS1 button). The pad pipe is read with `PeekNamedPipe` (non-blocking).
- **Files.** `port_open/create/pread/pwrite/filesize` on `CreateFile/ReadFile/WriteFile` (OVERLAPPED offsets); the FIFO
  options (`--pipe-frames`, `--pipe-audio`) write plain files on Windows (use `--window`). `timeBeginPeriod(1)` + `Sleep`
  + a spin for the last millisecond pace the vblanks; `QueryPerformanceCounter` is the clock.
- **Status (checked under wine32 9.0).** The exe boots, maps the memory, loads MAIN0.EXE and the disc and runs the first
  frame; under wine the KUSEG range 0x10000-0x110000 is wine's own reserved area, so the title screen's
  `queueLoadList(NULL)` scan (which on Linux walks the zero kernel area into the EXE image through the mirror until it
  meets a -1) reads wine's data instead and the load queue never completes (`sleepTask` loop at frame 1). On real
  Windows that range is normally free (unverified: no Windows machine here); if it is not, the warning line names the
  missing blocks and the same stall is expected. The Linux build is unchanged (`cmake -S port -B build/port` as before).
- **CI.** `.github/workflows/port-exe.yml`: job `game` (ubuntu, mingw) fetches the player's disc image from the
  `GAME_FILES_URL` secret like report.yml, runs `tools/build_full.py`, cross-compiles `tombi.exe`/`tombi-view.exe` and
  uploads them with `SDL2.dll`; job `launcher` (windows-latest) packs them with PyInstaller into `tombi-port.exe`
  (`--onefile --windowed`), uploads the artifact `tombi-port-windows` and commits `dist/windows/tombi-port.exe`;
  `zip-exe.yml` adds it to `tombi-tools.zip`. No game data is in any artifact (SDL2 licence: THIRD_PARTY_NOTICES.md).

### The launcher (tools/port_launcher.py, tombi-port.exe)

Tk window with the same style/i18n as tombi-gfx/tombi-text. *Play* tab: disc image (.bin/.cue/.iso; `EXE/MAIN0.EXE` is
read from its ISO9660 directory into the launcher's folder and checked against the SCES-01331 sha1
`bec8dc5f...`), save folder (`--save-dir`), window scale 1x/2x/3x, fullscreen, 50/60 Hz, "start a new game
automatically", a Play button (runs `tombi.exe --exe MAIN0.EXE --disc ... --window --viewer-arg --scale N ...` with the
game's stderr in `tombi-port.log` next to the exe, shown in the window) and the exact command line. *Mods* and
*Controls* tabs: see "Mods". Settings are saved in `tombi-port.json` next to the exe. From a checkout it runs
`build/port-win/tombi.exe` (Windows) or `build/port/tombi` (Linux); the PyInstaller exe carries `tombi.exe`,
`tombi-view.exe` and `SDL2.dll` and unpacks them to its temporary folder on each run (PyInstaller onefile).

## Mods (tools/port_mods.py)

Every mod is a set of options of the port binary, assembled by the launcher (the *Mods* tab shows each one with its
description; the list, labels and the arguments live in `tools/port_mods.py`). Frames: the title screen is up before
vblank 1000 (`--pad-script 1000:start`), `initGameConfig` has run by 1100, the debug select is poked at 1250 and
confirmed with Circle at 1300 (the values of the room sweep). Addresses: `GAME` 0x8009C960 (include/tomba/game.h).

| Mod | Port options | Notes |
|---|---|---|
| Skip intro logos/movies (default on) | nothing / `--play-movies` when off | the port skips them by default |
| 60 Hz | `--hz 60` | NTSC pacing (`--hz 50` otherwise) |
| Area select | `--poke 900:1F8001B4:1 --poke 1250:8009C960:A:2 --poke 1250:8009C962:S:2 --pad-script 1000:start,1300:circle:3` | area/section pickers from `ROOM_COUNTS` (names from README); rooms of `DOOR_ROOMS` are not offered |
| Start with N AP | `--poke 1100:8009C96C:N:4` | `GAME.playerAP` (drawHudAP) |
| Start with N lives | `--poke 1100:8009C980:N:1` | `GAME.playerLives` (drawHudLives) |
| Infinite lives | `--poke-every 50:8009C980:63:1` | new option: `--poke-every INTERVAL:ADDR:VALUE[:SIZE]` writes every INTERVAL vblanks |
| Infinite vitality | `--poke-every 1:8009C970:4:1` | `GAME.playerHealth`, 4 = full (initGameConfig) |
| Unlock all events | `--poke 1100:8009CDA4:FF:1x130` | new `SIZExCOUNT` suffix of `--poke`: COUNT consecutive values; event flag bytes 0x8009CDA4 + id (notes/names.md "Event links"), 0xFF = completed |
| Custom keys | `--viewer-arg --keys --viewer-arg up=Up,cross=X,...` | *Controls* tab: one SDL key name per PS1 button |
| Button prompts (PlayStation / Xbox / Keyboard) | `--patch-tex prompts-SET.tbtx` | see below |

**Button prompts.** `tools/prompt_glyphs.py` draws our own glyph sets with Pillow (no third-party icon pack):
`port/assets/prompts/{playstation,xbox,keyboard}/<button>.png`, 16x16, 4 palette indices (0 transparent, 1 outline,
2 fill, 3 mid tone); the keyboard set is rendered from the key mapping (`pack SET OUT --keys up=Up,...`, the launcher
passes the *Controls* tab's keys). `pack` writes a TBTX file (`"TBTX"`, u32 count, then u16 x, y, w, h in VRAM
words/rows + w*h words) from the rects of `port/assets/prompts/manifest.json`, and the port's new option
`--patch-tex FILE` (`port/shims/gpu.c`) re-applies every rect after each `LoadImage` that overlaps it, so the game's
own upload of the font/HUD page is overwritten 1:1 in VRAM. The launcher packs `prompts-<set>.tbtx` next to itself
when a set other than PlayStation is chosen. **Open: the rects.** The manifest's rects are all `null` (the mechanism
is checked with a synthetic manifest: 14 patches applied, no fault); locating the game's glyph sprites — the tutorial
hints `updateA00TutorialHint` / `a00TutorialHintJumpMessage` (x000, "press X to jump" by button configuration), the
button-config screen of the SYSTEM overlays, the HUD/font pages of `tools/ui_cluts.py` and the `UiSpriteDef` table —
and filling in `rect` (x, y, w words, h rows) and `idx` (which CLUT entries are dark/light) per button is what is
left; `tools/gfx_editor.py` can show the pages. Until then the option changes nothing (pack returns 0 glyphs).

Checked on Linux: the pokes of a new game (AP 5000, 9 lives, both `--poke-every`, the 130 event bytes) run 400 vblanks
without a fault; a 1500-frame run with the title's Start stalls at vblank ~443 in this checkout (`sleepTask` loop after
the SYSTEM overlay read at lba 176314) with and without the mods, and also with the port as committed before this work,
so it is an environment/regression question of the base port, not of the mods (left open; see the report of this work).

### Left to do

- Run `tombi.exe` on real Windows (the KUSEG mirror and the viewer's pipes are untested outside wine; wine's reserved
  low range breaks the NULL load-list scan, see "Status").
- The *button prompts* mod: the glyph rects of the manifest (see "Button prompts" above); the art and the
  `--patch-tex` mechanism are done.
- The stall at vblank ~443 noted above.
