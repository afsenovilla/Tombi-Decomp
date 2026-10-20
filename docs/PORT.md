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
  (`addr2line -f -e build/port/tombi ...`). The SYSTEM overlays are forwarded one overlay at a time, after
  MAIN0/X000 (as each area is): their functions are named by address and the overlays overlap, so a call resolves
  among the overlay's own definitions first (the `<PROG>_` renames must not hide it, which was the title →
  Options crash: `func_800E8B94()` → `func_800E8E44(sel)` read the cursor tile table with a stack word as
  `sel`). The first log line is `[port] build <git sha> <date>` (`port/cmake/build_id.cmake`, re-run on every
  build; the launcher shows the same id in its title and as the log header, from the bundled `build_id.txt`).
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

**Pacing: `--realtime`, `--hz 50|60`, `--show-fps`, `--stall FRAME:MS`.** One vblank every 1/hz s from the host
clock (default 50, PAL; 60 gives NTSC timing: the game logic runs 20% faster, as on an NTSC console). `--window`
implies `--realtime` and honours `--hz`. The schedule is absolute (`port_vblank` in `shims/psyq.c`): vblank n is
due at `epoch + n/hz`, waited for with `port_sleep_until_us` (Linux: `clock_nanosleep` `TIMER_ABSTIME` to 200 µs
before the deadline, then a spin on `clock_gettime`; Windows: `timeBeginPeriod(1)` at start, then `Sleep(1)` is
measured eight times and the worst case + 0.5 ms (at least 2 ms) is the margin: `Sleep` to that long before the
deadline, then a spin on `QueryPerformanceCounter`; the log says `timer: Sleep(1) takes up to 1.9 ms` normally, 15-16
ms where the 1 ms resolution is not honoured, and the spin then covers it), so no rounding accumulates. A vblank more than one period late
(a slow frame, the viewer's pipe blocked) re-anchors the epoch: one vblank runs without waiting and the game
drops to real time, it never runs a burst of fast frames to catch up. The Windows `port_time_us` used to compute
`count * 1e6 / freq` in 64 bits, which overflows after about 30 minutes of uptime with a 10 MHz counter: from then
on the comparison was garbage and the game ran unpaced (event banners and music far too fast, or right, depending
on the machine's uptime); it now divides first.

**Audio keeps its own clock** (`shims/sound.c`): paced, each vblank mixes the samples that became due on the wall
clock since the previous one (44100/s whatever the vblank rate), headless it mixes 44100/hz per vblank; the
sequencer ticks every 44100/hz samples (1/hz s of *audio* time, the libsnd vblank tick), so the tempo depends on
the sample count only. A stall over four frames re-anchors the audio clock too (the gap is dropped, not queued).
**The viewer never blocks the game** (`tools/view/tombi_view.c`): an audio thread drains the audio pipe
unconditionally into a half-second ring buffer that the SDL callback pulls from (two frames of silence are put
ahead of the data at start and after an underrun; the callback skips ahead when more than six frames are queued and
the thread drops the oldest samples when the ring is full, so a stalled *player* costs audio, never game time), and
a video thread reads the frame pipe into a one-frame latest-wins mailbox that the main loop presents (vsync on where
available: `SDL_RenderPresent` then blocks for up to a display period, which must not reach the game; a 60 Hz
display shows 50 of its 60 presents with a new frame). The earlier viewer read a frame and then presented it, and
stopped reading audio while SDL held over four frames: both put the game on the display's and the audio device's
clocks through the pipes (1 MB each on Windows, 64 KB on Linux). `--viewer-arg --stats` logs frames received /
shown / skipped and the ring fill, drops and padding every second (`tombi-view:` lines). Every pipe read in the
viewer loops until the count is complete (`ReadFile` on an anonymous pipe returns as soon as any bytes are there: a
245 KB frame arrives in pieces); a header that is not `TBFR` is resynchronised by sliding to the next magic and
logged (`frame stream out of step`). Test hooks: `PORT_PIPE_CHUNK=N` caps every viewer read at N bytes, and
`--viewer-arg --dump DIR[:N]` writes every Nth received frame as `DIR/view_NNNNNN.ppm`, numbered like the game's
`--dump-frames` PNGs (first vblank = frame 1); with `PORT_PIPE_CHUNK=1000` under Xvfb, 1500 frames (Start at 1000)
give 15 of 15 dumped frames identical between the game and the viewer, on Linux and with the exe under Wine. `--show-fps` logs the
measured vblank rate and audio samples per second every second, `--show-fps 2` adds a histogram of the inter-vblank
intervals (`vblank intervals (ms:count) ...: 20-21:50 max 20.3 ms` is right; counts under 15 ms are vblanks run
without waiting after a late one, counts over 25 ms are the stalls that caused them); the normal exit (`port_quit`:
frame limit or viewer closed) prints the overall rate (`paced: N vblanks in T s`, from the first paced vblank, so
loading counts) and the tally of emulated accesses below 0x200000 (see "Windows build"). The launcher passes
`--show-fps 2 --viewer-arg --stats`, so a user's `tombi-port.log` carries both. `--stall FRAME:MS` sleeps MS ms
before that vblank (test hook). Measured under Xvfb with SDL's dummy audio device (which consumes about 4 %
fast: the `padded` counter grows by it), 1500 frames with Start at 1000, histogram per second: old viewer 25x18-19
+ 25x20-21 ms (the 18-19 bucket was 19.9 ms before the buckets rounded to the nearest ms), old viewer with a
simulated 60 Hz blocking present the same plus one 15-17 and one 22-24 per second, new viewer 50x20-21 with `0
skipped`; the exe under Wine with `--window` (Win32 `Sleep`/`QueryPerformanceCounter`, `CreatePipe`): 48-50x20-21
per second, 50.00 vblank/s. So on Linux and under Wine none of the three viewer-side suspects (audio
back-pressure, `Sleep` rounding, the vsync'd present) moves the vblank rate; on a Windows machine where it still
does, the histogram in the log says which second and by how much. Measured on Linux, `--realtime --show-fps --dump-audio`: 3000 frames take
60.03 s and write 2646205 samples (60.005 s); with `--stall 1500:600` they take 60.63 s and write 2646302 samples
(60.007 s): the rate reads 27.99 vblank/s in the stalled second, 49.99-50.01 before and after, never above 50.
Under wine, tombi.exe reports 50.00 vblank/s and 44100 samples/s. The event banner (`printEventMessage`, type
0x22 objects) is advanced once per game frame from `ThreadTableRun`; `vblankHandler` only counts vblanks, no
`VSyncCallback` is registered (logged with `-v`), so its speed is the pacing alone.

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
| KUSEG / KSEG1 aliases (`ptr & 0xFFFFFF`, OT links) | e.g. `compactOrderingTable` | `gen_port.py` rewrites the game's KUSEG pointer masks to the KSEG0 view (`port_kuseg_mask`, see "Windows build": the mirror at 0x10000 is only a fallback). The same RAM object is mapped three times (memfd). Addresses below 0x10000 (BIOS area) are impossible, which is why `initTasks` reads the TCB table through `port_bios_tcb_table()` (fixes.txt). |
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

*Superseded: the Windows build of the native-pointer port is "M3: Phase 3" below (tombi2.exe, no fixed addresses);
this section documents what the fixed-address `port/` needed and why it was abandoned.*

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
  A game C file that *defines* a retail-named datum (`char D_80098c44[844] = {...}`, `fileLink FILE_LINKS[1119]`) is
  rewritten by `gen_port.py` (`retail_definition`) into `extern` of the retail name plus a copy named `port_copy_<name>`:
  the ELF link binds the defining object's own references to the `syms.ld` address, but a COFF object resolves them
  section-relative at assembly time, so before this the Windows build read SYS/LD0SYS.BIN into queueLoadList's native
  `D_80098c44` while the rest of the game read the (empty) lists at 0x80098C44, walked `queueLoadList(0)` through the
  kernel area, queued garbage file ids and asked the disc for lba 218357 (`CdRead failed`). `retail_definition`
  applies at file scope only (`filter_src` tracks the brace depth outside comments and strings): the matching
  sources also declare locals on unindented lines, and `int *h;` in `ObjMotionStep` (the object's x-position
  pointer, one line with the body) was rewritten to `extern int *h;` because `x000.full.ld` PROVIDEs the lib
  aliases `f`, `g`, `h` (`h` = SfxPlay2 at 0x8001E560, native code in the port). The store `h = o->h` then went
  to SfxPlay2's first bytes, `fault address=10006d5c (write)` at `ObjMotionStep+0x16` the first time the player
  bit a pig (`playerStepBitten` → `playerBiteHoldType02`). `check_retail_rewrites` stops the generation if a
  rewrite ever lands inside a block again. Checked: the 28 frames dumped every 200 vblanks of the 5760-frame
  AREA00 route below (pig grab at 3056) are identical between the Linux build and the exe under Wine.
- **Memory.** One 2 MB pagefile-backed section (`CreateFileMapping`) mapped with `MapViewOfFileEx` at 0x80000000 and
  0xA0000000 (the image is linked `--large-address-aware` and based at 0x10000000), the scratchpad/I/O page with
  `VirtualAlloc` at 0x1F800000. The KUSEG mirror at 0x00010000 is where Windows puts its own early allocations
  (the process parameters, the default heap, sometimes a thread stack), and the game *draws* through KUSEG pointers:
  the ordering tables and primitive buffers at 0x80090000-0x800CFFFF are reached masked with 0xFFFFFF (SetSprt,
  modelDrawFT4/GT3/GT4, drawObjectSpriteScaled, compactOrderingTable: 29 million such accesses in 4300 frames under
  `--no-kuseg`), so a block of that range in use by Windows meant the game wrote its packets over the host's heap
  and read them back through the KSEG0 view (garbage picture, then a jump to a wild pointer such as
  `eip=7fac06e8`, a heap word; Wine keeps its own data at 0x110000-0x1FFFFF, above the game's range, so it never
  showed). So when the range is not free, `start` runs the exe again with `CREATE_SUSPENDED`, reserves every free
  64 KB block of 0x10000-0x1FFFFF in the child with `VirtualAllocEx` before its loader has made a heap, resumes it
  and relays its exit code; the child (`PORT_KUSEG_CHILD=1` in its environment) releases the reservations in
  `port_setup_memory` and maps the mirror there (`-v` logs both steps). What still cannot be mapped is listed by the
  warning; `--kuseg-strict` stops there, `--no-kuseg` maps nothing and logs every emulated access (which code uses
  KUSEG), `--fake-kuseg` gives the game private memory instead of the mirror (reproduces the Windows failure under
  Wine: garbage packets, crash at frame 758). Page 0 cannot be mapped on Windows: accesses below 0x200000 that fault
  are emulated in the exception handler by copying the instruction into an executable scratch page with its memory
  operand rewritten to the KSEG0 address of the same byte (one exception per access, so only the stray NULL-pointer
  reads and the unmapped KUSEG blocks go this way).
- **The KUSEG mirror is not load-bearing any more.** On a real Windows 10 machine the relaunch still left blocks of
  0x10000-0x1FFFFF in use (the loader, a DLL or security software had them before `VirtualAllocEx` ran) and the game
  died in the title video exactly like `--fake-kuseg` under Wine: the video stuttered (one exception per packet word
  written through the unmapped blocks), the ordering table held a link `0x80d6a8a8` and `DrawOTag` crashed at frame
  758 after `[gpu] unknown GP0 command e8` (garbage packets). The fix is at generation time, `gen_port.py
  port_kuseg_mask` (in `filter_src`, so `src/` is untouched): the game keeps its primitive buffer pointer masked
  with 0xFFFFFF (`NEXT_PRIM` / `D_1f800164` / `G164` / `SPAD.p164 = (int)(LOAD_BUFFER_ALT + i * 0xc000) & 0xffffff`
  in 15 files) and `compactOrderingTable` masks the table pointer the same way; the port emits `(... & 0xFFFFFF) |
  0x80000000`, the same byte through the always-mapped KSEG0 view. Two things relied on the KUSEG pointer's zero top
  byte and are rewritten with it: the whole-word link store `*(unsigned long *)otz = (unsigned long)p;` (the
  modelDraw* / drawModelUnlit / otadd idiom, 36 sites: a KSEG0 pointer stored whole would put 0x80 into the tag's
  length byte, so the port stores the 24-bit link) and `compactOrderingTable`'s walk, which compares links with
  pointers and follows them (`fixes.txt`: 24-bit comparison, KSEG0 dereference). `DrawOTag` (shims/gpu.c) ORs
  0x80000000 into every link before following it, as before. Checked with `--no-kuseg` (now on Linux too: no mirror
  mapped, every access below 0x200000 emulated and tallied per site with its symbol by `port_low_report` at exit):
  **35,858,034 emulated accesses in 5000 frames (title + AREA00) before, 0 after**, and the 100 frames dumped every
  50 vblanks are identical to the previous build's; under Wine the exe passes 2500 frames with `--fake-kuseg` and
  with `--no-kuseg` (0 emulated accesses), same frames. The mirror and the relaunch stay as a fallback for code
  the rewrite does not know (a new idiom shows up as a non-zero `--no-kuseg` count, or on Windows as a slow
  game plus the tally in the log). The tally is now printed at every normal exit too (`port_quit`: frame limit
  reached, viewer closed), so a user's `tombi-port.log` ends with `N emulated accesses below 0x200000` and the
  sites; the full 5760-frame route (fog banner included) gives 0 on Linux with `--no-kuseg`, and under Wine 140 at
  three sites that are page-0 NULL reads (`playerCheckPerchLanding+0xac` address 3, `playerStepHurtKnockback`),
  not KUSEG. Blocks the warning lists as `committed` are Windows' own memory: an access there does not fault and
  is not counted, so a graphic corruption that appears on Windows only is checked with a run under `--no-kuseg`
  (every low access faults and is counted, with its site) rather than from the tally alone. The 28 frames dumped
  every 200 vblanks of the route are identical between the Linux build and the exe under Wine at 5760 frames
  (checked again after the viewer rewrite). The emulation itself (`port_low_emulate`, sys_common.c, shared by both runtimes)
  covers every ModRM-addressed instruction (any base/index/SIB/displacement, 8/16/32-bit, `lock`/segment/operand-size
  prefixes, `lea` excluded) and restarts a string instruction (`movs`/`stos`/`cmps`/`scas`/`lods`, with `rep`) with
  ESI/EDI moved to KSEG0. GP0 opcodes E7-FF and 03-1E are one-word nops to the GPU and are now skipped without
  touching state; an unknown command logs its frame and drops the rest of the packet.
- **The KUSEG range is guarded by default on Windows (no mirror).** A user's Windows 10 log after the mask rewrite
  showed 0 KUSEG accesses in the tally yet a picture that corrupted slowly: a sprite patch or a pig with a wrong
  CLUT, a room with every texture page wrong, event banner letters at random positions, then whole frames of garbage
  polygons; the same build is pixel-identical to Linux under Wine. The tally cannot see it: the warning listed
  `10000-60000, b0000, c0000` as `committed`, Windows' own memory (process parameters, the default heap), and a
  game access through a 0x000xxxxx pointer there does not fault, it reads and writes the heap silently (0xB0000-
  0xCFFFF is where the primitive buffers live). So `port_setup_memory` now maps nothing at 0x10000-0x1FFFFF
  unless `--kuseg-mirror` is given: every free block is reserved `PAGE_NOACCESS` (the blocks the relaunched parent
  reserved stay reserved), an access there faults into `low_access`, is served from KSEG0 by `port_low_emulate`
  and tallied with its site, and the blocks Windows had already committed are named at start (`KUSEG blocks
  committed by Windows before the game started: ...`, the only ones an unrewritten access could still touch
  silently). `--kuseg-mirror` is the old behaviour (map the RAM object where the range is free, `--kuseg-strict`
  needs it whole); `--no-kuseg` only adds logging; `--fake-kuseg` is unchanged. The MIPS interpreter
  (`runtime/mips_interp.c`), which runs the asm-only functions and the area overlays' code on plain host
  addresses, now translates every load/store in 0x10000-0x1FFFFF to KSEG0 itself (`port_interp_kuseg`, printed by
  `port_low_report` when non-zero), so an overlay's own `& 0xFFFFFF` never reaches host memory on any host. On
  Linux with `--no-kuseg` (the range is unmapped: checked in `/proc/PID/maps`) the 5760-frame AREA00 route, the
  6500-frame elder's-house / fog-banner run and the AREA02 -> AREA01 debug-select walk all count 0 accesses and 0
  interpreted translations, so the sites the user hits are off these routes; his next `tombi-port.log` lists them
  by symbol with the first address instead of corrupting the picture.
- **Calls to retail addresses.** The RAM is `PAGE_READWRITE` and the image `--nxcompat`, so a call into it is an access
  violation with EIP = the PS1 address; `AddVectoredExceptionHandler` rewrites EIP to the native function (the same
  trampoline as the Linux SIGSEGV handler); `EXCEPTION_INT_DIVIDE_BY_ZERO` gets the R3000 result. `__chkstk_ms` (stack
  probes for frames over 4 KB) is provided in asm; `port/runtime/sys_common.c` holds the printf and the 64-bit division
  helpers shared by both runtimes. The coroutines of `threads.c` are plain stack-switching asm and work unchanged. A fault prints the registers, the image base, the
  thread, the nearest native function of EIP and of every stack word inside the image (`port_sym_near`: funcmap,
  ovlmap, areamap, libmap, so shims too), the last call thunk `port_resolve_call` answered and, for an EIP equal to
  the fault address, that a bogus pointer was jumped to; the Linux handler prints the same (`port_dump_stack`,
  sys_common.c). `gen_port.py link` lists the retail-named data a game object still defines beside its syms.ld
  address (what `retail_definition` did not rewrite: only the `asm("D_80010000")` build-date strings today), since
  that is the pattern a COFF object binds to its own copy.
- **The image must not be relocated.** Every `call FUN_800xxxxx` to a function the port has no native symbol
  for is a rel32 to the syms.ld absolute address, computed at link time against the image base 0x10000000 (data
  references to those addresses are DIR32 immediates, for which ld emits no base relocation, so they survive). On
  Windows 10 the loader applied ASLR to the exe (binutils 2.36+ links `--dynamicbase` by default) and put it at
  0x0F2E0000: the game ran, since its RAM accesses were intact, until the first such direct call, `probeCollisionAtDepthA`
  → `call 800406e8`, which landed at 0x800406e8 + (0x0F2E0000 − 0x10000000) = `eip=7f3206e8`, outside the range
  the handler redirects ("a jump to a bogus address", the same registers every time: the same frame of the jump).
  Wine never relocates, so it never showed. The exe is now linked `--disable-dynamicbase`; as a fallback the handler
  computes `image_delta = __ImageBase − PORT_LINK_BASE` at start (a warning is printed when it is not zero, `-v`
  prints "image at its link base") and treats a fault at `eip = retail + delta` as the retail address (logged for the
  first 8, all with `-v -v`). Checked under Wine with a variant of the exe linked at 0x7F100000 (its end overlaps KUSER_SHARED_DATA at
  0x7FFE0000, so the loader has to relocate it, to 0x00120000; `sys_win32_i386.c` compiled with
  `-DPORT_LINK_BASE=0x7f100000ul`): without the fallback it dies in the jump exactly like the report (`eip=800406e8
  fault address=010606e8`, frame 2111), with it the game plays 3000 frames through the jump and logs
  `jump to 010606e8 = retail 800406e8 + image delta`. A fault report also lists the 8 call thunks resolved before the last one (`port_note_resolve`, sys_common.c,
  with the frame of each), and `-v -v -v` logs every thunk resolution; `-v -v` logs every redirect of a jump to a
  retail address.
- **The viewer.** `tombi_view.c` builds for both: on Windows it reads the frames from `GetStdHandle(STD_INPUT_HANDLE)`
  and gets the audio/pad pipe handles as `--audio-handle N --pad-handle N` (fds 3 and 4 cannot be inherited by number);
  `port_spawn` in the Win32 runtime makes the three pipes with `CreatePipe` and runs it with `CreateProcess`. New viewer
  options, passed through `tombi --viewer-arg X` (repeatable): `--scale 1..6`, `--fullscreen` (F11 toggles it), `--keys
  up=Up,cross=X,...` (SDL key names per PS1 button). The pad pipe is read with `PeekNamedPipe` (non-blocking).
- **Files.** `port_open/create/pread/pwrite/filesize` on `CreateFile/ReadFile/WriteFile` (OVERLAPPED offsets); the FIFO
  options (`--pipe-frames`, `--pipe-audio`) write plain files on Windows (use `--window`). `timeBeginPeriod(1)` + `Sleep`
  + a spin for the last millisecond pace the vblanks; `QueryPerformanceCounter` is the clock.
- **Status (checked under wine32 9.0).** The exe boots, maps the memory, loads MAIN0.EXE and the disc, performs the same
  CD reads as the Linux build over the first 1500 frames and reaches `titleSequenceTask`. Wine keeps its own data in
  0x110000-0x1FFFFF, so the KUSEG mirror is partial there (the warning lists the missing 64 KB blocks); the load-list
  walk reads a KUSEG pointer through KSEG0 (`fixes.txt`, queueLoadList), so this is harmless as far as tested;
  `--kuseg-strict` stops at the warning instead. The Linux build is unchanged (`cmake -S port -B build/port` as before).
- **Disc diagnostics.** `port_cd_open` tells raw (2352) from cooked (2048) images by the sync pattern `00 FF*10 00` or
  the ISO9660 PVD (the size alone misleads with a multi-track dump in one file), logs size and sector count, reads
  sector 16 and checks `CD001`. A failed read prints the file, offset, size, sector count, the OS error and "beyond
  the end of the image"; a Setloc outside the image names the `FILE_LINKS` id it came from (a garbage load record)
  or the parameter's address. `--trace-cd` logs every Setloc and sector read. A `.cue` is resolved relative to its
  own directory with either separator.
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
game's stderr in `tombi-port.log` next to the exe, shown in the window: the last 8 lines, with any earlier `[port]
warning` / `FATAL` lines kept above them), a *Copy log* button (the whole log file to the clipboard, for a bug
report) and the exact command line. *Mods* and
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

- Run `tombi.exe` on real Windows again after the KUSEG rewrite (the first run on Windows 10 crashed in the title
  video through the partial mirror, see "The KUSEG mirror is not load-bearing any more"): the log should show no
  `[port] warning` about the mirror being load-bearing and no emulated-access tally at exit, and the title video
  should not stutter.
- The *button prompts* mod: the glyph rects of the manifest (see "Button prompts" above); the art and the
  `--patch-tex` mechanism are done.
- The stall at vblank ~443 noted above.

## M3: native-pointer port (`port2/`)

**Decision.** The fixed-address i386 runtime of `port/` (RAM mapped at 0x80000000, SIGSEGV call trampoline,
KUSEG mirror, MIPS interpreter) is abandoned as the way forward. The new port compiles the game C as ordinary C with
**native pointers and ordinary, relocatable global data**: no fixed addresses, no exception-handler tricks, no
mirror, no interpreter. Only the GTE, GPU, SPU, CD and pad are emulated, linked in-process (the shims of `port/`
are to be carried over, with SDL2 directly instead of the viewer process). This is the SotN / PsyCross model.
Phase 1 (this section) is the **generator** and a first build of all of MAIN0 + X000 that compiles and links with
zero undefined symbols. The area and SYSTEM overlays, the runtime and the shims are the next rounds.

### Design

`port2/tools/gen2.py` works in two steps, like `gen_port.py`, and reuses its source filter (it imports
`port/tools/gen_port.py` and swaps two functions):

1. **`sources`**: `gen_port.sources()` writes the filtered copies of the 1332 MAIN0+X000 files (GTE macros to C,
   asm dropped, per-file `name__ADDR` renames, PORT_ARG forwarding, `port/fixes.txt`) into `build/port2/gen/src`.
   Two of its rewrites are replaced:
   - *Retail-named data definitions* (`D_xxxxxxxx`, `DAT_`, `PTR_`, and any name the matching build PROVIDEs)
     become plain `extern` declarations. The one copy of every retail datum is the generated **data image**
     (below); a C initialiser is kept only as an unused `static port_copy_` so the text stays balanced.
   - *The OT-link idiom*: `*(u_long *)otz = (u_long)p;` becomes `port_otlink(p)` and `((u_long)p) & 0xFFFFFF`
     becomes `port_otaddr(p)` (both in `port2/include/port_prelude.h`). The KSEG0 `| 0x80000000` of `port/` is gone.

   Then the **address resolver** runs over every generated source and over copies of the include trees (headers
   are materialised as real files where they contain literals, never written through to `include/`): each
   `0x80xxxxxx` / `0xA0xxxxxx` / `0x1F80xxxx` literal outside comments and strings becomes
   `PORT_ADDR(port_a_<symbol>, offset)`: the native function starting at that address, the data symbol at (or the
   nearest one before) it, or the data-image blob plus offset. `PORT_ADDR` yields an integer, so the surrounding
   cast (`(int *)`, `(void (*)(u8 *))`) stays as written, also in static initialisers. `port2_syms.h` declares each
   symbol as `extern unsigned char port_a_X[] __asm__("X")`, which never conflicts with the game's own typed
   externs of `X`. Values `>= 0x80200000` (`0x803Cxxxx`) and round values (`0x80020000`) are constants, not
   addresses; `0x80000000` is the KSEG0 bit. A literal inside a natively compiled function (a handler entry that
   the matched C has as a label) resolves to the image bytes and is listed in `literals.txt` as a *label*.
   Scratchpad literals and `D_1F80xxxx` names resolve into `port_scratch[0x400]`; the SPAD/G164-style macros of
   `include/tomba` are rewritten in the header copies, so no override header is needed.

2. **`link`** (after compiling, from `nm` of all objects):
   - **Data image** `gen/data.c`, generated at build time from the user's `game/MAIN0.EXE` and `game/AREA00/X000.BIN`
     (game data: it stays in the build directory). RAM is one relocatable 2 MB array (`port_ram`, since phase 2;
     phase 1 had 9 arrays) with these regions aliased into it at their retail offsets, which keep the retail
     layout: `port_bios` (0x80000000, zero), `port_main0_image` (0x80010000, the EXE bytes), `port_main0_bss`,
     `port_x000_image` (0x800E8028, X000.BIN), `port_x000_bss`, `port_heap` (0x80140000..0x801FD800, the load
     buffers), `port_hiram` (0x801FD800, the E-table), `port_scratch`, `port_io` (0x1F801000, plain memory until the
     shims take the registers). Every aligned word of a data range that holds the address of a function start
     becomes `(unsigned int)&func` and one that holds the address of any RAM byte becomes `PORT_ADDR(region, off)`:
     relocations, so the image moves with the binary. Words inside code units are kept raw (switch jump tables and
     the unrolled-loop entries of the matched code, unused natively). One array per region rather than one C
     definition per symbol because the game's externs disagree on the type and size of the same address and
     interior pointers (`D_80098C48` into the table at `D_80098C44`) must keep working; peeling single symbols
     off into typed C definitions is possible later, one at a time.
   - **Aliases** `gen/syms.ld` (symbol assignments only, no addresses): every retail data name the objects reference
     becomes `name = port_<region> + offset;`, every renamed or address-named function (`fa__8013839C`,
     `FUN_80063ffc`) becomes `name = native;`.
   - **Stubs** `gen/stubs.c`: every function nobody defines (and that the 32-bit libc does not provide) gets
     `int name() { port_stub_hit("name"); return 0; }`, classified as Psy-Q/BIOS, game-asm (the pad driver files
     `gen_port` skips), *overlay-target* (an address in 0x800E8028..0x80140000 that is a function start in another
     overlay but not in X000: MAIN0 dispatches to it by address; the next round makes that an explicit switch on
     the resident overlay) or label. Nothing is resolved at run time: a function pointer in data is a relocation
     to the native function, a call through a literal is a call to the symbol.
   - `gen/report.txt` with the counts, `gen/literals.txt` (unresolved and label literals), `gen/literals_resolved.txt`.

**Pointer width.** Phase 1 is 32-bit (`-m32`, normal glibc, `-funsigned-char -fwrapv -fno-strict-aliasing
-fno-delete-null-pointer-checks -fno-omit-frame-pointer -O1`), because the game stores pointers in `int`/`u_long`
fields and in the data image's 32-bit words. The OT links are 24-bit in the GPU tag, and the game masks pointers with
`0xFFFFFF` when it links primitives: the binary is linked with `-Wl,-Ttext-segment=0x100000`, so the whole data
image sits below 16 MB and the mask is the identity (checked at start-up by `port_check_layout`; `tombi2` prints
`layout ok (below 16 MB)`). **64-bit later:** `port_otlink`/`port_otaddr` become an index into a primitive arena
(or the links stay 32-bit offsets from a base, as PSY-Z's `OT_TYPE`), and the data image needs pointer-holding words
widened, which means typed definitions for the structures that hold pointers; everything else of the generator is
width-independent.

### Phase 2: the game runs (title screen, AREA00 gameplay, SDL2 window)

`port2/` now boots the real game natively: `build/port2/tombi2` opens an SDL2 window (paced at 50 Hz, audio
through the SDL callback, keyboard / game controller on pad 1; `--headless` runs as fast as possible without one)
and reaches the title screen and AREA00 gameplay with the same inputs as `port/`:

```
build/port2/tombi2 --headless --frames 1500 --dump-frames out --pad-script 1000:start      # title (frame 1000), AREA00 (1490)
build/port2/tombi2 --headless --frames 4000 --dump-frames out --pad-script 1000:start,1300:cross:3x45/30,3200:right:400
build/port2/tombi2 --window                                                                  # the same in a window
```

Every frame of those runs is byte-identical to the PNGs `port/` dumps for the same script (checked frame by frame
over the 4000-frame run), so the native-pointer build is a drop-in for the fixed-address one.

What was added over phase 1 (all of it in `port2/runtime`, `port2/shims`, `port2/tools/gen2.py`):

- **Runtime.** `main.c` has the game's `main()` in C (from `port/runtime/main_port.c`) on the `port_scratch`
  array and the command line; `threads.c` the coroutines (unchanged: plain stack switching, the task entry is a
  native pointer now); `sdl.c` the window (RGB24 streaming texture of the DISPENV area, every vblank), the audio
  device pulling the mixer ring, and the pad; `lowmem.c` a SIGSEGV handler for the game's reads and writes
  through null pointers (object fields of objects it has no pointer to yet: the BIOS kernel area on the PS1):
  the faulting instruction is re-executed against `port_ram` (the `port_low_emulate` of `port/`), and SIGFPE
  gives the R3000's division-by-zero result; `main0_handwritten.c` the five asm-only MAIN0 functions inside
  coarse units (message box, floor/wall probes, event actors) from `port/`, found by gen2 by their
  `func_XXXXXXXX(` definitions.
- **Shims** carried over on native pointers: `psyq.c` (CD reader with `.cue`, events, VSync, libgte helpers,
  movies skipped), `gpu.c` (software GPU; `DrawOTag` follows native links, the image is below 16 MB), `sound.c`
  (SPU mixer + sequencer into a ring; `--dump-audio`), `card.c` (memory cards; the BIOS file system is
  `port_bu_open/read/...`: gen2 renames the game's calls so the host libc's `open`/`read` stay the host's),
  `pad.c`, `libc.c` (Psy-Q `rand`, quiet `printf`, the forward-byte `bcopy` lzDecompress relies on). The game
  objects link against the ordinary 32-bit glibc for everything else.
- **Overlay dispatch.** gen2 compiles the 16 area overlays and the 8 SYSTEM overlays with `gen_port`'s prefixes
  (`X0nn_`, `O0PTSUB0_`, ...) and their dispatch tables (`gen/funcmap.c`: `port_ovlmap`, `port_areamap`,
  `port_areas` with the signature words). Every function start of the overlay region, over all overlays (2010
  addresses), gets a thunk in `gen/thunks.S` (`pushl $addr; call port_resolve_call; addl $4,%esp; jmp *%eax`):
  `runtime/ovl.c` picks the native code of the resident overlay (header id for the SYSTEM set, signature words
  for the areas, X000's shared code as the fallback) with a small direct-mapped cache. Every name MAIN0 calls
  into the region (the 296 overlay-target stubs of phase 1, the 226 varying X000 functions, address names of
  overlay functions) is aliased to its thunk in `syms.ld`, and data words pointing at such addresses become
  `&port_thunk_XXXXXXXX`.
- **Relocation of CD reads.** A file the game reads into RAM arrives with retail pointers: `CdRead` relocates
  the load lists (`SYS/LD0SYS.BIN`, `SYS/LD0ARnn.BIN`: FILE_LINKS ids 0..19, read into MAIN0's data) and every
  file read into the overlay region (`port_relocate_overlay`): each aligned word outside the overlay's code ranges
  (`gen/addrmap.c`, from `units()`) that holds a RAM address becomes the native function (`port_addrmap`), the
  thunk (`port_thunkmap`) or the data pointer. Assets read into the heap / load buffers are left alone
  (relocating them corrupted the compressed spawn records). A load that lands at `0x80000000` (G?OVER's record)
  goes to the overlay buffer.
- **One RAM array.** The 2 MB of RAM is one `port_ram[0x80000]` in the retail layout (designated initialisers
  for the MAIN0 and X000 bytes, zero elsewhere); the region names of phase 1 are link-time aliases into it
  (`gen/regions.ld`), because a CD read may cross a region boundary (LD0SYS.BIN lands across the end of MAIN0's
  `.data` into its `.bss`). The scratchpad and the I/O page stay their own arrays.
- **Generator fixes found by running:** integer suffixes on address literals (`0x80000000u` was never matched);
  `p | 0x80000000` (an OT link back to a pointer) is the identity on native pointers and is rewritten to `| 0u`;
  the round literal `0x1F800000` (`PSX_SCRATCH`) is an address; round data words (`0x80040000`: flag halfwords
  of the animation tables at the start of MAIN0's data) are constants, not pointers (relocating them gave the
  player sprite the wrong cell and CLUT); duplicate area definitions (`X005_updateBacoTownNpc`,
  `X011_updateFactoryNpc`) are weak; `P_1F8003xx` names alias into the scratchpad.

`build/port2/gen/report.txt` after phase 2:

| | |
|---|---|
| Sources compiled | 3138 (MAIN0, X000, 16 areas, 8 SYSTEM overlays) + runtime + shims |
| Address literals | 938 resolved (3 labels inside native functions), **0 unresolved**, 300 constants skipped |
| Data image | 7540 pointer words to data, 749 to functions / thunks; 2423 words point into the overlay region at addresses that are no function start, 2698 into code interiors (jump tables, unrolled-loop entries) |
| Data aliases / function aliases | 3371 / 876 |
| Dispatch thunks | 2010 (514 names aliased to one) |
| Stubs | **92** (was 513): Psy-Q internals 82 (`PRNT_OBJ_*`, `SPRINTF_OBJ_*`, `_addque`, ... reached only from Psy-Q code we keep; 13 are hit at boot: `SetFogNear`, the `_SpuInit`/`SsSetTableSize`/reverb family), pad-driver internals 8 (`_padSioRW`, ...: `pad.c` provides the API above them), overlay labels 2 |
| Calls into the overlay region without native code in the 4000-frame run | 0 |
| Emulated null-pointer accesses in the 4000-frame run | a handful per spawn (`spawnObjectFromRecord` reading the previous object's position through `DAT_8009bd9c = 0`) |

Build: `cmake -S port2 -B build/port2 -G Ninja && cmake --build build/port2 -j 8` (needs `gcc-multilib`, the
32-bit `libsdl2-2.0-0` runtime library and `python3 tools/build_full.py` run once for the overlay link scripts;
Debian's per-architecture `SDL_config.h` is taken from the host's with the pointer size corrected, since only
the 32-bit library is needed). The game data (`game/`, `game/disc/`) is read at build time (the image) and at
run time (the disc).

### Phase 3: the Windows build (tombi2.exe, tombi-port.exe)

`port2/` has no fixed addresses, so the Windows build is a plain cross-compile of the same sources with
mingw-w64 (`i686-w64-mingw32-gcc`), linked against the normal mingw CRT and the SDL2 mingw development
tarball; nothing of the exception-handler / KUSEG / COFF-binding fights of `port/` ("Windows build" above)
remains. The user runs `tombi-port.exe` (the launcher) or `tombi2.exe` directly; we test it under wine.

```
cmake -S port2 -B build/port2-win -G Ninja -DCMAKE_TOOLCHAIN_FILE=port2/cmake/mingw-i686.cmake \
      -DPORT_SDL2_MINGW=/path/to/SDL2-2.30.11/i686-w64-mingw32 && cmake --build build/port2-win -j 8
cp /path/to/SDL2-2.30.11/i686-w64-mingw32/bin/SDL2.dll build/port2-win/
wine build/port2-win/tombi2.exe --headless --frames 4000 --dump-frames out --pad-script 1000:start,1300:cross:3x45/30,3200:right:400
xvfb-run wine build/port2-win/tombi2.exe --window --frames 2600 --pad-script 1000:start
```

**Results under wine 9.0.** The 4000-frame headless run dumps 400 frames **byte-identical** to the Linux
build's (same script, every 10th frame compared); `--window` under Xvfb opens the SDL2 window and shows the
boot screens, the title and AREA00 gameplay at 50 Hz (the audio device is absent there; SDL reports it and
the game runs without sound). `--test-null` (below) passes: the null-page write lands in `port_ram` and
`100/0` gives the R3000's `-1`.

What the cross build needed (`port2/CMakeLists.txt` `if(WIN32)`, `port2/cmake/mingw-i686.cmake`,
`port2/tools/gen2.py`, `port2/runtime/{lowmem,threads,host_win32,sdl}.c`):

- **Symbol prefix.** Win32 C symbols carry a leading `_`. gen2's link step strips it from `nm` output and
  adds it back when it writes `syms.ld` / `regions.ld` (`_DAT_8009c971 = _port_main0_bss + 0x3971;`: the PE
  linker accepts the same symbol-assignment script) and `thunks.S` (`_port_thunk_XXXXXXXX`, `call
  _port_resolve_call`, no ELF `.type`). The asm-named declarations (`port2_syms.h`'s `port_a_X`, and the
  nine `extern u8 V asm("DAT_...")` aliases in the game's own sources) go through `PORT2_ASM(X)`, which
  prepends `__USER_LABEL_PREFIX__`, so one generated tree serves both targets. Stdcall imports (`_Name@N`,
  `__imp__Name`) are skipped, and the libc set comes from the mingw CRT archives (`libmsvcrt.a`,
  `libmingwex.a`). `threads.c`'s `port_ctx_switch` uses the same prefix macro for its asm label.
- **Layout below 16 MB.** The PE image is based at 4 MB with ASLR off (`--image-base 0x400000
  --disable-dynamicbase`), and the zero regions of the image (`port_scratch`, `port_io`) are emitted with
  an initialiser so they sit in `.data` right after `port_ram` instead of behind the runtime's 10 MB of
  `.bss` buffers (VRAM, PNG buffers, SPU RAM); `port_check_layout` prints `layout ok (below 16 MB)` as the
  second log line. (On Linux the same change moved nothing that matters.)
- **Fault handlers.** `lowmem.c` keeps one x86 operand decoder; on Windows a vectored exception handler
  (`AddVectoredExceptionHandler(1, ...)`) does what the SIGSEGV / SIGFPE handlers do: `EXCEPTION_ACCESS_VIOLATION`
  below 0x10000 re-executes the instruction against `port_ram` from a `VirtualAlloc`'d executable page,
  `EXCEPTION_INT_DIVIDE_BY_ZERO` yields `LO = rs < 0 ? 1 : -1`. Any other access violation prints the
  nearest native symbol, the return addresses inside the image and the emulated-access tally, then lets
  Windows end the process (the launcher shows those lines).
- **Coroutines.** The stack switching asm is the same; on Windows the TIB's stack base / limit (`fs:[4]`,
  `fs:[8]`) are set to the coroutine's stack on every switch (the main thread's saved on the first), since
  the exception dispatcher checks them (the game's tasks fault on null pointers on their own stacks). The
  stacks are static arrays, so gcc's `___chkstk_ms` probes touch real memory.
- **Host bits.** `host_win32.c`: `QueryPerformanceCounter` clock, `timeBeginPeriod(1)`, `Sleep()` to 2 ms before
  the deadline then yield (the vblank schedule of `psyq.c` is absolute, so the rounding does not accumulate);
  `_mkdir`, `fseeko64/ftello64`, backslashes accepted in the cue sheet and the save folder; `windows.h` is
  kept out of `main.c` (it claims the Psy-Q names `EnterCriticalSection` / `OpenEvent`). `libc.c`'s quiet
  `printf` is the CRT name itself (`__USE_MINGW_ANSI_STDIO` is set for the runtime objects only). No SDL2main:
  `tombi2.exe` is a console program with the plain `main()` (`SDL_SetMainReady`), the launcher runs it with
  `CREATE_NO_WINDOW` and captures stderr into `tombi-port.log`.
- **Options for the launcher / mods** (also on Linux): `--scale N`, `--fullscreen` (F11 toggles),
  `--keys up=Up,cross=X,...` (SDL key names), `--patch-tex FILE` (button prompts), `--show-fps [1|2]`,
  `--test-null` (fault-handler self-test: exit code 0 when both emulations work on this host). `--poke`,
  `--poke-every`, `--pad-script`, `--menu` were already there.
- **Build id.** `port/cmake/build_id.cmake` writes `gen/build_id.c`; the first log line is
  `[port2] build <sha>[+] <date>` (`+` = modified tree).

**Launcher** (`tools/port_launcher.py`, same window as before: disc `.bin/.cue/.iso`, save folder, scale,
fullscreen, 50/60 Hz, *Mods* and *Controls* tabs from `tools/port_mods.py`, *Copy log*, build id in the title):
it runs `tombi2.exe --disc <bin> --window --scale N [--fullscreen] --show-fps 2 [--save-dir DIR]` plus the
mods' options (`--keys` directly now: there is no viewer process). The disc check (MAIN0.EXE extracted and
sha1-compared) still runs before the first Play: it is how the launcher tells a wrong image apart.

**CI** `.github/workflows/port2-exe.yml`: job `game` (ubuntu, `gcc-mingw-w64-i686`) fetches the owner's disc
image like `report.yml`, runs `tools/build_full.py` (the generator takes the retail addresses from the
matching build), cross-builds `tombi2.exe` and uploads it with `SDL2.dll` (artifact `tombi-port-game`); job
`launcher` (windows-latest) runs `tombi2.exe --headless --test-null` on a real Windows, packs the launcher
with PyInstaller into `tombi-port.exe` (artifact `tombi-port-windows`: `tombi-port.exe`, `tombi2.exe`,
`SDL2.dll`, `THIRD_PARTY_NOTICES.md`) and commits `dist/windows/tombi-port.exe` to master; `zip-exe.yml` adds
it to `tombi-tools.zip`. No game data is in any artifact (the MAIN0/X000 bytes compiled into `tombi2.exe` come
from the owner's own disc, as the retail addresses of the matching build do).

**Running on Windows.** Download `tombi-port.exe` (or the `tombi-port-windows` artifact), start it, pick the
disc image (`.bin` or `.cue` of Tombi! PAL SCES-01331), *Play*. `tombi-port.log` next to the exe (also *Copy
log*) should begin with:

```
[launcher] build <sha>
[port2] build <sha> <date>
[port2] native pointers, 32-bit: data image 0055a000..00685f78, scratch 00547000, layout ok (below 16 MB)
[port] disc image C:\...\Tombi.bin: 492370032 bytes, raw 2352 sectors (209341), sync pattern found
[port2] window 640x512 (scale 2), audio 44.1 kHz
[port2] game main: init done, entering the main loop
[port] frame 100: ...
```

What to look for: `layout NOT below 16 MB` (the loader relocated the image: report the two addresses), a
`[port2] access violation: eip=... (<symbol>+0x...) fault address=... (read|write, frame N, thread T)` line
with its stack (the first native-code bug on Windows: the symbol and frame number locate it), `[port2]
stub: <name>` lines beyond the 13 Psy-Q ones hit at boot, `[port] fps:` lines far from 50.00 (pacing), and
`emulated accesses to the BIOS kernel area` with a site count in the hundreds (a null-pointer path the
Linux runs never took). Without a window/audio device the log says so (`no audio device: ...`). To run the
game directly: `tombi2.exe --disc Tombi.bin --window` from a folder holding `SDL2.dll`; `tombi2.exe --headless
--test-null` checks the fault handlers alone (from the main thread and from a coroutine stack: a task's null
read, byte and dword writes, and a division by zero; exit code 0 when all pass).

**A silent hang** (the first real-Windows run stopped at frame ~3300 with no access-violation report and no
exit) is what the watchdog (`runtime/watchdog.c`) is for: a host thread that, when the vblank counter has not
advanced for 3 s, suspends the game thread (`SuspendThread` + `GetThreadContext`; `SIGUSR1` on Linux) and
prints `[port2] watchdog: no vblank for 3 s at frame N: eip=... (<symbol>+0x...) esp=... thread T; stack:
...`, then `watchdog: last shim calls (oldest first): vblank(N)@N ChangeTh(1)@N ... VSync/DrawSync/CdControl/
CdRead`, then the null-page tally, and lets the game run on (at most three reports per run). stderr is
unbuffered, so the launcher's log is complete up to the hang. What the report tells: `eip` inside a game
function (`FUN_...`, `player...`) with the same `eip` three times = a spin in game code (a wait loop whose
condition never comes on Win32: look at the last shim calls); `eip` "outside the game code" with
`port_sleep_us`/`Sleep` on the stack = the pacing sleep never returns (clock); `thread T` > 0 with a `ChangeTh`
as the last shim call and an `emulated accesses` tally that grew = the fault emulation on a coroutine stack
(the TIB-bounds path in `threads.c`); no watchdog line at all while the log stops = the process died or was
blocked outside the game thread (the audio callback, the window's message loop). `--stall FRAME:MS` with
MS >= 3000 is the self-test of the watchdog (`--headless --frames 120 --stall 100:4000`).

### Open points / next

1. The 2698 image words and the "code labels kept" of a loaded overlay (2318 for X000) point inside coarse
   units: handler entries without a native name. None is reached in the title / AREA00 runs; the first one hit
   will need the function split out in `notes/functions_x000.csv` (or a hand translation registered as
   `main0_handwritten.c` does).
2. Assets are never relocated; a data file that carries absolute pointers (beyond the load lists) would show up
   as a bad pointer at use. The null-pointer emulation covers reads/writes of page 0 only; a fault elsewhere is
   reported with the nearest native symbols and the return addresses on the stack.
3. The 13 Psy-Q stubs hit at boot are the fog / SPU-init / reverb setters (no effect in the software SPU
   yet); `ScaleMatrix` and the other 69 are unreached.
4. 64-bit build: the OT links are 24-bit addresses, so the image must stay below 16 MB (`-Ttext-segment` /
   `--image-base` + `port_check_layout`); a later phase replaces `port_otlink/port_otaddr` with an index into a
   primitive arena and widens the pointer-holding words of the image.
6. Windows: tested under wine only (frames identical, window shows the game); the first real-Windows run
   (build 2c9db85) booted and played AREA00 and hung silently at frame ~3300 while walking (no fault report,
   no exit; wine runs 4000 frames). The build after it carries the watchdog (above), re-reads the main thread's
   TIB stack bounds on every coroutine switch (a stale StackLimit restored after the stack had grown), makes
   `--test-null` fault from a coroutine stack too, and makes the sound ring strictly single-producer /
   single-consumer (no index shared for writing between the game thread and the audio callback). The next
   Windows log says where the game thread is when it stops.
5. Movies (MDEC) are skipped as in `port/`; save states of `port/` (`--save/--load`) are not carried over.
