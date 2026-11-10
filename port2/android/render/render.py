#!/usr/bin/env python3
"""Real renders of the Android launcher on a PC, without the Android SDK: the app's own LauncherActivity / Library code
drawn by Robolectric's native graphics (Android's Skia, the same Canvas the device uses) into PNGs at 1920x1080.

  python3 port2/android/render/render.py OUT_DIR [--pal FS] [--usa FS] [--uilang N] [--discs DIR] [--test NAME]

  FS: a folder with the SYSTEM/ files of the player's own disc (game_usa/fs, an extracted PAL disc ...): the cards'
  logos are cut from it as on the device; without it the cards show the names.  --uilang: LauncherActivity.UI_LANGS
  index (default 2 = Español, 1 = English).  --discs: a folder of files to add as the player would pick them (disc
  images, .zip files ...: RenderTest.addFlow, in name order), with the real disc check: port2/android/disccheck.c built
  for the PC (gcc + the JDK's jni.h, no .chd) and loaded by the launcher as on the device.  --test: one test method
  (render, migrateAndKeys, addFlow).  The PNGs and what RenderTest prints (migration, key / touch navigation,
  the arguments per version) are the result.

Needs java + javac (17+), mvn and network access to Maven Central (its Google mirror) and Google's Maven: aapt2
(com.android.tools.build:aapt2) compiles the app's res/, Robolectric 4.14 with android-all 14 / 15 (AOSP, Maven) runs
it, the launcher fonts (OFL) come from google/fonts at the CI's commit.  Everything goes to build/android-render
(about 700 MB; delete it when done).  The game's TombiActivity (an SDLActivity) is replaced by a stub."""
import argparse, glob, os, re, shutil, subprocess, sys, urllib.request, zipfile

HERE = os.path.dirname(os.path.abspath(__file__)); APP = os.path.join(os.path.dirname(HERE), "app", "src", "main")
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
ap.add_argument("out")
ap.add_argument("--pal"); ap.add_argument("--usa")
ap.add_argument("--uilang", default="2")
ap.add_argument("--discs"); ap.add_argument("--test")
ap.add_argument("--work", default=os.path.join(ROOT, "build", "android-render"))
a = ap.parse_args()
W = os.path.abspath(a.work); os.makedirs(W, exist_ok=True)
CENTRAL = "https://maven-central.storage-download.googleapis.com/maven2"   # (repo1 rate-limits large downloads)
GOOGLE = "https://dl.google.com/android/maven2"
AAPT2 = "com/android/tools/build/aapt2/8.5.2-11315950/aapt2-8.5.2-11315950-linux.jar"
ALL_PLAIN = "org/robolectric/android-all/14-robolectric-10818077/android-all-14-robolectric-10818077.jar"
ALL_INSTR = ["org/robolectric/android-all-instrumented/14-robolectric-10818077-i7/android-all-instrumented-14-robolectric-10818077-i7.jar",
             "org/robolectric/android-all-instrumented/15-robolectric-12650502-i7/android-all-instrumented-15-robolectric-12650502-i7.jar"]
GF = "https://raw.githubusercontent.com/google/fonts/6c1dbe27dcf7d817d4b62cec033ba77fa8e2a5ec/ofl/"
FONTS = {"LilitaOne-Regular.ttf": GF + "lilitaone/LilitaOne-Regular.ttf", "Nunito.ttf": GF + "nunito/Nunito%5Bwght%5D.ttf"}


def fetch(url, dst):
    if os.path.isfile(dst) and os.path.getsize(dst) > 0: return dst
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    print("download", url, flush=True)
    with urllib.request.urlopen(url) as r, open(dst + ".part", "wb") as o: shutil.copyfileobj(r, o, 1 << 20)
    os.replace(dst + ".part", dst)
    return dst


def sh(cmd, **kw):
    print("+", " ".join(cmd)[:300], flush=True)
    return subprocess.run(cmd, check=True, **kw)


# 1. tools and the Android framework
aapt2 = os.path.join(W, "aapt2")
if not os.path.isfile(aapt2):
    with zipfile.ZipFile(fetch(GOOGLE + "/" + AAPT2, os.path.join(W, "aapt2.jar"))) as z: open(aapt2, "wb").write(z.read("aapt2"))
    os.chmod(aapt2, 0o755)
plain = fetch(CENTRAL + "/" + ALL_PLAIN, os.path.join(W, os.path.basename(ALL_PLAIN)))
deps = os.path.join(W, "deps")
for p in ALL_INSTR: fetch(CENTRAL + "/" + p, os.path.join(deps, os.path.basename(p)))
for n, u in FONTS.items(): fetch(u, os.path.join(W, "assets", "fonts", n))

# 2. Robolectric + JUnit (Maven, a local repository under the work dir; the proxy of $HTTPS_PROXY when there is one)
cpf = os.path.join(W, "cp.txt")
if not os.path.isfile(cpf):
    proxy = ""
    m = re.match(r"https?://([^:/]+):(\d+)", os.environ.get("HTTPS_PROXY", "") or os.environ.get("https_proxy", ""))
    if m: proxy = "<proxies><proxy><id>p</id><active>true</active><protocol>https</protocol><host>%s</host><port>%s</port></proxy></proxies>" % m.groups()
    open(os.path.join(W, "settings.xml"), "w").write(
        "<settings><localRepository>%s</localRepository>%s<mirrors><mirror><id>c</id><mirrorOf>central</mirrorOf><url>%s</url>"
        "</mirror></mirrors></settings>\n" % (os.path.join(W, "m2"), proxy, CENTRAL))
    shutil.copyfile(os.path.join(HERE, "pom.xml"), os.path.join(W, "pom.xml"))
    sh(["mvn", "-q", "-s", os.path.join(W, "settings.xml"), "-f", os.path.join(W, "pom.xml"), "dependency:build-classpath",
        "-Dmdep.outputFile=" + cpf])
