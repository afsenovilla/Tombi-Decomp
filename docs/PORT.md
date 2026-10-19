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
#   options: --exe PATH --disc PATH(.bin raw 2352 or .iso) --frames N --realtime (50 Hz) --autostart N -v [-v]
#            --pad-script FRAME:BUTTONS[:LEN[xCOUNT/PERIOD]],...  --dump-frames DIR --dump-every N --pipe-frames FIFO
#            --no-interp --interp ADDR --trace-interp
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
- The decompiled-but-asm-only functions above (interpreted today), MDEC movies (skipped), memory card (stub).
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

**Why no SDL window yet:** this host has the i386 `libSDL2-2.0.so.0` runtime but no 32-bit libc/crt or SDL
headers, and the runtime is freestanding (no dynamic loader). Installing `libsdl2-dev:i386 gcc-multilib` is
possible from the Ubuntu archive, but apt wants to remove 32 packages to do it on this machine, so it was not
done. Options, in order of cost: (a) on a host with
`gcc-multilib` + `libsdl2-dev:i386`, drop `-ffreestanding -nostdlib`, link SDL2 and upload
`port_gpu_frame()`'s RGB buffer to an `SDL_Texture` each vblank (≈80 lines; input goes into `port_pad_vblank`);
(b) without 32-bit dev libs, a 64-bit viewer process that maps a shared-memory frame buffer written by the
i386 binary (memfd + `--display-fd`), which also isolates the host toolkit from the 32-bit game process.
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
| `shims/psyq.c`, `pad.c`, `libc.c` | libapi/libetc (events, VSync pacing, callbacks), libgte matrix helpers, libcd (reads sectors by LBA from the user's .bin/.iso; the game seeks with its own file table), libpad (fills the PadInitDirect buffers from host input), and the libc subset. Sound, MDEC (movies are skipped) and the memory card are silent stubs. |
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
  `PadInfoAct`, `SsSetStereo/Mono`, `loadCollisionBounds`) plus its own two most-called helpers `func_800E97F4`
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

1. **M1: correct pacing.** Run the main loop at 50 Hz (PAL) or 60 Hz (NTSC timing option) from a host timer
   (`--realtime`), with input sampled per VBlank. The game logic stays untouched.
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
| M1 | Window + input + pacing | multilib or a Windows 32-bit build, SDL2 window, VRAM viewer, keyboard and gamepad, 50/60 Hz pacing, `.cue`/`.iso` parsing, memory card files | 1 week |
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

Pending:
- Walking AREA00 -> next area with --pad-script not achieved yet. Findings: sections 0/1/2 of AREA00 are one map
  (updateA00L5Type11: sec0 -> sec1 at x>=0x4ED,y<-0x95; sec1 exit 1 at x>=0x938,y>=-0x64 -> AREA02; sec1 -> sec2 at
  x>=0x9DD,y<-0x18F); sec3 (elder's house, FUN_801175a4) exit 1 at x>=0x12B,y<-0xDB -> AREA01, right side -> sec2.
  From the house: `1000:start,1200:down,1220:right:2x3/8,1350:circle,1900:cross:3x40/30,2300:cross:3x40/30,
  2700:right:400,3100:cross+right:30x8/60,3600:right:1500` with `--poke 900:1F8001B4:1` reaches sec1 x=2239, where a
  blue egg blocks the way to the AREA02 exit.
- Debug-select sweep crashes to fix (fixes.txt candidates): areas 8, 10, 14 (segfaults), 11/16/17/18 (interpreter hits
  garbage / NULL+2), 6 hangs; areas 2 and 5 load I0NFO/I0NFO2 at the overlay base from the debug select (not X002/X005).
- Sound: reverb, real-time output, libsnd details (voice priorities, NRPN loop counts).
