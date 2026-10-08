// FUNC 800428c0 188 MAIN0
// code identico salvo tabla de saltos (.text de ours > size): matchcheck no lo acepta
#include "TOBJ.H"
extern int FUN_800425c4(TObj *a, char *b, int c);
extern void FUN_8001f96c(int a, int b, int c, int d);
extern void SfxPlay(int a);

int FUN_800428c0(TObj *o, char *p, int c)
{
    int k;
    register int r asm("$17");
    r = 1;
    p[0x68] = 1;
    p[0x9e] = 0;
    k = FUN_800425c4(o, p, c);
    switch (k) {
    case 1: case 7:
        r = 1;
        break;
    case 4: case 5: case 10:
        r = 1;
        goto L;
    case 0: case 3: case 9:
        r = 2;
        break;
    case 13:
        p[0x68] = 0;
        r = -1;
        break;
    case 6: case 11: case 12:
        r = 2;
    L:
        o->b6a = 1;
        o->active = 2;
        o->wa8 = 0x4ff;
        break;
    case 2: case 8:
        r = 0;
        p[0x9e] = 1;
        p[0x9f] = 0;
        break;
    }
    if (r < 0) {
        r = 0;
    } else {
        int s;
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        s = 7;
        if ((p[0x1c] & 0x7f) == 4) s = 6;
        SfxPlay(s);
    }
    return r;
}
