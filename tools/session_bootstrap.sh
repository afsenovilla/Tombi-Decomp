#!/usr/bin/env bash
# Prepares a fresh cloud session (or any Linux box) to build and test the native port, from a clean clone:
#   1. system packages and Python modules (the same set as .github/workflows/port2-exe.yml's Linux job, plus PySide6,
#      Pillow, numpy and xvfb for the launcher tests and renders)
#   2. the PS1 toolchain for the matching build (tools/setup_native.sh: old-gcc, maspsx, PSY-Q headers)
#   3b. the Japanese disc (SLPS-01144) from $GAME_FILES_URL_JP when set (game_jp/, for the JP decomp), then
#   3. the PAL disc (SCES-01331) from $GAME_FILES_URL (a Google Drive / Dropbox / direct link of the owner's own copy,
#      set as an environment variable of the cloud environment; never committed) -> game/disc/Tombi.bin, game/ files
#   4. data/ (tools/extract_data.py), the matching build (tools/build_full.py) and the i386 port (build/port2)
#   5. the wasm2c toolchain and build/wasm/tombi2w (the Android arm64 code built for the PC, ~5 min; --no-wasm skips it)
# Every step is skipped when its result is already there, so the script can be run again at any time.
#   tools/session_bootstrap.sh [--no-wasm] [--no-build]
set -euo pipefail
cd "$(dirname "$0")/.."
WASM=1; BUILD=1
for a in "$@"; do case "$a" in --wasm) WASM=1;; --no-wasm) WASM=0;; --no-build) BUILD=0;; *) echo "unknown option $a"; exit 2;; esac; done
log() { printf '\n== %s\n' "$*"; }
SUDO=""; [ "$(id -u)" = 0 ] || SUDO="sudo"
J="$(nproc)"

log "1/5 packages"
if ! dpkg -s libsdl2-2.0-0:i386 >/dev/null 2>&1 || ! dpkg -s libgl1:i386 >/dev/null 2>&1 || ! command -v cmake >/dev/null || ! command -v mipsel-linux-gnu-as >/dev/null; then
  $SUDO dpkg --add-architecture i386
  $SUDO apt-get update -qq
  # libgl1:i386 + the Mesa DRI drivers: the i386 port's OpenGL renderer under xvfb (--window --renderer gl, llvmpipe)
  $SUDO apt-get install -y -qq binutils-mipsel-linux-gnu libyaml-dev ninja-build cmake ccache gcc-multilib \
    libsdl2-dev libsdl2-2.0-0:i386 libgl1:i386 libgl1-mesa-dri:i386 libglx-mesa0:i386 patchelf xvfb unzip curl p7zip-full megatools >/dev/null
fi
if [ "$WASM" = 1 ] && ! command -v clang-18 >/dev/null; then
  $SUDO apt-get install -y -qq clang-18 lld-18 llvm-18 wasi-libc libclang-rt-18-dev-wasm32 >/dev/null
fi
python3 -c "import spimdisasm, rabbitizer, yaml, PIL, numpy" 2>/dev/null || {
  python3 -m pip install -q -r tools/requirements.txt Pillow numpy
  python3 -m pip install -q --no-deps splat64==0.35.2
}
# splat imports its optional pylibyaml accelerator unconditionally; a stub keeps the C build out (same as CI).
python3 -c "import pylibyaml" 2>/dev/null || python3 -c "import site,os; open(os.path.join(site.getsitepackages()[0],'pylibyaml.py'),'w').write('# optional accelerator stub\n')"
python3 -c "import PySide6" 2>/dev/null || python3 -m pip install -q PySide6-Essentials || echo "(PySide6 not installed: launcher tests skipped)"

log "2/5 PS1 toolchain"
[ -x /opt/oldgcc/gcc-2.7.2-psx/cc1 ] && [ -d include/tomba/psyq ] || bash tools/setup_native.sh

log "3/5 disc"
if [ ! -s game/disc/Tombi.bin ]; then
  if [ -z "${GAME_FILES_URL:-}" ]; then
    echo "GAME_FILES_URL is not set: add it as an environment variable of the cloud environment (the owner's link to"
    echo "their own SCES-01331 .bin), start a new session, and run this script again."; exit 1
  fi
  URL="$(printf '%s' "$GAME_FILES_URL" | tr -d '\r\n"'"'"' ')"
  URL="$(printf '%s' "$URL" | sed -E 's#(dropbox\.com/.*[?&])dl=0#\1dl=1#')"
  mkdir -p game/disc
  T="$(mktemp -d)"
  bash tools/ci_download.sh "$URL" "$T/game.dl" 100000000
  if unzip -tq "$T/game.dl" >/dev/null 2>&1; then           # a zip: take the largest .bin inside
    unzip -q "$T/game.dl" -d "$T/z"; mv "$(find "$T/z" -iname '*.bin' -printf '%s %p\n' | sort -n | tail -1 | cut -d' ' -f2-)" game/disc/Tombi.bin
  else mv "$T/game.dl" game/disc/Tombi.bin; fi
  rm -rf "$T"
fi
if [ ! -s game/MAIN0.EXE ]; then                            # the retail files the matching build and the generator read
  T="$(mktemp -d)"
  python3 -I tools/isoext.py game/disc/Tombi.bin "$T"
  mkdir -p game/disc/fs; cp -r "$T"/. game/disc/fs/
  cp "$(find "$T" -iname MAIN0.EXE | head -1)" game/MAIN0.EXE
  for a in 00 01 02 03 04 05 06 08 09 10 11 13 14 16 17 18 19; do
    f="$(find "$T" -ipath "*AREA$a/X0$a.BIN" | head -1)"; mkdir -p "game/AREA$a"; cp "$f" "game/AREA$a/X0$a.BIN"
  done
  for n in 1 2 3 4 5 6 7 8; do f="$(find "$T" -ipath "*EXE/MAIN$n.EXE" | head -1)"; [ -n "$f" ] && cp "$f" "game/MAIN$n.EXE"; done
  f="$(find "$T" -maxdepth 1 -iname 'SCES_013.31' | head -1)"; [ -n "$f" ] && cp "$f" game/SCES_013.31
  rm -rf "$T"
