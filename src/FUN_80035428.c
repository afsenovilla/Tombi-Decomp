// FUNC 80035428 580 MAIN0
// MATCHING 80035428 580
#include "TOBJ.H"
extern short FUN_800411cc(TObj *, short, short);
extern void FUN_800eaffc(short, short, short, unsigned char);
extern void FUN_8001f96c(int, short, short, short);
extern void FUN_8001e4f0(int);
extern short FUN_80041ebc(void *, short, short);
extern void FUN_800352f4(TObj *);
extern unsigned char DAT_800a60d6;
extern unsigned short DAT_1f800282;
extern unsigned short DAT_1f800282b;
extern TObj *DAT_80096330;
extern char DAT_800a6038[];
extern short *DAT_800a6078;
extern unsigned short DAT_800a604e;
extern unsigned short DAT_800a60a8;

int FUN_80035428(TObj *o)
{
    short n;
    int k;

    if (o->active == 1 && FUN_800411cc(o, o->h->p.whole, o->y.p.whole)) {
        o->active = 2;
        o->ba5 = 0;
        o->b69 = 0;
        if (DAT_800a60d6 == 0) {
            if (!(DAT_1f800282 & 0x800)) {
                if (DAT_1f800282 & 2) o->b69 = 1;
            }
        }
        k = (DAT_1f800282b >> 5) & 0xf;
        if (!(DAT_1f800282b & 0x3000)) {
            switch (k) {
            case 1:
            case 2:
            case 3:
                FUN_800eaffc(o->h->p.whole, o->y.p.whole, o->d->p.whole, o->animFrame);
                break;
            }
        }
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e4f0(5);
        o->wa8 = 0x4ff;
    }
    if (o->b69 != 0 && DAT_800a60d6 != 4 && DAT_800a60d6 != 7) {
    n = 0;
    switch ((signed char)DAT_80096330->substep) {
    case 0:
    case 2:
        if (FUN_80041ebc(DAT_800a6038, DAT_800a6078[1] + 0x10, DAT_800a604e + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    case 1:
    case 3:
        if (FUN_80041ebc(DAT_800a6038, DAT_800a6078[1] - 0x10, DAT_800a604e + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    }
    if (n == 0) {
        FUN_800352f4(o);
        return 1;
    }
    return 0;
    }
    o->b69 = 0;
    return 0;
}
