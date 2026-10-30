import json, sys, numpy as np
J = json.load(open("/dev/shm/clut2/corpus.json")); idx = J["idx"]; names = J["names"]
W = np.memmap("/dev/shm/clut2/corpus.u16", np.uint16, "r")
def where(o):
    for h, ph, s, n in idx:
        if s <= o < s + n: return names[h], ph, (o - s) * 2 + ph
if __name__ == "__main__":
    for a in sys.argv[1:]:
        o = int(a); print(o, where(o)); print(" ".join("%04X" % x for x in W[o:o+64]))
