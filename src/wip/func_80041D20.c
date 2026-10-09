// FUNC 80041d20 412 MAIN0
/* score 16 (o37): structure now matches; only the hard regs of k/ws/d differ (game k a0, ws a1, d a2; ours ws a0, d a1, k a2).
   -dg: ws 3 refs/5 insns (0.6) and d 3/7 (0.43) are allocated before k 2/9 (0.22, pref a0). The raw read
   `*(unsigned short *)((char *)q + 6)` keeps the q[3] load after the DAT_1f800278 store (q[3] was hoisted), and loading
   t3 before the `ws == 0` test with k = (t3 >> 4) & 0xf after it puts srl in the delay slot like the game.
   Tried: multi-set k (k = t3 >> 4; k &= 0xf gives k a0 but the andi sinks to the next delay slot and ws/d swap), k/ws/d/x/w
   types, ws = w = *DAT++, d = b - x; d -= w. Needs k ranked above ws and d (4 refs) with the andi still freeing a register. */
#include "TOBJ.H"
extern short *func_8003F200(int, int);
extern short FUN_800205d8(int);
extern unsigned short *DAT_1f800278;
extern short DAT_1f80027c;

int func_80041D20(TObj *o, short a, int b)
{
    char pad[8];
    short *p;
    unsigned short *q;
    unsigned short f;
    int n, cnt, x, w, d, e;
    unsigned int k;
    short s;
    short ws;
    unsigned short t3;
    p = func_8003F200(a, o->d->p.whole);
    DAT_1f800278 = (unsigned short *)(p + 1);
    n = cnt = *p;
    goto test;
top:
    q = DAT_1f800278;
    DAT_1f800278 = q + 1;
    f = *q;
    cnt--;
    if (f & 0x4000) {
        if (!(f & 0x10)) goto hit;
    }
    DAT_1f800278 = q + 4;
    goto next;
hit:
    x = *DAT_1f800278++;
    w = *DAT_1f800278++;
    DAT_1f800278++;
    ws = w;
    t3 = *(unsigned short *)((char *)q + 6);
    if (ws == 0) goto next;
    k = (t3 >> 4) & 0xf;
    d = b - x - w;
    if (((d - 1) & 0xffff) > -ws) {
        if (d << 16 > 0) return 0;
        goto next;
    }
    s = FUN_800205d8(k);
    DAT_1f80027c = s;
    if (s < 0x40) {
        DAT_1f80027c = s + 0xc0;
    } else if (s > 0xc0) {
        DAT_1f80027c = s - 0xc0;
    }
    e = x + 8;
    if (((b - e - w - 1) & 0xffff) > -(short)w) return 2;
    return 1;
next:
    n = cnt << 16;
test:
    if (n != 0) goto top;
    return 0;
}
