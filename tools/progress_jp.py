#!/usr/bin/env python3
"""README block for the Japanese release (SLPS-01144): byte-matching progress per program from
build/slps01144/report.json (tools/build_report.py --version slps01144). Rewrites the text between
<!-- jp-progress:start --> and <!-- jp-progress:end --> in README.md."""
import json
import os
import re
import sys
from collections import OrderedDict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPORT = os.path.join(ROOT, "build", "slps01144", "report.json")
README = os.path.join(ROOT, "README.md")
LABEL = {"slps01144": "SLPS_011.44: engine", "x00": "X00.BIN: shared object code"}


def bar(p, w=10):
    """A row of coloured squares (green = matched, yellow = the partly matched cell, white = to do)."""
    full = int(p * w // 100)
    part = 1 if full < w and p * w / 100 - full >= 0.25 else 0
    return "\U0001F7E9" * full + "\U0001F7E8" * part + "\u2B1C" * (w - full - part)


def block(r):
    g = OrderedDict()
    for u in r["units"]:
        m = u.get("measures", {})
        a = g.setdefault(u["name"].split("/")[0], [0, 0, 0, 0])
        a[0] += int(m.get("total_code", 0)); a[1] += int(m.get("matched_code", 0))
        a[2] += int(m.get("total_functions", 0)); a[3] += int(m.get("matched_functions", 0))
    m = r["measures"]
    tc, mc = int(m["total_code"]), int(m["matched_code"])
    out = ["**Ore! Tomba (SLPS-01144): %.2f %% of the code matches byte for byte** (%d of %d bytes, %d of %d functions)."
           % (100.0 * mc / tc, mc, tc, int(m.get("matched_functions", 0)), int(m.get("total_functions", 0))), "",
           "| Part | Code | Matching bytes | Functions | Matching |", "|---|---:|---:|---:|---|"]
    for k, (c, mm, f, mf) in g.items():
        p = 100.0 * mm / c if c else 0.0
        out.append("| %s | %d B | %d B | %d / %d | %s %.1f %% |" % (LABEL.get(k, k.upper() + ".BIN: area code"), c, mm, mf, f, bar(p), p))
    out += ["", "Regenerated with `python3 tools/build_report.py --version slps01144` then `python3 tools/progress_jp.py`."
            " Analysis of JP-only code (Ghidra, PAL names ported by matching, functions with no PAL counterpart):"
            " [notes/jp_retail.md](notes/jp_retail.md)."]
    return "\n".join(out)


def main():
    r = json.load(open(REPORT, encoding="utf-8"))
    s = open(README, encoding="utf-8").read()
    new = "<!-- jp-progress:start -->\n" + block(r) + "\n<!-- jp-progress:end -->"
    s2, n = re.subn(r"<!-- jp-progress:start -->.*?<!-- jp-progress:end -->", lambda _: new, s, flags=re.S)
    if not n:
        sys.exit("README.md has no jp-progress markers")
    # Version table at the top of the README: the Japan row carries the live figures.
    m = r["measures"]
    tc, mc = int(m["total_code"]), int(m["matched_code"])
    row = ("| Japan (*Ore! Tomba*, `SLPS_011.44`) | Code %.2f %% | Data %.2f %% | Functions %d / %d | see [docs/JP.md](docs/JP.md) |"
           % (100.0 * mc / tc, float(m.get("matched_data_percent", 0.0)), int(m.get("matched_functions", 0)), int(m.get("total_functions", 0))))
    s2, n2 = re.subn(r"^\| Japan \(\*Ore! Tomba\*[^\n]*$", lambda _: row, s2, count=1, flags=re.M)
    if not n2:
        sys.exit("README.md has no Japan row in the version table")
    open(README, "w", encoding="utf-8").write(s2)
    print(block(r).splitlines()[0])


if __name__ == "__main__":
    main()
