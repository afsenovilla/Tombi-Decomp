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
- **Another agent often works at the same time** on the widescreen room strips (`port2/runtime/ws_backdrop*.c/.inc`,
  `ws_room3d.c`, `ws_edge.c`, docs/PORT.md widescreen sections). Pull before editing those files, keep your commits
  to explicit paths, and expect master to move under you; each of its pushes cancels the running APK CI run.

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
- In side view one world unit is about one screen column at Tomba's depth (measured), so the widescreen pad in
  columns is also the distance in units.
- Picking the moment of a room change in a frame series: the last frame before the fade (brightness drops).

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