cp = []
for j in open(cpf).read().strip().split(os.pathsep):
    if j.endswith(".aar"):            # (androidx.test: the classes.jar inside)
        dst = os.path.join(W, "aar", os.path.basename(j)[:-4] + ".jar")
        if not os.path.isfile(dst):
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            with zipfile.ZipFile(j) as z: open(dst, "wb").write(z.read("classes.jar"))
        j = dst
    cp.append(j)

# 3. the app's resources (R.java + a resource APK for Robolectric's binary resources) and its Java
man = open(os.path.join(APP, "AndroidManifest.xml")).read()
man = man.replace('xmlns:android="http://schemas.android.com/apk/res/android"', 'xmlns:android="http://schemas.android.com/apk/res/android" package="io.github.tombi.port"', 1)
open(os.path.join(W, "AndroidManifest.xml"), "w").write(man)
gen = os.path.join(W, "gen"); shutil.rmtree(gen, ignore_errors=True)
sh([aapt2, "compile", "--dir", os.path.join(APP, "res"), "-o", os.path.join(W, "res.zip")])
sh([aapt2, "link", "-I", os.path.join(deps, os.path.basename(ALL_INSTR[0])), "--manifest", os.path.join(W, "AndroidManifest.xml"),
    "--min-sdk-version", "23", "--target-sdk-version", "34", "-A", os.path.join(W, "assets"), "-o", os.path.join(W, "res.apk"),
    "--java", gen, os.path.join(W, "res.zip")])
cfg = os.path.join(W, "cfg", "com", "android", "tools"); os.makedirs(cfg, exist_ok=True)
open(os.path.join(cfg, "test_config.properties"), "w").write(
    "android_resource_apk=%s\nandroid_merged_manifest=%s\nandroid_merged_assets=%s\nandroid_custom_package=io.github.tombi.port\n"
    % (os.path.join(W, "res.apk"), os.path.join(W, "AndroidManifest.xml"), os.path.join(W, "assets")))
srcs = [f for f in glob.glob(os.path.join(APP, "java", "io", "github", "tombi", "port", "*.java")) if not f.endswith("TombiActivity.java")]
srcs += glob.glob(os.path.join(HERE, "src", "io", "github", "tombi", "port", "*.java")) + glob.glob(os.path.join(gen, "**", "R.java"), recursive=True)
classes = os.path.join(W, "classes"); shutil.rmtree(classes, ignore_errors=True); os.makedirs(classes)
sh(["javac", "-nowarn", "-source", "8", "-target", "8", "-cp", os.pathsep.join(cp + [plain]), "-d", classes] + srcs)

# the launcher's disc check (libtombidisc.so) for the PC: the import runs as on the device
native = os.path.join(W, "native"); os.makedirs(native, exist_ok=True)
jh = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(shutil.which("javac")))), "include")
try:
    sh(["gcc", "-O2", "-shared", "-fPIC", "-I" + jh, "-I" + os.path.join(jh, "linux"), "-I" + os.path.join(ROOT, "port2", "include"),
        os.path.join(ROOT, "port2", "android", "disccheck.c"), os.path.join(ROOT, "port2", "shims", "cd_chd.c"),
        "-o", os.path.join(native, "libtombidisc.so")])
except (subprocess.CalledProcessError, OSError) as e:
    print("no PC libtombidisc.so (%s): the import stops before the disc check" % e)

# 4. the renders
os.makedirs(a.out, exist_ok=True)
props = ["-Djava.library.path=" + native, "-Drobolectric.offline=true", "-Drobolectric.dependency.dir=" + deps, "-Dout=" + os.path.abspath(a.out), "-Duilang=" + a.uilang]
if a.pal: props.append("-Dpal=" + os.path.abspath(a.pal))
if a.usa: props.append("-Dusa=" + os.path.abspath(a.usa))
if a.discs: props.append("-Ddiscs=" + os.path.abspath(a.discs))
cpath = os.pathsep.join([classes, os.path.join(W, "cfg")] + cp + [plain])
if a.test:   # one method (JUnitCore runs whole classes)
    open(os.path.join(W, "One.java"), "w").write(
        "public class One { public static void main(String[] a) { org.junit.runner.Result r = new org.junit.runner.JUnitCore().run("
        "org.junit.runner.Request.method(io.github.tombi.port.RenderTest.class, a[0])); for (org.junit.runner.notification.Failure f : "
        "r.getFailures()) System.out.println(f.getTrace()); System.out.println(r.wasSuccessful() ? \"OK\" : \"FAILED\"); System.exit(r.wasSuccessful() ? 0 : 1); } }\n")
    sh(["javac", "-nowarn", "-cp", cpath, "-d", classes, os.path.join(W, "One.java")])
    r = subprocess.run(["java"] + props + ["-cp", cpath, "One", a.test])
else:
    r = subprocess.run(["java"] + props + ["-cp", cpath, "org.junit.runner.JUnitCore", "io.github.tombi.port.RenderTest"])
sys.exit(r.returncode)
