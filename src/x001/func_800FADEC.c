// FUNC 800fadec 680 X001
// MATCHING 800fadec 680
#include "TOBJ.H"
typedef struct { TObj t; char c0[7]; unsigned char bc7; unsigned char bc8; } PO;
typedef struct { char pad[8]; unsigned char b8; char pad2[0x20 - 9]; short w20; short w22; } P;
extern P *D_8009C330;
extern int D_8009C934;
extern unsigned short D_8009D670;
extern unsigned char D_801152E8[];
extern short FUN_800411cc(PO *, int, int);
extern void SfxPlay3(int, int);
extern short ObjTileCollide(PO *, int, int);

static __inline__ short cond(PO *o)
{
    int v = 0;
    if (o->t.wb4 < 0x29) {
        v = (*(volatile unsigned short *)&D_8009D670 >> 4) & 1;
    }
    return v;
}

void func_800FADEC(PO *o)
{
    int one, s;
    short n, k;
    if (*(unsigned short *)0x1F8001FC & (*(unsigned short *)0x1F8003C6 | *(unsigned short *)0x1F8003C8)) {
        one = 1;
        D_8009C934 = 0;
        o->bc7 = one;
        o->t.ba4 = 0;
        *(unsigned char *)&o->t.wac = 0;
        o->t.b69 = 0;
        o->t.b9c = 0;
        o->t.b9d = 0;
        o->bc8 = 0;
        o->t.ba7 = 0;
        o->t.velH = 0;
        o->t.velV = 0;
        o->t.velX = 0;
        o->t.velY = 0;
        o->t.d84 = 0;
        o->t.d88 = 0;
        D_8009C330->b8 = 0;
        D_8009C330->w20 = 0;
        D_8009C330->w22 = 0;
        if (cond(o)) {
            o->t.h->p.whole = o->t.d30;
            o->t.y.p.whole = o->t.d34 + 0x14;
            k = FUN_800411cc(o, o->t.h->p.whole, (short)(o->t.y.p.whole - 0x40)) != 0;
            n = k;
            if (FUN_800411cc(o, o->t.h->p.whole, (short)(o->t.y.p.whole - 0x50))) n = k + 1;
            if (FUN_800411cc(o, o->t.h->p.whole, (short)(o->t.y.p.whole - 0x60))) n++;
            if (n == 0) {
                *(signed char *)&o->t.b0f = -20;
                *(unsigned char *)&o->t.waa = one;
                o->t.step = 0x49;
                o->t.state = 0;
            } else {
                o->t.step = 2;
                o->t.state = 0;
            }
        } else {
            o->t.step = 0x37;
            o->t.state = 0;
        }
    } else if (o->t.b69 == 1) {
        D_8009C330->w20 = 0;
        D_8009C330->w22 = 0;
        D_8009C330->b8 = 0;
        D_8009C934 = 0;
        SfxPlay3(0x1c, 0x7f);
        o->bc7 = 1;
        o->t.b9c = 0;
        *(unsigned char *)&o->t.wac = 0;
        o->t.b9d = 0;
        o->bc8 = 0;
        o->t.d8c = D_801152E8[o->t.wb0];
        o->t.step = 0;
        o->t.state = 0;
    } else if (ObjTileCollide(o, 0, 0)) {
        D_8009C330->w20 = 0;
        D_8009C330->w22 = 0;
        D_8009C330->b8 = 0;
        D_8009C934 = 0;
        SfxPlay3(0x1c, 0x7f);
        o->bc7 = 1;
        o->t.b9c = 0;
        *(unsigned char *)&o->t.wac = 0;
        o->t.b9d = 0;
        o->bc8 = 0;
        o->t.d8c = D_801152E8[o->t.wb0];
        o->t.step = 1;
        o->t.state = 0;
    }
}
