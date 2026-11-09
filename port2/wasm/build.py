#!/usr/bin/env python3
"""port2 as wasm32 -> wasm2c -> native C (spike: 32-bit guest address space on a 64-bit host).  docs/PORT.md "wasm32 spike".

  python3 port2/wasm/build.py [-j N] [--out build/wasm] [--cc gcc]   ->  build/wasm/tombi2w (x86-64 Linux, SDL2 window or headless)
  python3 port2/wasm/build.py --until w2c   ->  build/wasm/w2c: the portable C (module + wasm2c runtime) that the
                                              arm64-v8a Android build compiles with the host files (port2/CMakeLists.txt)

Needs: clang/wasm-ld/llvm-link/llvm-nm 18 (Ubuntu: clang lld llvm), wasi-libc (/usr/lib/wasm32-wasi), libclang-rt-18-dev-wasm32,
python3, a native cc.  WABT (wasm2c) is cloned at a pinned tag and built into OUT/wabt-build when not there.
Game data: gen2.py link reads the user's game/MAIN0.EXE etc. as the PC build does; everything stays in OUT."""
import argparse, os, re, shutil, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
HERE = os.path.dirname(os.path.abspath(__file__)); P2 = os.path.dirname(HERE); ROOT = os.path.dirname(P2)
WABT_TAG = "1.0.36"
PROGS = ("MAIN0 X000 X001 X002 X003 X004 X005 X006 X008 X009 X010 X011 X013 X014 X016 X017 X018 X019 "
         "O0PTSUB0 D0SPSUB D0SPSUB2 D0SPSUB3 G0OVER I0NFO I0NFO2 I0NFO3").split()
# host-only files: signals, the watchdog thread (stubbed in rt_wasm.c), SDL (runtime/sdl.c is compiled for the host:
# host_sdl.c bridges it).  shims/gpu_gl.c is in the module, its GL calls are imports (gl_wasm.h, host_gl.c).
RT_EXCLUDE = {"lowmem.c", "watchdog.c", "host_win32.c", "sdl.c"}
ap = argparse.ArgumentParser()
ap.add_argument("--out", default=os.path.join(ROOT, "build", "wasm"))
ap.add_argument("-j", type=int, default=os.cpu_count())
ap.add_argument("--cc", default=os.environ.get("CC", "cc"))
ap.add_argument("--native-flags", default="-O2")
ap.add_argument("--from", dest="start", default="sources", help="first step: sources|compile|link|ir|w2c|native")
ap.add_argument("--until", dest="stop", default="native", help="last step")
ap.add_argument("--headless-host", action="store_true", help="native step without SDL2/GL (headless_stubs.py), e.g. "
                "--cc aarch64-linux-gnu-gcc --native-flags '-O2 -static' to run the arm64 host code under qemu-aarch64")
ap.add_argument("--exe", default="tombi2w", help="native output name (in --out)")
ap.add_argument("--version", default=os.environ.get("PORT_VERSION", ""), help="game version (port/tools/portver.py): empty "
                "= PAL SCES-01331, usa = SCUS-94236 (needs tools/build_full.py --version usa); use its own --out")
a = ap.parse_args()
OUT = os.path.abspath(a.out); GEN = os.path.join(OUT, "gen"); OBJ = os.path.join(OUT, "obj")
STEPS = ["sources", "compile", "link", "ir", "w2c", "native"]
def want(step): return STEPS.index(a.start) <= STEPS.index(step) <= STEPS.index(a.stop)
def sh(cmd, **kw):
    print("+", " ".join(cmd) if isinstance(cmd, list) and len(" ".join(cmd)) < 300 else str(cmd)[:300], flush=True)
    subprocess.run(cmd, check=True, **kw)
