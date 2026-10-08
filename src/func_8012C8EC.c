// FUNC 8012c8ec 504 X000
// MATCHING 8012c8ec 504
#include "TOBJ.H"
extern char DAT_80077cf4[];
extern unsigned short *DAT_8013a1c0[];
extern unsigned short *DAT_8013a1c4[];
extern unsigned char DAT_80138fd8[];
extern void FUN_80127214(TObj *);
extern int FUN_801274cc(TObj *);

static __inline__ void SetBox(TObj *o)
{
    unsigned char *p;
    p = &DAT_80138fd8[((unsigned short *)o->anim)[1] * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p++;
    o->box3 = *p;
    o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
}

void func_8012C8EC(TObj *o)
{
    unsigned char *p;
    switch (o->state) {
    case 0:
        o->movetab = DAT_80077cf4;
        o->velV = -0x200;
        o->b69 = 0;
        o->wac = 0x18;
        o->state++;
        o->anim = DAT_8013a1c0[0];
        SetBox(o);
        break;
    case 1:
        FUN_80127214(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV > 0)
            o->state++;
        break;
    case 2:
        FUN_80127214(o);
        o->velV += 0x30;
        if (o->velV > 0x800)
            o->velV = 0x800;
        o->y.raw += o->velV << 8;
        if (FUN_801274cc(o))
            o->state++;
        break;
    case 3:
        o->active = 1;
        o->timer = 0x3c;
        o->wac = 0x19;
        o->state++;
        o->anim = DAT_8013a1c4[0];
        SetBox(o);
        break;
    case 4:
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
