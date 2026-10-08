// FUNC 800f0374 220 X016
// MATCHING 800f0374 220
#include "TOBJ.H"
#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
extern void FUN_8003fd78(TObj *, int, int);
extern TObj *DAT_8009c330;

int FUN_800f0374(TObj *o)
{
    B(o, 0xad) = 0;
    FUN_8003fd78(o, 0, 0);
    if ((o->b69 | o->b9c | o->b9e | o->b9f | o->bbe) == 0) {
        B(o, 0xad) = 1;
        o->velY += 0x223;
        o->y.raw += o->velY << 8;
        DAT_8009c330->timer = 0x21;
        if (o->velY >= 0x447) {
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            B(o, 0xad) = 0;
            o->velY = 0;
            o->b9c = 2;
            B(o, 0xc3) = 0;
            o->d8c = 0;
            return 1;
        }
    } else {
        o->velY = 0;
        o->b9c = 0;
    }
    return 0;
}