ENV = dict(os.environ, PORT2_ARCH="wasm", PORT_VERSION=a.version)
WCC = ["clang", "--target=wasm32-wasi", "-O2", "-w", "-funsigned-char", "-fwrapv", "-fno-strict-aliasing",
       "-fno-delete-null-pointer-checks", "-emit-llvm", "-Wno-error=implicit-function-declaration", "-Wno-error=int-conversion",
       "-Wno-error=incompatible-function-pointer-types", "-Wno-error=implicit-int", "-Wno-error=return-type"]
GAME = WCC + ["-ftrivial-auto-var-init=zero", "-fno-builtin", "-DSKIP_ASM", "-D_LANGUAGE_C", "-DMIPSEL", "-DPORT2_WASM", "-I" + os.path.join(P2, "include"), "-I" + GEN]
CHDR = os.path.join(P2, "third_party", "libchdr")        # .chd disc images, as in the CMake builds
RT = WCC + ["-include", os.path.join(HERE, "wasm_compat.h"), "-I" + os.path.join(P2, "include"), "-I" + GEN,
            "-I" + os.path.join(GEN, "version"),       # port_version.h (gen2.py: the version's layout, as the CMake build)
            "-I" + os.path.join(CHDR, "include"), "-DPORT_HAVE_CHD"]

def compile_all(jobs):
    def run(j):
        src, obj, flags = j
        if os.path.exists(obj) and os.path.getmtime(obj) > os.path.getmtime(src): return None
        os.makedirs(os.path.dirname(obj), exist_ok=True)
        r = subprocess.run(flags + ["-c", src, "-o", obj], capture_output=True, text=True)
        return (src + ":\n" + r.stderr[-3000:]) if r.returncode else None
    errs = [e for e in ThreadPoolExecutor(a.j).map(run, jobs) if e]
    for e in errs[:20]: sys.stderr.write(e)
    if errs: sys.exit("%d compile failures" % len(errs))

# 1. filtered game sources (gen2.py, as the CMake build, with the clang K&R fix-ups)
if want("sources"):
    sh([sys.executable, os.path.join(P2, "tools", "gen2.py"), "sources", GEN] + PROGS, env=ENV)
    sh(["cmake", "-DSRC=" + ROOT, "-DOUT=" + os.path.join(GEN, "build_id.c"), "-P", os.path.join(ROOT, "port", "cmake", "build_id.cmake")])
# 2. game + runtime -> LLVM bitcode
if want("compile"):
    txt = open(os.path.join(GEN, "sources.cmake")).read()
    sets = {m.group(1): m.group(2).split() for m in re.finditer(r"set\((\w+)\s+([^)]*)\)", txt)}
    jobs = []
    # (another version: its VERSION_<SERIAL> macro picks the version's #if branches, and the psx_tomba header set inc26)
    vdefs = ["-D" + d for d in sets.get("PORT_VERSION_DEFS", []) if d != "None"]   # (PAL: "None")
    for key, inc in (("PORT_GAME_SOURCES", "inc"), ("PORT_GAME_SOURCES_T24", "inc24"), ("PORT_GAME_SOURCES_T26", "inc26")):
        for s in sets.get(key, []):
            jobs.append((s, os.path.join(OBJ, inc, os.path.relpath(s, GEN)) + ".bc",
                         GAME + vdefs + ["-I" + os.path.join(GEN, inc), "-I" + os.path.join(GEN, inc, "tomba")]))
    rt = [os.path.join(P2, d, f) for d in ("runtime", "shims") for f in sorted(os.listdir(os.path.join(P2, d)))
          if f.endswith(".c") and f not in RT_EXCLUDE]
    rt += [os.path.join(HERE, "rt_wasm.c"), os.path.join(GEN, "funcmap.c"), os.path.join(GEN, "build_id.c")]
    for s in rt: jobs.append((s, os.path.join(OBJ, "rt", os.path.basename(s)) + ".bc", RT))
    for s in (os.path.join(CHDR, "unity.c"), os.path.join(CHDR, "src", "libchdr_codec_avhuff.c")):
        jobs.append((s, os.path.join(OBJ, "chd", os.path.basename(s)) + ".bc", WCC + ["-DZSTD_DISABLE_ASM", "-I" + os.path.join(CHDR, "include")]))
    compile_all(jobs)
    with open(os.path.join(OBJ, "objects.txt"), "w") as f:
        for j in jobs: f.write(j[1] + "\n")
