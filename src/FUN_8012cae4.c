// FUNC 8012cae4 532 X000
// MATCHING 8012cae4 532
#include "TOBJ.H"
extern char DAT_80077cf4[];
extern unsigned char DAT_801390ec[];
extern unsigned short *DAT_8013a160[];
extern unsigned char DAT_80138fd8[];
extern unsigned short DAT_1f80016a, DAT_1f800172;
extern void FUN_80127214(TObj *);
extern void FUN_8001fb20(TObj *);
extern int FUN_801274cc(TObj *);

void FUN_8012cae4(TObj *o)
{
    unsigned char *p;
    unsigned short k;
    int dx;
    switch (o->state) {
    case 0:
        o->movetab = DAT_80077cf4;
        o->state++;
        o->b69 = 0;
        k = DAT_801390ec[o->subtype];
        o->wac = k;
        o->anim = DAT_8013a160[k];
        p = &DAT_80138fd8[((unsigned short *)o->anim)[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 1:
        FUN_80127214(o);
        FUN_8001fb20(o);
        if (FUN_801274cc(o) != 0) {
            o->timer = 0;
            o->state++;
        }
        break;
    case 2:
        FUN_80127214(o);
        dx = (unsigned short)o->h->p.whole - DAT_1f80016a + 0x60;
        if ((unsigned short)(o->d->p.whole - DAT_1f800172 + 0x2d) >= 0x5b || (unsigned short)dx >= 0xc1) {
            o->timer = 0;
        } else if (o->timer++ >= 0x46) {
            o->timer = 10;
            o->state++;
        }
        break;
    case 3:
        FUN_80127214(o);
        if (--o->timer == -1) {
            o->step = 1;
            o->state = 1;
            o->substep = 2;
            o->b69 = 0;
            o->b68 = 0;
        }
        break;
    }
}