fi
echo "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc  game/MAIN0.EXE" | sha1sum -c --quiet - || { echo "game/MAIN0.EXE is not the PAL SCES-01331 one"; exit 1; }

log "3b/5 Japanese disc (optional: GAME_FILES_URL_JP)"
# The Japanese retail disc (SLPS-01144) for the JP decomp (docs/JP.md): same link rules as GAME_FILES_URL, a raw
# .bin (2352-byte sectors) or a zip of the extracted disc. Skipped when the variable is missing; never committed.
if [ ! -s game_jp/SLPS_011.44 ] && [ -n "${GAME_FILES_URL_JP:-}" ]; then
  URL="$(printf '%s' "$GAME_FILES_URL_JP" | tr -d '\r\n"'"'"' ')"
  URL="$(printf '%s' "$URL" | sed -E 's#(dropbox\.com/.*[?&])dl=0#\1dl=1#')"
  T="$(mktemp -d)"
  bash tools/ci_download.sh "$URL" "$T/jp.dl" 100000000
  bash tools/disc_unpack.sh "$T/jp.dl" "$T/jp"
  if [ -s "$T/jp.img" ]; then mkdir -p game_jp/disc; mv "$T/jp.img" game_jp/disc/tomba_jp.bin; fi
  mkdir -p game_jp/SYSTEM
  cp "$(find "$T/jp" -iname SLPS_011.44 | head -1)" game_jp/SLPS_011.44
  for f in INFO2 X00 X01 X02 X03 X04 X05 X06 X08 X09 X10 X11 X13 X14 X16 X17 X18 X19; do
    s="$(find "$T/jp" -ipath "*SYSTEM/$f.BIN" | head -1)"; [ -n "$s" ] && cp "$s" "game_jp/SYSTEM/$f.BIN"
  done
  rm -rf "$T"
  echo "07d577698ad31ec1a69b9d7b80a7c3f90b650888  game_jp/SLPS_011.44" | sha1sum -c --quiet - || { echo "game_jp/SLPS_011.44 is not the SLPS-01144 one"; exit 1; }
fi
# Demo / prototype discs: never fetched here (that is 2 GB on every new session). When a task needs one, run
# `bash tools/disc_fetch.sh DENGEKI_D5` (or PREPRE9, PREPRE8, JP_DEMO, BETA980203, USA): it extracts the disc of
# GAME_FILES_URL_<NAME> to /dev/shm/<name>/fs once per session. This only says which are available.
AVAIL=""; for NAME in DENGEKI_D5 PREPRE9 PREPRE8 HYPER12 JP_DEMO BETA980203 USA; do V="GAME_FILES_URL_$NAME"; [ -n "${!V:-}" ] && AVAIL="$AVAIL $NAME"; done
[ -z "$AVAIL" ] || log "3c/5 demo discs available on demand (tools/disc_fetch.sh):$AVAIL"

if [ -s game_jp/SLPS_011.44 ] && [ ! -x /root/.cargo/bin/objdiff-cli ]; then   # the report tool of the JP decomp
  mkdir -p /root/.cargo/bin
  curl -fsSL -o /root/.cargo/bin/objdiff-cli https://github.com/encounter/objdiff/releases/download/v3.8.2/objdiff-cli-linux-x86_64 && chmod +x /root/.cargo/bin/objdiff-cli
fi

log "4/5 data/"
[ -s data/manifest.txt ] || python3 -I tools/extract_data.py game/disc/Tombi.bin data --quiet

[ "$BUILD" = 1 ] || { log "done (--no-build)"; exit 0; }
log "5/5 builds"
# a failed earlier run leaves build/full half made: test the linker scripts build_full.py writes, not the directory
if ! ls build/full/*.full.ld >/dev/null 2>&1 || [ -n "$(find src include -newer build/full -name '*.[ch]' -print -quit 2>/dev/null)" ]; then
  python3 tools/build_full.py -j "$J"
fi
[ -f build/port2/Makefile ] || cmake -S port2 -B build/port2 >/dev/null
PORT_STALE_FULL=warn make -C build/port2 -j"$J" 2>&1 | grep -E "error|Error" || true
[ -x build/port2/tombi2 ] && echo "build/port2/tombi2 ready" || { echo "the port build failed"; exit 1; }
if [ "$WASM" = 1 ]; then
  PORT_STALE_FULL=warn python3 port2/wasm/build.py -j "$J" >/dev/null && echo "build/wasm/tombi2w ready"
fi
if [ -s game_jp/SLPS_011.44 ] && [ ! -s build/slps01144/report.json ]; then
  log "JP report (build/slps01144/report.json, ~5 min)"
  [ -s config/symbol_addrs_x000.txt ] || python3 tools/gen_symbols.py >/dev/null
  python3 tools/build_report.py --version slps01144 --objdiff /root/.cargo/bin/objdiff-cli >/dev/null 2>&1 && python3 tools/progress_jp.py | tail -1 || echo "the JP report failed: python3 tools/build_report.py --version slps01144 --objdiff /root/.cargo/bin/objdiff-cli"
fi
log "smoke test"
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy timeout 120 build/port2/tombi2 --data data --headless --frames 300 >/dev/null 2>&1 \
  && echo "the port runs (300 headless frames)" || echo "smoke test failed: run build/port2/tombi2 --data data --headless --frames 300"