# 3. gen2.py link: data image, aliases (syms.ld), C dispatch thunks, stubs, address tables
if want("link"):
    sh([sys.executable, os.path.join(P2, "tools", "gen2.py"), "link", GEN, "llvm-nm", os.path.join(OBJ, "objects.txt")], env=ENV)
    gj = [(os.path.join(GEN, f), os.path.join(OBJ, "gen", f) + ".bc", RT + ["-O0", "-fno-zero-initialized-in-bss"]) for f in ("data.c", "stubs.c", "thunks.c", "addrmap.c")]
    for j in gj:
        if os.path.exists(j[1]): os.remove(j[1])
    compile_all(gj)
# 4. one LLVM module: llvm-link everything, then the aliases of syms.ld (wasm-ld has no linker scripts): function and
#    whole-symbol aliases become renames of the declaration, region offsets become GlobalAliases into port_ram;
#    then wasm object -> wasm-ld with wasi-libc
if want("ir"):
    objs = [l.strip() for l in open(os.path.join(OBJ, "objects.txt")) if l.strip()]
    objs += [os.path.join(OBJ, "gen", f) + ".bc" for f in ("data.c", "stubs.c", "thunks.c", "addrmap.c")]
    lst = os.path.join(OUT, "bc.rsp"); open(lst, "w").write("\n".join(objs) + "\n")
    sh(["llvm-link", "-o", os.path.join(OUT, "merged.bc"), "@" + lst])
    sh(["llvm-dis", os.path.join(OUT, "merged.bc"), "-o", os.path.join(OUT, "merged.ll")])
    sh([sys.executable, os.path.join(HERE, "apply_aliases.py"), os.path.join(OUT, "merged.ll"), os.path.join(GEN, "syms.ld"),
        os.path.join(OUT, "game.ll")])
    sh(["clang", "--target=wasm32-wasi", "-O2", "-c", os.path.join(OUT, "game.ll"), "-o", os.path.join(OUT, "game.o")])
    rtlib = "/usr/lib/llvm-18/lib/clang/18/lib/wasi/libclang_rt.builtins-wasm32.a"
    sh(["wasm-ld", "-o", os.path.join(OUT, "tombi2.wasm"), "/usr/lib/wasm32-wasi/crt1-command.o", os.path.join(OUT, "game.o"),
        "-L/usr/lib/wasm32-wasi", "-lc", "-lwasi-emulated-signal", rtlib,
        "--global-base=65536",          # nothing below 64 KB: the null-page window (patch_w2c.py redirects it into port_ram)
        "-z", "stack-size=8388608", "--initial-memory=134217728", "--max-memory=1073741824",   # (grows: --internal-res buffers)
        "--export=port_thread_entry_wasm", "--export=port_ram", "--export=__stack_pointer"])
# 5. wasm2c (WABT, pinned tag) and the patches of its output
W2C = os.path.join(OUT, "wabt-build", "wasm2c")
if want("w2c"):
    if not os.path.exists(W2C):
        src = os.path.join(OUT, "wabt-src")
        if not os.path.exists(src):
            sh(["git", "clone", "-q", "--depth", "1", "--branch", WABT_TAG, "--recurse-submodules", "--shallow-submodules",
                "https://github.com/WebAssembly/wabt.git", src])
        sh(["cmake", "-S", src, "-B", os.path.join(OUT, "wabt-build"), "-G", "Ninja", "-DBUILD_TESTS=OFF", "-DBUILD_LIBWASM=OFF",
            "-DCMAKE_BUILD_TYPE=Release"])
        sh(["ninja", "-C", os.path.join(OUT, "wabt-build"), "wasm2c"])
    w2 = os.path.join(OUT, "w2c"); os.makedirs(w2, exist_ok=True)
    for f in os.listdir(w2): os.remove(os.path.join(w2, f))
    sh([W2C, os.path.join(OUT, "tombi2.wasm"), "-n", "tombi", "--num-outputs", "8", "-o", os.path.join(w2, "tombi.c")])
    sh([sys.executable, os.path.join(HERE, "patch_w2c.py"), w2])
    wrt = os.path.join(OUT, "wabt-src", "wasm2c")      # the wasm2c runtime next to the module: one self-contained dir
    for f in ("wasm-rt.h", "wasm-rt-impl.h", "wasm-rt-impl.c", "wasm-rt-mem-impl.c", "wasm-rt-impl-tableops.inc",
              "wasm-rt-mem-impl-helper.inc", "wasm-rt-exceptions.h"):
        shutil.copy(os.path.join(wrt, f), os.path.join(w2, f))
