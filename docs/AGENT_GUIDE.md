# Agent guide: practical knowledge for taking over

Read `CLAUDE.md` first (rules), then `docs/STATUS.md` (what is open). This file holds what a session learns the hard
way and would otherwise lose: how the user works, recipes for headless checks, build and CI pitfalls. `docs/PORT.md`
is the reference for each feature (it is long: search it).

## 1. Working with the user

- Replies in Spanish, short; everything written to the repo or into images in English (see CLAUDE.md).
- **Name rooms by place, not by number.** "AREA03 s2" means nothing to the user. Use the names of the debug area
  select (`port2/runtime/main.c`, `port_dbg_area` / `port_dbg_sec`, English and Spanish), e.g. "Montaña Fénix, cuevas
  de lava (AREA03 s2)", and always show a picture.
- **Pictures, not descriptions.** For a visual change send real headless renders. For a set of rooms make one
  numbered sheet (each row: number, place name, what changed, 4:3 on the left, the wide size on the right, collision
  view on when it helps) and keep the same numbers while the user reviews them; the user answers by number.
- **Room by room.** The user reviews widescreen work one room at a time and decides each case. Do not apply a rule to
  many rooms and push it as done: show the candidates, apply what was approved. "Widening the room does not always
  mean moving its limits."
- Decisions already taken (widescreen): an exit through a door, a gate or a fixed opening stays where it is; the
  AREA09 mansion stairs keep their 4:3 exits (the stair leaves the frame vertically); rooms narrower than the wide
  frame keep their exits. See docs/PORT.md "Widescreen exits" for the per-room list.
- **Small means small.** When the user asks for a small change to an existing asset or behaviour, change exactly that
  (the CARGANDO "R" rework went badly because the letter was redesigned instead of edited by a few pixels).
- Say plainly what was tested (PC headless, which widths, which build) and what was not (the device). The user tests
  on an AYN Thor (Android, arm64 = the wasm2c build) and an Odin 3.
- The user does not review pull requests: verified work goes to master. Releases only when asked.

## 2. Source and build pitfalls

- **`src/` is the matching decompilation: do not edit it for port behaviour.** Port changes to game code go in
  `port/fixes.txt` (`<src path> | <exact text> | <replacement>`; every occurrence in that file is replaced; rules
  apply in order, so a later rule can match text an earlier one inserted; `@version usa` ... `@version all` limits a
  block to some versions). Declarations the replacements need go in `port2/include/port_prelude.h`, with a stub in
  `port/include/port_prelude.h` (the old port includes the same fixes). Runtime code goes in `port2/runtime/`.
- The filtered game sources are made at **CMake configure time** (`port2/tools/gen2.py sources` into
  `build/port2/gen/src`). After editing `src/`, `port/fixes.txt` or adding a file to `port2/runtime/` (globbed), run
  `cmake -S port2 -B build/port2` before `make`; a plain `make` may keep the old generated files.
- Touching anything in `src/` makes `build/full` "stale": `python3 tools/build_full.py -j 8` must end with `OK` (every
  sha1 OK) before the generator accepts it; `PORT_STALE_FULL=warn` only skips the check (for a quick try).
- Merges from master can add runtime files: a link error like `undefined reference to port_ws_floor3d_blocks` means
  "re-run cmake".
- The device build: `PORT_STALE_FULL=warn python3 port2/wasm/build.py -j 8` (~5 min) gives `build/wasm/tombi2w`, the
  same 64-bit code as the Android arm64 library, with the same command line as `build/port2/tombi2`. Run the same
  headless test on both when a change touches game logic.
- Memory the game reads must be in game RAM or the port's data image; the collision view checks pointers with
  `port_is_ram_ptr` (a buffer of the runtime, like the WHM copies of `ws_exits.c`, needs its own check).
- Layout-only or rename-only edits of port2/runtime / port2/shims: prove them with `python3 tools/same_code.py
  --base <ref>` (C tokens equal modulo layout, comments and safe renames, and the same i386 objects).  Splitting
  `int a = 1, b = 2;` into two declarations counts as a change; struct members, headers and code under
  platform #ifs keep their names.
- The cloud container's disk is small: delete scratch builds (`build/cctest`, extra dumps) when done.

## 3. Headless recipes

- Debug area select: `--skip-movies --poke 900:1F8001B4:1 --poke 1250:8009C960:AREA:2 --poke 1250:8009C962:SECTION:2
  --pad-script 1000:start,1300:circle:3` (hex values). Gameplay starts at about frame 1400-1500.
- AREA19 (the Dwarf Village in blossom) is loaded with area 0x13 but runs as area 2 section 1: the game state reads
  2:1, so code keyed on 2:1 applies to both villages.
