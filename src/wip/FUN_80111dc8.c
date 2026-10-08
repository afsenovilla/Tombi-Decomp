// FUNC 80111dc8 400 X000
// wip score 65: (d8c - 0x80) - w78 gets folded to d8c - (w78 + 0x80); start of the b69 block has swapped regs (velX + s)
#include "TOBJ.H"
extern short DAT_8013c984[];
extern short DAT_8007a1f0[], DAT_8007a5f0[];

void FUN_80111dc8(TObj *o)
{
    int u;
    unsigned v;
    short s;
    int t;
    int a;
    if (o->b69 != 0) {
        o->b69 = 0;
        u = (o->d38 + 0x800 & 0xfff) >> 4;
        o->w78 = u;
        if (0x80 < u)
            o->w78 = 0x80;
        o->velY = 0x100;
        s = *(short *)((char *)DAT_8013c984 + ((o->w78 - 0x40U) >> 2 & 0x1e));
        if (s == 0 && (s = -4, o->velX < 0))
            s = 4;
        s = o->velX + s;
        o->velX = s;
        a = (short)-s;
        o->velV = (a * DAT_8007a1f0[o->w78]) >> 12;
        a = (a * DAT_8007a5f0[o->w78]) >> 12;
        o->velH = a;
        o->h->raw = o->h->raw + ((a << 16) >> 8);
        t = o->velV;
    } else {
        s = o->velY + 0x20;
        o->velY = s;
        if (0x380 < s)
            o->velY = 0x380;
        t = o->velY;
    }
    o->y.raw = o->y.raw + t * 0x100;
    a = o->d8c;
    v = (int)(a - 0x80) - (unsigned short)o->w78 & 0xff;
    if (v != 0) {
        int b = a - 1;
        if (0x7f < v)
            b = a + 1;
        o->d8c = b;
        o->d8c = *(unsigned char *)&o->d8c;
    }
}
