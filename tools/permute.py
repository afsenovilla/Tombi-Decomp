#!/usr/bin/env python3
"""Our own permuter (CPU only, no AI): starts from approximate C (usually src/wip/X.c) and applies
random mutations (types, casts, operand/declaration order, register, flags) looking for
assembly that matches the game's. Measures the distance with `matchcheck --score`.

Usage: tools/permute.py src/wip/Fun.c [--rounds 200] [--pop 24] [--work /opt/psyq/pm_x] [--seed N]
If it reaches distance 0: copies it to src/<Name>.c and marks it with // MATCHING.
Otherwise, saves the best one to src/wip/<Name>.c (only if it improves on the original).
"""
import os, random, re, shutil, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
a = sys.argv[1:]
def opt(n, d): return a[a.index(n) + 1] if n in a else d
SRC = [x for x in a if x.endswith(".c")][0]
ROUNDS = int(opt("--rounds", 200)); POP = int(opt("--pop", 24))
WORK = opt("--work", "/opt/psyq/pm_" + os.path.basename(SRC)[:-2])
random.seed(int(opt("--seed", random.randrange(10**6))))
TYPES = ["int", "short", "unsigned short", "unsigned int", "unsigned char", "char"]
FLAGS = ["-O2 -G0", "-O1 -G0", "-O2 -G8", "-O1 -G8", "-O3 -G0", "-O0 -G0"]

DECL = re.compile(r"^(\s*)((?:register\s+)?(?:unsigned\s+)?(?:int|short|char|long)\b)(\s+\**\w+(?:\s*=[^;]+)?;)\s*$")
CAST = re.compile(r"\(((?:unsigned\s+)?(?:int|short|char))\)")
EXTERN = re.compile(r"^(extern\s+)((?:unsigned\s+)?(?:int|short|char)\b)(\s+\w+(?:\[\])?;)\s*$")
COMM = re.compile(r"\b([\w>.\-\[\]]+)\s*([+*&|^])\s*([\w>.\-\[\]]+)\b")


def m_local_type(L):
    idx = [i for i, l in enumerate(L) if DECL.match(l)]
    if not idx: return False
    i = random.choice(idx); m = DECL.match(L[i])
    t = random.choice(TYPES); reg = "register " if random.random() < 0.15 else ""
    L[i] = "%s%s%s%s" % (m.group(1), reg, t, m.group(3)); return True

def m_extern_type(L):
    idx = [i for i, l in enumerate(L) if EXTERN.match(l)]
    if not idx: return False
    i = random.choice(idx); m = EXTERN.match(L[i])
    L[i] = "%s%s%s" % (m.group(1), random.choice(TYPES), m.group(3)); return True

def m_cast(L):
    i = random.randrange(len(L)); m = list(CAST.finditer(L[i]))
    if m:  # change or remove an existing cast
        c = random.choice(m); new = "" if random.random() < 0.4 else "(%s)" % random.choice(TYPES)
        L[i] = L[i][:c.start()] + new + L[i][c.end():]; return True
    mm = re.search(r"=\s*([\w>.\-\[\]*()]+);", L[i])  # add a cast to the rvalue
    if mm and "(" not in mm.group(1)[:1]:
        L[i] = L[i][:mm.start(1)] + "(%s)" % random.choice(TYPES) + L[i][mm.start(1):]; return True
    return False

def m_swap_operands(L):
    i = random.randrange(len(L)); ms = list(COMM.finditer(L[i]))
    if not ms: return False
    m = random.choice(ms)
    L[i] = L[i][:m.start()] + "%s %s %s" % (m.group(3), m.group(2), m.group(1)) + L[i][m.end():]; return True

def m_swap_decls(L):
    idx = [i for i in range(len(L) - 1) if DECL.match(L[i]) and DECL.match(L[i + 1])]
    if not idx: return False
    i = random.choice(idx); L[i], L[i + 1] = L[i + 1], L[i]; return True

def m_compound(L):
    i = random.randrange(len(L))
    m = re.search(r"(\S+)\s*([+\-|&^]|<<|>>)=\s*([^;]+);", L[i])
    if m:
        L[i] = L[i][:m.start()] + "%s = %s %s %s;" % (m.group(1), m.group(1), m.group(2), m.group(3)) + L[i][m.end():]; return True
    m = re.search(r"(\S+)\s*=\s*\1\s*([+\-|&^]|<<|>>)\s*([^;]+);", L[i])
    if m:
        L[i] = L[i][:m.start()] + "%s %s= %s;" % (m.group(1), m.group(2), m.group(3)) + L[i][m.end():]; return True
    return False