- Addresses (PAL): GAME area / section 0x8009C960 / 0x8009C962; PLAYER object 0x800A6038 (whole x 0x800A604A, y
  0x800A604E, z 0x800A6052; +0x40 / +0x44 point at the path / depth coordinate: in "swapped" sections such as AREA00
  s4, AREA01 s2/s3, AREA04 s1 the path is z); CAMERA_SCROLL 0x800A4550 (minX/maxX/minY/maxY at +0x2C..+0x32);
  per-section camera bounds `SECTION_CAMERA_BOUNDS` (pointer table at 0x80078B7C, one per area, 4 shorts per
  section). `--dump-ram FRAME:FILE` writes RAM in retail order (pointers inside it are native).
- Placing Tomba: `--poke FRAME:800A604A:X:2` a few times (one poke can be undone by the room's spawn code);
  `--poke-every` pins a value every frame (Tomba cannot walk then). At most 32 pokes per run (`port2/shims/psyq.c`).
- Some rooms cannot be tested from the debug select: a dialogue that never closes (the mermaid, AREA06 s2), an event
  that resets Tomba (AREA03 s0/s4), a pit under the poked position. Say so instead of guessing.
- `--show-collision` draws the WHM lines, object boxes and room exits; `PORT_CVLOG=1` (with it) prints one line per
  frame, `CVLOG area section H=.. Y=.. D=..`, used by `tools/ws_exit_sweep.py`.
  It prints nothing in the wasm2c build (`build/wasm/tombi2w`): there compare positions with `--dump-ram FRAME:FILE`
  (PLAYER x / y at 0x800A604A / 0x800A604E).
- In side view one world unit is about one screen column at Tomba's depth (measured), so the widescreen pad in
  columns is also the distance in units.
- Picking the moment of a room change in a frame series: the last frame before the fade (brightness drops).
- The widescreen strips are cached (docs/PORT.md "Widescreen strips cache"): in memory and in `ws_cache/` next to the
  memory cards (`--save-dir`, else the repo root for `--data data`).  To time a generator or to compare a changed one,
  use `--no-ws-cache` (or a fresh `--save-dir`); `-v` says "(made)" / "(from the cache ...)".  A rebuilt
  ws_backdrop.c never takes an older build's strips (its build stamp is in the key).

## 4. Widescreen exits (2026-11-16/17)

Code: `port2/runtime/ws_exits.c` (+ `ws_exits.h`), `port/fixes.txt` block "Widescreen exits", data
`tools/colview_exits.json` (163 exit boxes read from the area overlays; `"ws": [a, b]` = h0 / h1 move by a / b pads)
generated into `port2/runtime/colview_exits.inc` by `python3 tools/colview_exits.py` (`--check`). The `WALK` table in
`ws_exits.c` opens a side without an exit. To change a room: edit the JSON line by hand (keep one object per line),
regenerate, rebuild, run `python3 tools/ws_exit_sweep.py --rooms A:S` (4:3, 16:9, 21:9, 32:9: same destination, Y on
the floor), make the numbered picture sheet, let the user decide. Details and the per-room decisions: docs/PORT.md
"Widescreen exits".

## 5. CI and releases

- No `gh` CLI: read CI with `curl -s "https://api.github.com/repos/afsenovilla/Tombi-Decomp/actions/runs?head_sha=SHA"`
  and trigger workflows with the GitHub MCP tool `actions_run_trigger` (`run_workflow`, `release.yml`, ref master,
  inputs `{"version": "X.Y.Z", "include_tools": true}`). Job logs: the MCP `get_job_logs` tool (save to a file, grep).
- Release checklist: (1) bump `VERSION` (the Android launcher shows it; v0.5.8 went out with 0.5.7 and had to be
  republished as v0.5.9), commit, push; (2) wait for both "Native port (Windows exe)" and "Native port (Android APK)"
  runs of the master head to be `success`; (3) check master did not move since (another agent may push); (4) trigger
  the release; (5) update "Last release" in docs/STATUS.md. Release title is "Tombi port X.Y.Z" (the workflow sets it).
- Timing: Windows/Linux exe ~8 min; Android APK ~22 min cold. The game sources were never ccache hits until
  2026-11-16 (their `#line 1 "src/..."` was resolved from the build directory and failed; `port2/CMakeLists.txt` now
  links `src` into the build directory). The APK job saves its ccache even when cancelled; later runs should be
  much faster once the cache is warm.

## 6. Nintendo 3DS version only (2026-11-24)

Everything in this section is about the 3DS build (`port2/n3ds/`, `.github/workflows/3ds.yml`); none of it applies to
the PC or Android builds.

**How the user tests.** An Old 3DS (sometimes a New 3DS), the `.3dsx` from the Homebrew Launcher, widescreen 5:3,
60 Hz, improved Spanish, no movies. Never suggest 50 Hz. Tell the user the moment the `n3ds` job of a push is green
(do not wait for the `emulate` job): poll `actions/runs?head_sha=SHA` with a background script and report at once.
A 3DS-only release goes to a `dev` branch as a pre-release, never to master's release flow.

**Builds and artifacts.** Off master, `3ds.yml` also builds a function-profile build: artifact
`PERFIL-tombi-3ds-lenta`, file `tombi-PERFIL.3dsx` (it has the same build id as the normal one and runs slower on
purpose; say clearly which artifact to take). Its log has `[prof]` lines: name the addresses with
`arm-none-eabi-addr2line -f -e tombi.elf ADDR` (the ELF is in the artifact). Artifacts can be fetched from the
session with the GitHub MCP tool `actions_get` / `download_workflow_run_artifact` (a short-lived URL for curl).
Instrumentation inflates small functions (each call gets enter/exit hooks): read shares, not absolute times.

**Reading a device log.** The per-second `[port] fps:` line ends with `lowmem: N` and `room A:S`; aggregate it per
room (fps, `game` ms, `draw` ms, seconds below 58 fps) and compare builds room by room. `[3ds] profile of N ordering
tables` gives the cost of a drawn frame (decoder vs backend vs the GPU's own part).

**Null-pointer accesses can crash an Old 3DS's kernel.** The port emulates the game's reads/writes of the PS1
kernel area (page 0, through null object pointers) from libctru's user exception handler
(`port2/runtime/lowmem_arm.inc`): every access is a kernel exception dispatch. A few a minute are harmless; tens to
hundreds a second (a site that faults every frame) eventually stop the console in a kernel exception (Luma crash
dump: ARM11, SVC mode, `pc=0xfffffffe`, `lr=0`; the game's log just ends). It is reproducible in the same room. Azahar
does not reproduce it (its kernel is emulated). Fix such a site at its source in `port/fixes.txt`: point the null
pointer at `port_page0()` (the page-0 image in the port's RAM: the same bytes the handler used, so identical
behaviour), e.g. `chainedPartPlaceChild` (Forest: hut entrance) and `playerCheckPerchLanding`. Any `lowmem:` site in
the tens per second on a device log needs the same treatment. Never tell the user a crash is "the system's" without
proof: a crash in the same place twice is the port's.

**Crash dumps** (`crash_dump_*.dmp`, Luma): 11 little-endian words of header (magic, magic, version, processor,
exception type, ..., register size, code size, stack size, extra size), then r0-r12, sp, lr, pc, cpsr, dfsr, ifsr,
far, fpexc, fpinst, fpinst2 from offset 40; the extra data names the process (`3dsx_app`). cpsr mode 0x13 with a
kernel address in pc = a kernel crash, not a fault in the game's code.

**Speed (Old 3DS, 268 MHz ARM11 + VFP11).**
- Double-precision division on the VFP11 is far slower than it looks: replacing the GTE's perspective division
  (`runtime/gte.c`) with a single-precision one plus one correction step on the remainder (exact over all 3.2 billion
  h / sz pairs) took 2-3 ms off every frame. Look for `double` math and 64-bit divisions in per-vertex / per-primitive
  code first.
- The game's own code plus the GTE (RTPS / RTPT and the model loops) is ~45 % of the main thread; the renderer ~25 %,
  spread over many small functions.
- Check every GTE change against the plain C path in qemu (`port2/tools/gte_arm_test.c`): `arm-none-eabi-gcc` with
  `--specs=rdimon.specs` works; do not install `gcc-arm-linux-gnueabi`, it removes `gcc-multilib` and breaks the i386
  build.

**Memory.** The code + data + bss image must end below 0x04000000 (`port2/n3ds/check_3dsx.py`); an Old 3DS has 64 MB
of application memory and gets no widescreen tables (lazy_bss: `port_lazy_alloc` gives them on a New 3DS only). The
CPU cannot write VRAM on a real console (emulators allow it): no CPU writes to VRAM frame buffers.

**Emulator runs.** `.github/workflows/3ds-old.yml` runs builds in Azahar as an Old 3DS; the run ids (and an optional
`cases:` line choosing the cases, e.g. `hut-wide`) are in `port2/n3ds/old3ds_runs.txt`. It checks start-up, memory
and that rooms draw; it says nothing about the console's speed or its kernel.

**PC memory checks of the runtime.** An AddressSanitizer build of the i386 port needs `--param asan-stack=0` (the
game's tasks are coroutines on their own stacks: ASan reports false stack overflows in `switch_to`) and
`ASAN_OPTIONS=handle_segv=0:allow_user_segv_handler=1`.
