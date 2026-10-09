// FUNC 8013bab0 372 X001
// MATCHING 8013bab0 372
#include "TOBJ.H"

extern unsigned char D_8009CEAB;
extern unsigned char D_800A60A1;
extern Fix16 *D_800A6078;
extern short D_800A604E;
extern signed char D_8009D2B0;
extern unsigned short D_1F8001FC;
extern unsigned short D_1F8003C4;
extern unsigned char D_8009C942;
extern unsigned char D_800A60F8;
extern void func_8013A0D8(TObj *);
extern void func_8013A2D8(TObj *);
extern void PoolFree_1F800210(TObj *);

static __inline__ short check(TObj *o)
{
    int r = 0;

    if (D_8009CEAB != 0 && D_8009CEAB < 4 && D_800A60A1 != 0
        && (unsigned short)(D_800A6078->p.whole - 0xaad) < 0x40 && D_800A604E >= -0x10f) {
        if (o->animTimer != 0) {
            r = 1;
        } else if (D_8009D2B0 == 1 && (D_1F8001FC & D_1F8003C4)) {
            r = 1;
            D_8009C942 = 1;
            D_800A60F8 = 1;
            o->animTimer = 1;
        }
    }
    return r;
}

void func_8013BAB0(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8013A0D8(o);
        break;
    case 1:
        if (check(o)) func_8013A2D8(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
