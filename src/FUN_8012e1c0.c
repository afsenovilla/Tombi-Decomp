// FUNC 8012e1c0 772 X000
// MATCHING 8012e1c0 772
#include "TOBJ.H"
#include "raw7.h"
extern void FUN_800202b4(TObj *);
extern int FUN_800203dc(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_80018790(TObj *);
extern TObj *FUN_800183b8(void);
extern TObj *FUN_800182ac(void);
extern void *PTR_DAT_8013b104;
extern int DAT_1f8002d4;
extern unsigned char DAT_8009d2ae[];
extern int DAT_800a6048, DAT_800a604c, DAT_800a6050;

typedef struct { int a, b, c; } V3;

void FUN_8012e1c0(TObj *o)
{
    TObj *n;
    unsigned char d;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            S16(o, 0x6c) = 0x14;
            S16(o, 0x6e) = 0x28;
            S16(o, 0x70) = 0x18;
            S16(o, 0x72) = 0x30;
            o->d3c = DAT_1f8002d4;
            o->step++;
            o->w1e = 10;
            o->b0d = 0;
            o->b0a = 0;
            o->b69 = 0;
            o->b68 = 0;
            o->b0f = 0;
            if ((DAT_8009d2ae[0] >> o->subtype) & 1) {
                n = FUN_800183b8();
                if (n != 0) {
                    n->active = 1;
                    n->type = 3;
                    n->subtype = o->subtype;
                    n->b0c = o->b0c;
                    *(V3 *)&n->a = *(V3 *)&o->a;
                    n->b6b = o->b6b;
                    d = o->b1d;
                    n->w7a = 0;
                    n->b1d = d;
                }
                o->b04 = 3;
            }
            break;
        case 1:
            if (FUN_800203dc(o)) {
                o->step = 0;
                o->state = 0;
                o->substep = 0;
                o->b04++;
            }
            break;
        }
        break;
    case 1:
        FUN_800202b4(o);
        if (o->visible) {
            if (o->step == 0) {
                if (o->state == 0) {
                    o->b69 = 0;
                    o->anim = PTR_DAT_8013b104;
                    FUN_8001fe6c(o);
                    o->state++;
                }
            }
        }
        break;
    case 2:
        FUN_800202b4(o);
        if (o->visible) {
            switch (o->step) {
            case 0:
                o->anim = PTR_DAT_8013b104;
                FUN_8001fe6c(o);
                o->step = 1;
                break;
            case 1:
                break;
            case 2:
                n = FUN_800182ac();
                if (n != 0) {
                    n->active = 1;
                    n->type = 2;
                    n->animFrame = (o->animFrame & 1) | 2;
                    *(V3 *)&n->a = *(V3 *)&DAT_800a6048;
                    n->subtype = o->subtype;
                    n->b0c = o->b0c;
                    n->b6b = o->b6b;
                    n->b1d = o->b1d;
                }
                o->b04 = 3;
                break;
            }
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
