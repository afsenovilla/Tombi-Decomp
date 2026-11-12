#!/usr/bin/env python3
"""The game library of another version for the APK (docs/PORT_VERSIONS_PLAN.md stages U2 / J1): libtombi_<ver>.so for each
ABI, next to the PAL libtombi.so that Gradle's CMake build makes.  The launcher's library (Library.java) names it to the
loader (loader.c --game-lib); one game library per version and ABI.

  python3 port2/android/game_libs.py --version usa --sdl2 SDL2_SRC --ndk NDK [-j N] [--abi armeabi-v7a] [--abi arm64-v8a]
      -> port2/android/app/src/main/jniLibs/<abi>/libtombi_usa.so (git-ignored; Gradle packs it with the CMake outputs)

Needs what the PAL build needs plus the version's matching build and disc files: python3 tools/build_full.py --version usa
(build/scus94236/full) and the USA disc's files in game_usa/fs (bash tools/disc_fetch.sh USA).
  armeabi-v7a: port2/CMakeLists.txt with -DPORT_VERSION=usa (the native 32-bit build), target tombi2;
  arm64-v8a:   port2/wasm/build.py --version usa --until w2c (build/wasm-<ver>/w2c), then port2/CMakeLists.txt's arm64
               branch around that wasm2c output, target tombi2.
Each in build/android-<ver>/<abi> (CMake + Ninja, the NDK's toolchain file, android-23 as the app's minSdk)."""
import argparse, os, shutil, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__)); P2 = os.path.dirname(HERE); ROOT = os.path.dirname(P2)
ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
ap.add_argument("--version", required=True, help="usa or jp (port/tools/portver.py)")
ap.add_argument("--sdl2", required=True, help="SDL2 2.30.x source tree (the same as Gradle's -Psdl2Source)")
ap.add_argument("--ndk", default=os.environ.get("ANDROID_NDK_LATEST_HOME") or os.environ.get("ANDROID_NDK_HOME"))
ap.add_argument("--abi", action="append", choices=("armeabi-v7a", "arm64-v8a"))
ap.add_argument("--out", default=os.path.join(HERE, "app", "src", "main", "jniLibs"))
ap.add_argument("-j", type=int, default=os.cpu_count())
a = ap.parse_args()
if not a.ndk or not os.path.isfile(os.path.join(a.ndk, "build", "cmake", "android.toolchain.cmake")):
    sys.exit("game_libs: --ndk must name an Android NDK (build/cmake/android.toolchain.cmake)")
ver = a.version.lower()
NAME = "libtombi_%s.so" % ver


def sh(cmd, **kw):
    print("+", " ".join(cmd), flush=True)
    subprocess.run(cmd, check=True, **kw)


for abi in a.abi or ["armeabi-v7a", "arm64-v8a"]:
    bdir = os.path.join(ROOT, "build", "android-%s" % ver, abi)
    args = ["cmake", "-S", P2, "-B", bdir, "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release",
            "-DCMAKE_TOOLCHAIN_FILE=" + os.path.join(a.ndk, "build", "cmake", "android.toolchain.cmake"),
            "-DANDROID_ABI=" + abi, "-DANDROID_PLATFORM=android-23", "-DANDROID_STL=c++_static",
            "-DPORT2_SDL2_SOURCE=" + os.path.abspath(a.sdl2), "-DPython3_EXECUTABLE=" + sys.executable]
    if abi == "arm64-v8a":
        w2c_out = os.path.join(ROOT, "build", "wasm-%s" % ver)
        sh([sys.executable, os.path.join(P2, "wasm", "build.py"), "--version", ver, "--out", w2c_out, "--until", "w2c", "-j", str(a.j)])
        args.append("-DPORT2_W2C_DIR=" + os.path.join(w2c_out, "w2c"))
    else:
        args.append("-DPORT_VERSION=" + ver)
    sh(args)
    sh(["cmake", "--build", bdir, "--target", "tombi2", "-j", str(a.j)])
    so = os.path.join(bdir, "libtombi.so")
    if not os.path.isfile(so): sys.exit("game_libs: %s was not built" % so)
    os.makedirs(os.path.join(a.out, abi), exist_ok=True)
    dst = os.path.join(a.out, abi, NAME)
    shutil.copyfile(so, dst)
    dbg = os.path.join(bdir, "libtombi.debug.so")     # (the DWARF copy, for symbolising a device crash)
    if os.path.isfile(dbg): shutil.copyfile(dbg, os.path.join(bdir, NAME.replace(".so", ".debug.so")))
    print("game_libs: %s (%d bytes)" % (dst, os.path.getsize(dst)), flush=True)
