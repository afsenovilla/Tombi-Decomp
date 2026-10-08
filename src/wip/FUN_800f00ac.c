// FUNC 800f00ac 712 X000
#include "TOBJ.H"
#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
extern int FUN_800eff1c(TObj *);
extern void FUN_8003fd78(TObj *, int, int);
extern TObj *DAT_80096330;
extern int DAT_8009c984;
extern unsigned short DAT_1f8003c4;
extern unsigned short DAT_8009d670[];
extern unsigned char DAT_8009d2b0[];

void FUN_800f00ac(TObj *o)
{
    int v;

    B(o, 0xad) = 0;
    if (B(o, 0xab) != 2) {
        switch (FUN_800eff1c(o)) {
        case 1:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            if ((DAT_8009c984 & 0x40) && (DAT_1f8003c4 & DAT_8009d670[0])) o->ba7 = 1;
            B(DAT_80096330, 8) = o->animFrame & 1;
            o->step = 0x10;
            o->state = 0;
            break;
        case 2:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            B(DAT_80096330, 8) = o->animFrame & 1;
            o->step = 0x17;
            o->state = 0;
            break;
        case 3:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            B(DAT_80096330, 8) = o->animFrame & 1;
            o->step = 0x1b;
            o->state = 0;
            break;
        case 4:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            B(DAT_80096330, 8) = o->animFrame & 1;
            if ((DAT_8009c984 & 0x40) && (DAT_1f8003c4 & DAT_8009d670[0])) o->ba7 = 1;
            o->step = 0x1e;
            o->state = 0;
            break;
        case 5:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            B(DAT_80096330, 8) = o->animFrame & 1;
            o->step = 0x21;
            o->state = 0;
            break;
        }
    }
    FUN_8003fd78(o, 0, 0);
    if ((o->b69 | o->b9c | o->b9e | o->b9f | o->bbe) == 0) {
        B(o, 0xad) = 1;
        o->velY += 0x223;
        o->y.raw += o->velY << 8;
        DAT_80096330->timer = 0x21;
        if (o->velY >= 0x447) {
            B(o, 0xad) = 0;
            o->velY = 0;
            o->b9c = 2;
            B(o, 0xac) = 1;
            B(o, 0xc3) = 0;
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            DAT_8009d2b0[0] = 0;
            v = 0x10;
            if (o->step == 3) {
                o->timer = 10;
                o->d84 = 0;
                if (o->animFrame & 1) v = 0xf0;
                o->d88 = v;
                o->d8c = 0;
                o->step = 4;
                o->state = 1;
            } else {
                o->timer = 10;
                o->d84 = 0;
                if (o->animFrame & 1) v = 0xf0;
                o->d88 = v;
                o->d8c = 0;
                o->step = 2;
                o->state = 3;
            }
        }
    } else {
        o->velY = 0;
        o->b9c = 0;
    }
}
