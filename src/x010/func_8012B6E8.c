// FUNC 8012b6e8 472 X010
// MATCHING 8012b6e8 472
#include "TOBJ.H"

extern unsigned char D_8012F460, D_8012F461, D_8012F462, D_8012F463;
extern int D_1F8002D0;
extern void *D_8013239C[];
extern char D_80077D0C[];
extern void AnimLoadDuration(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void FUN_8001faf4(TObj *o);
extern void ObjFree(TObj *o);

void func_8012B6E8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 1;
        o->b0d = 0;
        o->b0a = 2;
        *(int *)((char *)o + 0x8c) = 0;
        o->wac = 0x23;
        o->d3c = D_1F8002D0;
        o->box0 = D_8012F460;
        o->box1 = D_8012F461;
        o->box2 = D_8012F462;
        o->box3 = D_8012F463;
        o->anim = D_8013239C[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (ObjCullRegister(o)) {
            switch (o->step) {
            case 0:
                o->b0b = 1;
                o->b0f = 4;
                o->velV = -0x400;
                o->movetab = D_80077D0C;
                o->step++;
                break;
            case 1:
                FUN_8001faf4(o);
                o->velV += 0x40;
                if (o->velV > 0x400) o->velV = 0x400;
                o->y.raw += o->velV << 8;
                break;
            }
            if (o->animFrame & 1) o->d8c = (o->d8c + 0x14) & 0xff;
            else o->d8c = (o->d8c - 0x14) & 0xff;
        } else {
            o->b04 = 3;
        }
        break;
    case 2:
    case 3:
        ObjFree(o);
        break;
    }
}
