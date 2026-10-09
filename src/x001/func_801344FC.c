// FUNC 801344fc 376 X001
// MATCHING 801344fc 376
#include "TOBJ.H"
extern char D_80077D0C[];
extern void *D_8013F1FC;
extern TObj *D_8009C948;
extern void FUN_8001fa88(TObj *, unsigned short);

void func_801344FC(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        p = (TObj *)o->d90;
        o->state++;
        o->movetab = D_80077D0C;
        o->velV = -0x400;
        o->animFrame &= 1;
        o->anim = D_8013F1FC;
        o->b0a = 2;
        o->b0f -= 30;
        for (; p != 0; p = (TObj *)p->d90) {
            p->b04 = 3;
        }
        for (p = (TObj *)o->d94; p != 0; p = (TObj *)p->d94) {
            p->b04 = 3;
        }
        ((struct { char pad[0xa]; short n; } *)D_8009C948)->n--;
        break;
    case 1:
        FUN_8001fa88(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1) {
        o->d8c = (unsigned char)(o->d8c + 0x14);
    } else {
        o->d8c = (unsigned char)(o->d8c - 0x14);
    }
}
