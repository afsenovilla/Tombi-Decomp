// FUNC 800ef1ac 476 X019
// MATCHING 800ef1ac 476
#include "TOBJ.H"
#include "raw7.h"
extern TObj *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern unsigned char DAT_801152e8[];
extern void ObjSetAnimFromTable(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001e5f4(int, int);
extern short FUN_8003fd78(TObj *, int, int);

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    TObj *p = DAT_8009c330;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable(o);
        FUN_8001fe94(o, 0);
        DAT_8009c330->animFrame = DAT_8009c330->animTimer;
    }
}

void FUN_800ef1ac(TObj *o)
{
    if (o->b69) {
        (*(unsigned char*)&o->waa) = 0;
        o->b9e = 0;
        o->ba7 = 0;
        (*(unsigned char*)&o->wac) = 0;
        o->b9c = 0;
        o->d8c = DAT_801152e8[o->wb0];
        if (o->bbe & 8) {
            (*(unsigned char*)&DAT_8009c330->w08) = (*(unsigned char*)&o->animFrame) & 1;
            if ((o->bbe & 1) != o->animFrame) {
                setanim(o, 8);
                FUN_8001fe94(o, 2);
            } else {
                setanim(o, 0x11);
            }
            o->step = 0x1b;
        } else {
            FUN_8001e5f4(0x1c, 0x7f);
            (*(unsigned char*)&DAT_8009c330->w08) = 0;
            o->step = 0;
        }
        o->state = 0;
    }
    if (DAT_8009f0ec && DAT_8009f0ec->type == 0x14 && FUN_8003fd78(o, 0, 0)) {
        FUN_8001e5f4(0x1c, 0x7f);
        (*(unsigned char*)&DAT_8009c330->w08) = 0;
        o->ba7 = 0;
        (*(unsigned char*)&o->wac) = 0;
        o->b9c = 0;
        o->b9e = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->step = 0;
        o->state = 0;
    }
}
