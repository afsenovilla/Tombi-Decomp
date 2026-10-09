// FUNC 801282d0 788 X001
// MATCHING 801282d0 788
#include "TOBJ.H"

extern unsigned char D_8013C7B8[], D_8013C7C8[];
extern char D_80077CE8[];
extern void *D_8013FCB0[], *D_8013FCB8[];
extern short D_1F80027E, D_1F800284, D_1F80016A;
extern unsigned int FUN_8001f9e0(void);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fb20(TObj *);
extern short FUN_80040278(TObj *, short, short);

void func_801282D0(TObj *o)
{
    short t;

    switch (o->state) {
    case 0:
        o->b9c = 0;
        o->wb0 = 0;
        if (D_8013C7B8[FUN_8001f9e0() & 0xf])
            o->timer = 0x180;
        else
            o->timer = 0xc0;
        o->movetab = D_80077CE8;
        o->wac = 0x10;
        o->state++;
        o->anim = D_8013FCB0[0];
        FUN_8001fe6c(o);
    case 1:
        if (o->visible)
            o->state++;
        break;
    case 2:
        FUN_8001fec0(o);
        if (o->d8c != 0x40 && o->d8c != 0xc0) {
            FUN_8001fb20(o);
            if (o->b69 == 1) {
                o->wae = -1;
                o->wb2 = 0;
                o->b69 = 0;
                o->wba = 1;
            } else if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
                o->b69 = 0;
                o->wba = 0;
                t = D_1F80027E;
                o->wb2 = t;
                o->wae = D_1F800284;
                o->d8c = -(t << 2) & 0xff;
            }
        }
        if (--o->timer == -1) {
            o->timer = 0x80;
            o->state++;
            if (o->w22)
                o->animFrame = o->h->p.whole > o->wb4;
            else
                o->animFrame = D_1F80016A < o->h->p.whole;
            o->wac = 0x12;
            o->anim = D_8013FCB8[0];
            FUN_8001fe6c(o);
        }
        break;
    case 3:
        if (FUN_8001fec0(o)) {
            o->state = 0;
            switch (o->w98) {
            case 0:
            case 1:
                if (D_8013C7B8[FUN_8001f9e0() & 0xf])
                    o->step = 3;
                else
                    o->step = 1;
                break;
            case 2:
                if (D_8013C7C8[FUN_8001f9e0() & 0xf])
                    o->step = 1;
                else
                    o->step = 3;
                break;
            case 3:
                if (D_8013C7B8[FUN_8001f9e0() & 0xf])
                    o->step = 1;
                else
                    o->step = 3;
                break;
            }
        }
        break;
    }
}