# 6. native: the translated module + wasm2c runtime + host (WASI subset, coroutines, imports, runtime/sdl.c with SDL2)
HOST_SRCS = [os.path.join(HERE, f) for f in ("host.c", "host_sdl.c", "host_gl.c")]
W2C_FLAGS = ["-fno-strict-aliasing", "-fwrapv", "-ffp-contract=off", "-w", "-DWASM_RT_NONCONFORMING_UNCHECKED_STACK_EXHAUSTION=1",
             "-DWASM_RT_SKIP_SIGNAL_RECOVERY=1", "-DWASM_RT_TRAP_HANDLER=port_w2c_trap"]
if want("native"):
    w2 = os.path.join(OUT, "w2c")
    srcs = sorted(os.path.join(w2, f) for f in os.listdir(w2) if f.endswith(".c"))
    nobj = os.path.join(OUT, "nobj" if a.exe == "tombi2w" else "nobj-" + a.exe); os.makedirs(nobj, exist_ok=True)
    fl = [a.cc] + a.native_flags.split() + ["-I" + w2, "-I" + HERE] + W2C_FLAGS
    if a.headless_host:
        stubs = os.path.join(nobj, "headless_stubs.c")
        sh([sys.executable, os.path.join(HERE, "headless_stubs.py"), w2, stubs])
        srcs += [os.path.join(HERE, "host.c"), os.path.join(HERE, "host_sdl.c"), stubs]
        fl += ["-DPORT2_WASM_HEADLESS"]
        jobs = [(s_, fl) for s_ in srcs]
    else:
        srcs += HOST_SRCS
        sdl = os.path.join(P2, "runtime", "sdl.c")
        jobs = [(s_, fl) for s_ in srcs] + [(sdl, [a.cc] + a.native_flags.split() + ["-include", os.path.join(HERE, "sdl_bridge.h"),
                                                   "-I" + HERE, "-I" + os.path.join(P2, "include"), "-w"])]
    deps = [os.path.abspath(__file__)] + [os.path.join(HERE, f) for f in os.listdir(HERE) if f.endswith(".h")]
    def run(j):
        s_, f_ = j
        o = os.path.join(nobj, os.path.basename(s_) + ".o")
        if os.path.exists(o) and os.path.getmtime(o) > max([os.path.getmtime(s_)] + [os.path.getmtime(x) for x in deps]): return o, None
        r = subprocess.run(f_ + ["-c", s_, "-o", o], capture_output=True, text=True)
        return o, (s_ + ":\n" + r.stderr[-3000:]) if r.returncode else None
    res = list(ThreadPoolExecutor(a.j).map(run, jobs))
    errs = [e for o, e in res if e]
    for e in errs: sys.stderr.write(e)
    if errs: sys.exit("native compile failed")
    sh([a.cc] + a.native_flags.split() + ["-o", os.path.join(OUT, a.exe)] + [o for o, e in res] +
       ([] if a.headless_host else ["-lSDL2"]) + ["-lm", "-lpthread"])
    print("built", os.path.join(OUT, a.exe))
