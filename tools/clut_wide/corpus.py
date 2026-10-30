import sys, os, hashlib, json, numpy as np
sys.path.insert(0, "tools"); import area000 as A
DISCS = [("pal","game/disc/fs"),("usa","/dev/shm/usa/fs"),("jp","/dev/shm/jp/fs"),("beta","/dev/shm/beta/fs"),
         ("jpdemo","/dev/shm/jpdemo/fs"),("pp8","/dev/shm/pp8"),("pp8u","/dev/shm/pp8u")]
seen = {}; idx = []; out = open("/dev/shm/clut2/corpus.u16", "wb"); pos = 0
PAD = np.zeros(1024, np.uint16).tobytes()
def add(name, d):
    global pos
    h = hashlib.md5(d).hexdigest()
    if h in seen: seen[h].append(name); return
    seen[h] = [name]
    for ph in (0, 1):
        b = d[ph:]; b = b[:len(b)//2*2]
        if len(b) < 64: continue
        idx.append((h, ph, pos, len(b)//2)); out.write(b); out.write(PAD); pos += len(b)//2 + 1024
for disc, fs in DISCS:
    for dp, dns, fns in os.walk(fs):
        dns[:] = sorted(x for x in dns if x != "MOVIE"); 
        for f in sorted(fns):
            p = os.path.join(dp, f); nm = disc + ":" + os.path.relpath(p, fs)
            if f.upper().endswith(".STR"): continue
            d = open(p, "rb").read(); add(nm, d)
            if f.upper().endswith(".GAM"):
                try: add(nm + "#unpacked", A.gam_unpack(d))
                except Exception: pass
out.close()
json.dump(dict(idx=idx, names=seen), open("/dev/shm/clut2/corpus.json", "w"))
print(len(seen), "unique files", pos, "words")
