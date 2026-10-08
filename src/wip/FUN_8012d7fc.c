// FUNC 8012d7fc 284 X000
#include "TOBJ.H"
extern unsigned char DAT_80138fd8[];
extern unsigned short *DAT_8013a214;
extern char DAT_80077cdc[];
extern void FUN_80127214(TObj *);

void FUN_8012d7fc(TObj *o)
{
    unsigned char *p;
    TObj *q;
    switch (o->state) {
    case 0:
        o->movetab = DAT_80077cdc;
        o->b69 = 0;
        o->wac = 0x2d;
        o->state = o->state + 1;
        o->anim = DAT_8013a214;
        p = DAT_80138fd8 + DAT_8013a214[1] * 4;
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 1:
        FUN_80127214(o);
        q = (TObj *)o->d94;
        o->h->p.whole = q->h->p.whole;
        o->y.p.whole = q->y.p.whole - 0x17;
        o->d8c = (-(q->d8c >> 4)) & 0xff;
        break;
    }
}
