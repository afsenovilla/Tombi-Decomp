#!/usr/bin/env python3
"""The pause map's graph of a Tomba!/Tombi! disc: nodes, links, routes, and what nothing refers to.

The sub-screen overlay (SYSTEM/D0SPSUB.BIN on PAL, SYSTEM/DSPSUB.BIN on JP; found by content, not by name) holds
MAP_NODES ({short x y z pad}[], the map positions), the per-region route lists ({short type a b shape}, type 0 a dot
at node a, type 1 a strip from a to b, -1 ends a list; 12 regions back to back right after the nodes), and MAP_LINKS
({u8 a b c d e f}[]: the segments the marker walks, c/d = the area / room of the name tables).  A node that no link
and no route uses is a leftover (PAL retail: node 41, in the sea west of Mushroom Forest).

  python3 tools/map_tables.py DIR_OR_FILE [--ref data/SYSTEM/D0SPSUB.BIN] [--dump]

DIR is an extracted disc (tools/isoext.py / extract_data.py); every file under it is scanned.  --ref diffs the
node / link / route tables against another overlay (the PAL one by default when data/ is there).  Prints a
markdown summary (used by the map-tables workflow's job summary); nothing of the disc is written anywhere."""
import os, struct, sys

NODE_Y = (-1200, -700)          # map nodes sit at this height; route entries and other data do not
SIGNATURE = struct.pack("<8h", -233, -944, -284, 0, -185, -958, -266, 0)   # retail nodes 0 and 1
NODE41 = struct.pack("<3h", -269, -974, -40)


def nodes_at(d, o):
    """How many consecutive {x y z pad} records at o look like map nodes."""
    n = 0
    while o + 8 <= len(d):
        x, y, z, p = struct.unpack_from("<4h", d, o)
        if not (NODE_Y[0] <= y <= NODE_Y[1]) or p != 0 or abs(x) > 2000 or abs(z) > 2000:
            break
        n += 1; o += 8
    return n


def find_nodes(d):
    """[(offset, count)] of every run of 20+ node-like records (4-aligned; MAP_ROOM_POSITIONS looks alike)."""
    runs = []; o = 0
    while o + 8 <= len(d):
        n = nodes_at(d, o)
        if n >= 20:
            runs.append((o, n))
        o += 4 if n == 0 else 8 * n
    return runs


def read_routes(d, o, nnodes):
    """12 route lists starting at o; None if they do not parse."""
    out = []; p = o
    for r in range(12):
        lst = []
        while True:
            if p + 8 > len(d):
                return None, p
            t, a, b, s = struct.unpack_from("<4h", d, p); p += 8
            if t == -1:
                break
            if t not in (0, 1) or not (0 <= a < 256) or len(lst) > 64:
                return None, p
            lst.append((t, a, b, s))
        out.append(lst)
    return out, p


def find_links(d, o, nnodes):
    """The MAP_LINKS run after o: 6-byte records {a b c d e f} with plausible fields, at least 20 of them."""
    def ok(q):
        a, b, c, e, f, g = d[q:q + 6]
        return a < nnodes and b < nnodes and c < 24 and e < 24 and f < 16 and g == 0
    p = o
    while p + 6 * 20 <= len(d):
        if all(ok(p + 6 * i) for i in range(20)) and d[p] == 0 and d[p + 1] == 1:
            n = 0
            while p + 6 * (n + 1) <= len(d) and ok(p + 6 * n) and not (n and d[p + 6 * n] == 0 and d[p + 6 * n + 1] == 0):
                n += 1
            return p, n
        p += 2
    return None, 0


def analyse(d):
    """The run of nodes that the 12 route lists follow (the retail layout); None when there is none."""
    best = None
    for o, n in find_nodes(d):
        # the PAL retail adds nodes behind the JP ones, so the route lists may start after a shorter run
        nn = n
        routes, rend = read_routes(d, o + 8 * nn, nn)
        while routes is None and nn > 20:
            nn -= 1
            routes, rend = read_routes(d, o + 8 * nn, nn)
        if routes is None or sum(1 for l in routes if l) < 6:
            continue
        if best is None or nn > len(best[1]):
            best = (o, [struct.unpack_from("<3h", d, o + 8 * i) for i in range(nn)], routes, rend)
    if best is None:
        return None
    o, nodes, routes, rend = best; nn = len(nodes)
    lo, nl = find_links(d, rend, max(nn, 1))
    links = [tuple(d[lo + 6 * i:lo + 6 * i + 6]) for i in range(nl)] if lo is not None else []
    ref = set()
    for lst in routes:
        for t, a, b, s in lst:
            ref.add(a)
            if t:
                ref.add(b)
    for l in links:
        ref.add(l[0]); ref.add(l[1])
    orphans = [i for i in range(nn) if i not in ref and nodes[i][:2] != (0, 0)]
    return dict(off=o, nodes=nodes[:nn], routes=routes, links=links, link_off=lo, orphans=orphans, ref=ref)


