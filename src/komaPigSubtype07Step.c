// FUNC 8012d7fc 284 X000
// MATCHING 8012d7fc 284
#include "TOBJ.H"
#include "raw7.h"
extern unsigned char KOMA_PIG_ANIM_BOXES[];
extern unsigned short *DAT_8013a214;
extern char MOVE_TABLE_0[];
extern void FUN_80127214(TObj *);

void komaPigSubtype07Step(TObj *o)
{
    unsigned char *p;
    TObj *q;
    switch (o->state) {
    case 0:
        PTR(o, 0x28) = MOVE_TABLE_0;
        o->b69 = 0;
        S16(o, 0xac) = 0x2d;
        o->state = o->state + 1;
        o->anim = DAT_8013a214;
        p = KOMA_PIG_ANIM_BOXES + DAT_8013a214[1] * 4;
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
