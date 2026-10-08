// FUNC 80109920 152 X013
// MATCHING 80109920 152
#include "TOBJ.H"
extern char *DAT_8009c330;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001fec0(TObj *);

void FUN_80109920(TObj *o)
{
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->wb0 = 0;
        DAT_8009c330[8] = 0;
        o->b69 = 0;
        o->velY = 0;
        o->velV = 0;
        *(char *)&o->wac = 0;
        o->animFrame = o->animFrame & 1;
        FUN_800eeb5c(o, 0x1e);
        o->state++;
    case 1:
        FUN_8001fec0(o);
    }
}