def scan(path):
    """Every file under path (or the file itself) that holds a map graph: [(relpath, analysis)]."""
    found = []
    files = [path] if os.path.isfile(path) else [os.path.join(r, f) for r, _, fs in os.walk(path) for f in fs]
    for f in sorted(files):
        try:
            if os.path.getsize(f) > 4 << 20 or os.path.getsize(f) < 4096:
                continue
            d = open(f, "rb").read()
        except OSError:
            continue
        a = analyse(d)
        name = os.path.relpath(f, path) if os.path.isdir(path) else os.path.basename(f)
        if a:
            found.append((name, a))
        else:
            # no parseable graph: say where the retail layout's first nodes or node 41 appear, if they do
            hits = [(k, d.find(v)) for k, v in (("retail nodes 0-1", SIGNATURE), ("node 41 coords", NODE41)) if d.find(v) >= 0]
            runs = [(o, n) for o, n in find_nodes(d) if n >= 20]
            if hits or runs:
                found.append((name, dict(partial=True, hits=hits, runs=runs, nodes_at=[struct.unpack_from("<3h", d, o + 8 * i) for o, n in runs[:1] for i in range(min(n, 70))])))
    return found


def report(name, a, ref=None, dump=False):
    if a.get("partial"):
        out = [f"### {name} (no parseable graph)", ""]
        for k, o in a["hits"]:
            out.append(f"- {k} at file offset 0x{o:X}")
        for o, n in a["runs"]:
            out.append(f"- a run of {n} node-like records at 0x{o:X}, not followed by route lists")
        if dump and a["nodes_at"]:
            out.append("nodes: " + "; ".join(f"{i} {n}" for i, n in enumerate(a["nodes_at"])))
        return "\n".join(out)
    out = [f"### {name}", "",
           f"- map graph at file offset 0x{a['off']:X}: **{len(a['nodes'])} nodes**, {len(a['links'])} links"
           + (f" (at 0x{a['link_off']:X})" if a['link_off'] is not None else " (links not found)")
           + f", routes in {len(a['routes'])} regions ({sum(len(l) for l in a['routes'])} entries)",
           f"- nodes nothing refers to (no link, no route): **{a['orphans'] or 'none'}**"
           + ("  " + ", ".join(f"{i} at ({a['nodes'][i][0]}, {a['nodes'][i][1]}, {a['nodes'][i][2]})" for i in a['orphans']) if a['orphans'] else "")]
    if ref:
        rn, an = ref['nodes'], a['nodes']
        common = min(len(rn), len(an))
        moved = [i for i in range(common) if rn[i] != an[i]]
        out.append(f"- vs reference: {len(an)} nodes here, {len(rn)} there; {len(moved)} of the first {common} differ"
                   + (": " + ", ".join(f"{i} {an[i]} vs {rn[i]}" for i in moved[:12]) if moved else ""))
        if len(an) != len(rn):
            extra = an[common:] if len(an) > len(rn) else rn[common:]
            out.append(f"- {'extra here' if len(an) > len(rn) else 'only in the reference'}: " + ", ".join(f"{common + i} {n}" for i, n in enumerate(extra)))
        for r in range(min(len(a['routes']), len(ref['routes']))):
            if a['routes'][r] != ref['routes'][r]:
                out.append(f"- region {r} routes differ: here {a['routes'][r]}; reference {ref['routes'][r]}")
        if a['links'] != ref['links']:
            la, lr = a['links'], ref['links']
            diff = [i for i in range(min(len(la), len(lr))) if la[i] != lr[i]]
            out.append(f"- links: {len(la)} here, {len(lr)} in the reference; {len(diff)} of the common ones differ"
                       + (": " + ", ".join(f"{i} {la[i]} vs {lr[i]}" for i in diff[:12]) if diff else ""))
    if dump:
        out.append(""); out.append("nodes: " + "; ".join(f"{i} {n}" for i, n in enumerate(a['nodes'])))
        for r, lst in enumerate(a['routes']):
            out.append(f"region {r}: {lst}")
        out.append("links: " + "; ".join(f"{i} {l}" for i, l in enumerate(a['links'])))
    return "\n".join(out)


def main(argv):
    if len(argv) < 2 or argv[1] in ("-h", "--help"):
        print(__doc__); return 2
    path = argv[1]; ref_path = None; dump = "--dump" in argv
    if "--ref" in argv:
        ref_path = argv[argv.index("--ref") + 1]
    elif os.path.isfile(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "data", "SYSTEM", "D0SPSUB.BIN")):
        ref_path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "data", "SYSTEM", "D0SPSUB.BIN")
    ref = None
    if ref_path and os.path.abspath(ref_path) != os.path.abspath(path):
        r = scan(ref_path)
        ref = r[0][1] if r else None
    found = scan(path)
    if not found:
        print(f"no map graph found under {path} (no run of 20+ {{x y z 0}} records with y in {NODE_Y})"); return 1
    for name, a in found:
        print(report(name, a, ref, dump)); print()
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
