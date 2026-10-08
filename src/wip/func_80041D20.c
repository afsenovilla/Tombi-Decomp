// FUNC 80041d20 412 MAIN0
// r11: score 20; left: store of DAT_1f800278(q+8) and ws sign-extension should precede the q[3] load; ws/d regs a1/a2 swapped.
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
    k = (q[3] >> 4) & 0xf;
    if (ws == 0) goto next;
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
