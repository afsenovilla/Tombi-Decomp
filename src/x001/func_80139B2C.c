// FUNC 80139b2c 292 X001
// MATCHING 80139b2c 292
#include "TOBJ.H"

extern unsigned char D_8009CDB2;
extern int D_8009C984;
extern unsigned char D_800A603C, D_800A60A1, D_8009C942, D_800A60F8;
extern unsigned short D_800A604E;
extern Fix16 *D_800A6078;
extern signed char D_8009D2B0;
extern unsigned short D_1F8001FC, D_1F8003C4;

int func_80139B2C(TObj *o)
{
    TObj *p = *(TObj **)&o->category;
    int r = 0;

    if (D_8009CDB2 >= 2) return 0;
    if (!(D_8009C984 & 0x100) && D_800A603C == 1 && D_800A60A1 &&
        (unsigned short)(D_800A604E + 0x50a) < 0x10 &&
        (unsigned short)(D_800A6078->p.whole - p->h->p.whole + 0x30) < 0x60) {
        if (*(unsigned short *)&o->anim) {
            r = 1;
        } else if (D_8009D2B0 == D_800A603C && (D_1F8001FC & D_1F8003C4)) {
            r = 1;
            *(unsigned short *)&o->anim = 1;
            D_8009C942 = 1;
            D_800A60F8 = 1;
        }
    }
    return r;
}
