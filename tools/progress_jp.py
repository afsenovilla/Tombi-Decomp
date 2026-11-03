#!/usr/bin/env python3
"""README block for the Japanese release (SLPS-01144) and the demo builds: byte-matching progress per program from
build/<ver>/report.json (tools/build_report.py --version <ver>). Rewrites the text between
<!-- jp-progress:start --> and <!-- jp-progress:end --> in README.md (the release) or, with --version slpm80165 /
beta980203 / pcpx96084, between <!-- <ver>-progress:start --> and <!-- <ver>-progress:end -->.

  python3 tools/progress_jp.py [--version slps01144|slpm80165|beta980203|pcpx96084]"""
import json
import os
import re
import sys
from collections import OrderedDict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from versions import version_arg
README = os.path.join(ROOT, "README.md")
LABEL = {"slps01144": "SLPS_011.44: engine", "x00": "X00.BIN: shared object code", "slpm80165": "SLPM_801.65: boot exe",
         "beta980203": "SLPS_011.44 (beta): boot exe", "tomba": "TOMBA.EXE: whole demo"}
# headline, README marker, regenerate note, per version
INFO = {
    "slps01144": ("Ore! Tomba (SLPS-01144)", "jp",
                  " Analysis of JP-only code (Ghidra, PAL names ported by matching, functions with no PAL counterpart):"
                  " [notes/jp_retail.md](notes/jp_retail.md)."),
    "slpm80165": ("Ore! Tomba demo (SLPM-80165)", "slpm80165",
                  " `python3 tools/demo_sweep.py` installs the PAL / JP C that matches demo functions as is"
                  " (see [docs/JPDEMO.md](docs/JPDEMO.md))."),
    "beta980203": ("Tomba! beta 1998-02-03", "beta980203", " See [docs/BETA_980203.md](docs/BETA_980203.md)."),
    "pcpx96084": ("Tomba! PrePre 8 (PCPX-96084)", "pcpx96084", " See [docs/PREPRE8.md](docs/PREPRE8.md)."),
}


def bar(p, w=10):
    """A row of coloured squares (green = matched, yellow = the partly matched cell, white = to do)."""
    full = int(p * w // 100)
    part = 1 if full < w and p * w / 100 - full >= 0.25 else 0
    return "\U0001F7E9" * full + "\U0001F7E8" * part + "⬜" * (w - full - part)


def block(r, ver="slps01144"):
    g = OrderedDict()
    for u in r["units"]:
        m = u.get("measures", {})
        a = g.setdefault(u["name"].split("/")[0], [0, 0, 0, 0])
        a[0] += int(m.get("total_code", 0)); a[1] += int(m.get("matched_code", 0))
        a[2] += int(m.get("total_functions", 0)); a[3] += int(m.get("matched_functions", 0))
    m = r["measures"]
    tc, mc = int(m["total_code"]), int(m["matched_code"])
    title, marker, note = INFO[ver]
    out = ["**%s: %.2f %% of the code matches byte for byte** (%d of %d bytes, %d of %d functions)."
           % (title, 100.0 * mc / tc, mc, tc, int(m.get("matched_functions", 0)), int(m.get("total_functions", 0))), "",
           "| Part | Code | Matching bytes | Functions | Matching |", "|---|---:|---:|---:|---|"]
    for k, (c, mm, f, mf) in g.items():
        p = 100.0 * mm / c if c else 0.0
        out.append("| %s | %d B | %d B | %d / %d | %s %.1f %% |" % (LABEL.get(k, k.upper() + ".BIN: area code"), c, mm, mf, f, bar(p), p))
    out += ["", "Regenerated with `python3 tools/build_report.py --version %s` then `python3 tools/progress_jp.py%s`.%s"
            % (ver, "" if ver == "slps01144" else " --version " + ver, note)]
    return "\n".join(out)


def main():
    ver = version_arg(sys.argv[1:], "slps01144")
    if ver not in INFO:
        sys.exit("no README block for version %s (%s)" % (ver, ", ".join(INFO)))
    report = os.path.join(ROOT, "build", ver, "report.json")
    r = json.load(open(report, encoding="utf-8"))
    s = open(README, encoding="utf-8").read()
    marker = INFO[ver][1]
    new = "<!-- %s-progress:start -->\n" % marker + block(r, ver) + "\n<!-- %s-progress:end -->" % marker
    s2, n = re.subn(r"<!-- %s-progress:start -->.*?<!-- %s-progress:end -->" % (marker, marker), lambda _: new, s, flags=re.S)
    if not n:
        sys.exit("README.md has no %s-progress markers" % marker)
    open(README, "w", encoding="utf-8").write(s2)
    print(block(r, ver).splitlines()[0])


if __name__ == "__main__":
    main()
