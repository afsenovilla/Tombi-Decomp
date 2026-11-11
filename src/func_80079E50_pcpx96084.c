// VERSION PCPX_96084 tomba 80079E50 748
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of updateEgg (src/updateEgg.c): halfword at +0x0A, scratchpad word 0x1F80015C, the hatched chick placed at the egg.
#include "TOBJ.H"
#include "raw7.h"
extern void func_80018550(TObj *);
extern int isObjectInView(TObj *);
extern void AnimLoadDuration(TObj *);
extern void freeObjectLayer7(TObj *);
extern TObj *func_8001A36C_pcpx96084(void);
extern TObj *func_8001A2F0_pcpx96084(void);
extern void *D_80013F1C;
extern int D_1F80015C;
extern unsigned char D_800DE4AA[];
extern int PLAYER_POS, DAT_800a604c, DAT_800a6050;

typedef struct { int a, b, c; } V3;

void func_80079E50_pcpx96084(TObj *o)
{
    TObj *n;
    unsigned char d;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->box0 = 0x14;
            o->box1 = 0x28;
            o->box2 = 0x18;
            S16(o, 0x72) = 0x30;
            o->d3c = D_1F80015C;
            o->step++;
            o->w1e = 10;
            o->b0d = 0;
            *(short *)&o->b0a = 0;
            o->b69 = 0;
            o->b68 = 0;
            o->b0f = 0;
            if ((D_800DE4AA[0] >> o->subtype) & 1) {
                n = func_8001A36C_pcpx96084();
                if (n != 0) {
                    n->active = 1;
                    n->type = 3;
                    n->subtype = o->subtype;
                    n->b0c = o->b0c;
                    *(V3 *)&n->a = *(V3 *)&o->a;
                    n->b6b = o->b6b;
                    n->b1d = o->b1d;
                }
                o->b04 = 3;
            }
            break;
        case 1:
            if (isObjectInView(o)) {
                o->step = 0;
                o->state = 0;
                o->substep = 0;
                o->b04++;
            }
            break;
        }
        break;
    case 1:
        func_80018550(o);
        if (o->visible) {
            if (o->step == 0) {
                if (o->state == 0) {
                    o->b69 = 0;
                    o->anim = D_80013F1C;
                    AnimLoadDuration(o);
                    o->state++;
                }
            }
        }
        break;
    case 2:
        func_80018550(o);
        if (o->visible) {
            switch (o->step) {
            case 0:
                o->anim = D_80013F1C;
                AnimLoadDuration(o);
                o->step = 1;
                break;
            case 1:
                break;
            case 2:
                n = func_8001A2F0_pcpx96084();
                if (n != 0) {
                    n->active = 1;
                    n->type = 2;
                    n->animFrame = (o->animFrame & 1) | 2;
                    *(V3 *)&n->a = *(V3 *)&o->a;
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
        freeObjectLayer7(o);
        break;
    }
}
#endif
