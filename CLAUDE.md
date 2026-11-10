# Working on Tombi-Decomp with Claude Code

Read this first in every session. The current state and the open tasks are in `docs/STATUS.md`; keep that file up
to date as work lands (it is the hand-off between sessions).

**First thing in a new cloud session:** run `bash tools/session_bootstrap.sh`, without being asked. It installs the
packages and the PS1 toolchain, downloads the PAL disc from the `GAME_FILES_URL` environment variable, extracts `game/`
and `data/`, builds the matching build, `build/port2/tombi2` and the Android arm64 code for the PC
(`build/wasm/tombi2w`), and runs a smoke test (about 10 minutes plus the download). It skips whatever is already there. If `GAME_FILES_URL` is
missing, tell the user to add it in the cloud environment's settings and start a new session.

## The project

Tombi! (PAL, SCES-01331) decompilation plus everything built on it:

- the native port (`port2/`: i386 PC build, plus an arm64 build made through wasm2c in `port2/wasm/`, docs/PORT.md
  "64-bit builds: wasm2c");
- the PC launcher (`tools/port_launcher.py`, Qt / PySide6) and the Android app (`port2/android/`, one APK with
  arm64-v8a + armeabi-v7a; Canvas-drawn launcher `LauncherActivity`, `SelvaDialog`, `TombiActivity`);
- the second-screen panel for dual-screen handhelds (AYN Thor) (`port2/runtime/panel*.c/.inc`, docs/PORT.md
  "Second screen");
- the improved Spanish translation (`patches/es_mejorada.tombipatch.json` + compiled `.tpatch`);
- tools: translation editor (`tools/text_editor.py`), save editor (`tools/tombi_save_editor.py`), save import
  (`tools/save_import.py`), gfx tools.

Main references: `README.md`, `docs/PORT.md` (long; search it), `docs/STATUS.md`.

## How to talk to the user

- Reply in **Spanish**, short and direct, first person. Never claim something works without verifying it; say
  plainly what was tested (PC headless renders) and what was not (the device).
- Show real renders/screenshots of UI changes (send the PNGs), not descriptions.
- When the user proposes UI, they like 3-4 real-render options to choose from.
- The user tests on an AYN Thor (Android 13, Snapdragon 8 Gen 2 → arm64 = wasm2c build, top 1920x1080 60 Hz,
  bottom 1240x1080) and an Odin 3. Bugs that only show on the device usually come from the wasm2c build or Android
  glue: reproduce with `build/wasm/tombi2w` (PC build of the same 64-bit code: `PORT_STALE_FULL=warn python3
  port2/wasm/build.py -j 8`, ~5 min). Ask for the app log (Opciones → Ver último log → Compartir) when needed.
- Discord texts: English, plain paragraphs, under 2000 characters, never credit Folkin.
- **Everything generated is in English**: reports, agents' reports, index/summary files, notes, docs, and also the
  labels, titles and captions inside generated images (sprite sheets, comparison GIFs, charts). Only the chat replies
  are in Spanish.
- Changelog and release notes: English.

## Hard rules

- **Never commit** game/SDK binaries, disc images, extracted assets, game graphics, text dumps, screenshots or
  saves. "Ship scripts, not pixels": glyphs/graphics are rebuilt at run time from the user's disc; tables hold
  coordinates only. Game URLs live only in secrets (GitHub Actions `GAME_FILES_URL*`; e.g. `GAME_FILES_URL_SCES01330`
  for the FR/DE/IT/NL disc).
- `tombi_mc*.mcd` in the repo root are the user's: read-only, work on copies.
- Never modify `.claude/settings.json`.
- Git: commit explicit paths only (no `git add -A`), no amend, no force push, `git pull --no-rebase origin master`
  before pushing to `master`. Never `pkill -f`. Author stays `noreply@anthropic.com`. Every commit ends with the
  trailers the session gives (Co-Authored-By / Claude-Session). No model names anywhere in the repo.
- The translation JSON is edited textually (never `json.dump` the whole file); `itemNames`/`eventNames` keys are the
  original disc strings and are never edited. Validate with
  `python3 tools/check_text_patch.py game/disc/Tombi.bin patches/es_mejorada.tombipatch.json` and rebuild with
  `python3 tools/tpatch_build.py data/ patches/es_mejorada.tombipatch.json -o patches/es_mejorada.tpatch`.
- In Spanish, the panel and every dialog always use the improved translation's names (accents, ñ); never cut text
  with "..." (wrap instead); one thing per line in lists (title / place plate / place).
