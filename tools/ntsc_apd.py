#!/usr/bin/env python3
"""The USA disc's object placements where the PAL disc moved them for its 256-line picture (docs/PORT.md "NTSC
picture"): the APD placement blocks (load-record kinds 0x80..0x86, 16-byte records) of the USA load lists
(SYS/LDSYS.BIN) against the PAL ones (SYS/LD0ARnn.BIN), record by record. A record that differs only in its y word
(+8) is written out with all its PAL words (the fingerprint the port checks before it patches) and the USA y.

  python3 tools/isoext.py game_usa/tomba_us.bin US_FS            (once)
  python3 -I tools/ntsc_apd.py US_FS game_usa/tomba_us.bin [-o port2/runtime/ntsc_apd.inc]

Prints a summary; with -o writes the table port2/runtime/ntsc.c includes (numbers only, no game data)."""
import argparse, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import area000 as A  # noqa: E402
import usa_compare as U  # noqa: E402


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("usfs")
    ap.add_argument("usdisc")
    ap.add_argument("-o")
    a = ap.parse_args()
    us_lists, pal_lists = U.usa_lists(a.usdisc, a.usfs), A.load_lists()
    rows, other = [], []
    for ld, lists in sorted(us_lists.items()):
        if ld not in pal_lists or not ld[2:].isdigit():
            continue
        area = int(ld[2:])
        for i, lst in enumerate(lists):
            if i >= len(pal_lists[ld]):
                continue
            ub = [(r, d) for r, d in U.blocks(lst, a.usfs) if 0x80 <= r["kind"] <= 0x8F]
            pb = [(r, d) for r, d in A.list_blocks(ld, i) if 0x80 <= r["kind"] <= 0x8F]
            for (ru, du), (rp, dp) in zip(ub, pb):
                if du is None or dp is None or du == dp or ru["kind"] != rp["kind"]:
                    continue
                if len(du) != len(dp):
                    other.append("%s list %d kind %02X: %d / %d bytes (records reordered), not patched" % (ld, i, ru["kind"], len(dp), len(du)))
                    continue
                for o in range(0, len(du) - 15, 16):
                    if du[o:o + 16] == dp[o:o + 16]:
                        continue
                    wp, wu = struct.unpack_from("<8h", dp, o), struct.unpack_from("<8h", du, o)
                    if wp[:4] + wp[5:] != wu[:4] + wu[5:]:
                        other.append("%s list %d kind %02X record %d: more than y differs, not patched" % (ld, i, ru["kind"], o // 16))
                        continue
                    rows.append((area, ru["kind"] & 15, o // 16, wp, wu[4]))
    uniq = sorted(set((ar, k, n, wp, uy) for ar, k, n, wp, uy in rows))
    print("%d records (%d distinct) in %d blocks differ in y only" % (len(rows), len(uniq), len({(r[0], r[1]) for r in rows})))
    for s in other:
        print("  " + s)
    if a.o:
        with open(a.o, "w") as f:
            f.write("/* tools/ntsc_apd.py: APD placement records the USA disc has higher than the PAL disc (area, APD slot,\n"
                    "   record, the PAL record's eight words, USA y). Generated; do not edit. */\n")
            for ar, k, n, wp, uy in uniq:
                f.write("{%d, %d, %d, {%s}, %d},\n" % (ar, k, n, ", ".join(str(w) for w in wp), uy))
        print("wrote %s" % a.o)


if __name__ == "__main__":
    main()