def m_param_type(L):
    for i, l in enumerate(L):
        m = re.match(r"^(\w[\w\s\*]*?\b\w+)\(([^)]*)\)\s*$", l)
        if m and m.group(2).strip() not in ("", "void"):
            ps = [p.strip() for p in m.group(2).split(",")]
            k = random.randrange(len(ps)); mm = re.match(r"^(.*?)(\w+)$", ps[k])
            if mm and "*" not in ps[k]:
                ps[k] = "%s %s" % (random.choice(TYPES), mm.group(2))
                L[i] = "%s(%s)" % (m.group(1), ", ".join(ps)); return True
    return False

def m_flags(L):
    for i, l in enumerate(L):
        if l.startswith("// FLAGS"):
            L[i] = "// FLAGS " + random.choice(FLAGS); return True
    return False

def m_abs_addr(L):
    """DAT_xxxxxxxx -> access by absolute address (the compiler splits lui/addu/lw differently)."""
    decl = {}
    for l in L:
        m = re.match(r"^extern\s+((?:struct\s+)?[\w\s\*]+?)\s+(DAT_[0-9a-fA-F]{8})(\[\])?;", l)
        if m: decl[m.group(2)] = (m.group(1).strip(), bool(m.group(3)))
    if not decl: return False
    name = random.choice(list(decl)); ty, arr = decl[name]; addr = "0x" + name[4:]
    rep = "((%s *)%s)" % (ty, addr) if arr else "(*(%s *)%s)" % (ty, addr)
    ch = False
    for i, l in enumerate(L):
        if l.startswith("extern"): continue
        n = re.sub(r"\b%s\b" % name, rep, l)
        if n != l: L[i] = n; ch = True
    return ch

OPS = [m_abs_addr, m_abs_addr, m_local_type, m_local_type, m_extern_type, m_cast, m_cast, m_swap_operands, m_swap_decls,
       m_compound, m_param_type, m_flags]

def mutate(text):
    for _ in range(20):
        L = text.split("\n"); n = 0
        for _ in range(random.choice((1, 1, 2, 3))):
            n += bool(random.choice(OPS)(L))
        t = "\n".join(L)
        if n and t != text: return t
    return text


def score_all(cands):
    shutil.rmtree(WORK + "/c", ignore_errors=True); os.makedirs(WORK + "/c")
    files = []
    for k, t in enumerate(cands):
        p = "%s/c/m%d.c" % (WORK, k); open(p, "w").write(t); files.append(p)
    r = subprocess.run([sys.executable, ROOT + "/tools/matchcheck.py", "--score"] + files,
                       env=dict(os.environ, WORK=WORK + "/w"), capture_output=True, text=True, timeout=1800)
    out = {}
    for line in r.stdout.splitlines():
        m = re.match(r"SCORE m(\d+)\.c (\d+)", line)
        if m: out[int(m.group(1))] = int(m.group(2))
    return [out.get(k, 9999) for k in range(len(cands))]


def main():
    base = open(SRC).read()
    hm = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)", base)
    if not hm: sys.exit("missing // FUNC")
    name = os.path.basename(SRC)[:-2]
    best, bs = base, score_all([base])[0]
    print("initial:", bs, flush=True)
    for rd in range(ROUNDS):
        if bs == 0: break
        cands = [mutate(best) for _ in range(POP)]
        sc = score_all(cands)
        k = min(range(POP), key=lambda i: sc[i])
        if sc[k] <= bs and (sc[k] < bs or cands[k] != best):
            if sc[k] < bs: print("round %d: %d -> %d" % (rd, bs, sc[k]), flush=True)
            best, bs = cands[k], sc[k]
    if bs == 0:
        dst = "%s/src/%s.c" % (ROOT, name); open(dst, "w").write(best)
        subprocess.run([sys.executable, ROOT + "/tools/matchcheck.py", "--mark", dst],
                       env=dict(os.environ, WORK=WORK + "/w"), capture_output=True)
        if os.path.abspath(SRC) != os.path.abspath(dst) and "/wip/" in SRC: os.remove(SRC)
        print("MATCH", name)
    else:
        print("best distance:", bs)
        if bs < score_all([base])[0] and "/wip/" in SRC: open(SRC, "w").write(best)

main()