- Long lists on the touch panel scroll by finger, smoothly (no paging).
- The user does not review PRs (they approve them all by default): verified work goes straight to master, and
  Claude is responsible for judging that the code is correct before pushing. Open a PR only when asked.
- Releases only when the user says so: `release.yml` (workflow_dispatch, inputs `version`, `include_tools: true`)
  after the Windows exe and Android APK CI runs of the head commit are green. Release title "Tombi port X".
- Disk in the cloud container is small: delete builds/dumps when done; agents in worktrees must clean up.

## CI and the game discs

- Disc secrets, one per disc, the same names as environment variables of the cloud container: `GAME_FILES_URL` (PAL
  SCES-01331), `GAME_FILES_URL_SCES01330` (FR/DE/IT/NL), `GAME_FILES_URL_USA` (SCUS-94236), `GAME_FILES_URL_JP`
  (SLPS-01144), `GAME_FILES_URL_JP_DEMO` (SLPM-80165), `GAME_FILES_URL_BETA980203`, `GAME_FILES_URL_PREPRE8`
  (PCPX-96084), `GAME_FILES_URL_PREPRE9` (PCPX-96093), `GAME_FILES_URL_DENGEKI_D5` (SLPM-80186), `GAME_FILES_URL_HYPER12`
  (SLPM-80167, Hyper PlayStation Re-mix No.12). The owner keeps the
  zips in a Cloudflare R2 bucket (public dev URL, no egress quota); a direct URL, Dropbox, Drive or public MEGA link
  works too (all through `tools/ci_download.sh`). The zip may hold the image inside a folder or the extracted files:
  `tools/disc_unpack.sh` handles every shape (PrePre 8 through `tools/prepre8.py extract`). On Actions the downloads
  stay in the cache (`.github/actions/disc-cache`), so a disc is fetched once per workflow and disc.
- `jp-sweep.yml` (manual, or a push to master touching JP C/config): runs `jp_autofix --install` and `demo_sweep` over
  the JP builds and commits what matched straight to master (a push with GITHUB_TOKEN starts no other workflow, so the
  progress report runs on the next stamp push). `report.yml`
  rebuilds the reports when `notes/report_stamp.txt` changes (`date -u > notes/report_stamp.txt`).
- Cloud bootstrap also fetches the JP disc when `GAME_FILES_URL_JP` is set (step 3b) and builds
  `build/slps01144/report.json`.
- The demo discs are never fetched by the bootstrap: `bash tools/disc_fetch.sh DENGEKI_D5` (PREPRE9, PREPRE8, HYPER12,
  JP_DEMO, BETA980203, USA, SCES01330) extracts the one a task needs from its `GAME_FILES_URL_<NAME>` variable to
  `game_<name>/fs` (+ `disc.bin`, git-ignored; not /dev/shm, which the idle restarts of the container wipe), once
  per container; `tools/event_icons.py` reads those paths.

## Build / test cheatsheet

- i386 port: `PORT_STALE_FULL=warn make -C build/port2 -j8` → `build/port2/tombi2`.
- Headless run: `SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy build/port2/tombi2 --data data --headless ...`
  `--dump-frames DIR --dump-every N`, `--dump-vram`, `--pad-script FRAME:BUTTONS[:LEN]...`, `--poke FRAME:ADDR:VAL[:SIZE]`.
- Debug area select: `--skip-movies --poke 900:1F8001B4:1 --poke 1250:8009C960:AREA:2 --poke 1250:8009C962:SECTION:2
  --pad-script 1000:start,1300:circle:3` (the Start at 1000 is needed). `--poke` values are **hex**: AREA10 is `A`, AREA17
  is `11`. The 13 `DOOR_ROOMS` of tools/psxemu/rooms.py (e.g. 17:0) crash from the select in the real game too.
- Panel: `--second-screen --dump-panel DIR --panel-size 1240x1080 --panel-input FRAME:ACTION,...` (actions: up down
  left right ok back tabl tabr zoomout zoomin tap@X@Y hold@X@Y drag@X0@Y0@X1@Y1 pinch@P@X@Y). Spanish:
  `--lang 5 --text-patch patches/es_mejorada.tombipatch.json`.
- GL renderer can be tested with `xvfb-run -a ... --window --renderer gl`.
- Python tests: `python3 tools/test_save_import.py`, `QT_QPA_PLATFORM=offscreen python3 tools/test_port_launcher.py`.
- CI status without gh: `curl -s "https://api.github.com/repos/afsenovilla/Tombi-Decomp/actions/runs?head_sha=SHA"`.
