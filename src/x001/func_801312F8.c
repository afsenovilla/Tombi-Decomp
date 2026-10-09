// FUNC 801312f8 740 X001
// MATCHING 801312f8 740
#include "TOBJ.H"

extern int D_1F8002D4[];
extern short D_1F80027E;
extern void *D_8013E740[];
extern TObj D_800A6038;
extern unsigned char D_8009CE59;
void AnimLoadDuration(TObj *o);
int ObjCullRegister(TObj *o);
short TileCollideAt(TObj *o, int x, int y);
void FUN_80018790(TObj *o);
void func_80130298(TObj *o);
void func_80130420(TObj *o);
void func_801305D0(TObj *o);
void func_80130DC4(TObj *o);
void func_80130F4C(TObj *o);
void func_8013109C(TObj *o);
void func_8012FF4C(TObj *o);
void func_80130114(TObj *o);

void func_801312F8(TObj *o)
{
    int d;

    switch (o->b04) {
    case 0:
        d = D_1F8002D4[0];
        o->w1e = 0x12;
        o->b0d = 1;
        o->w08 = 0x7813;
        o->d3c = d;
        o->b0f = D_800A6038.b0f + 1;
        o->b68 = 0;
        o->wb6 = 0;
        o->wb4 = 0;
        o->d8c = 0;
        o->d84 = 0;
        o->box0 = 0x18;
        o->box1 = 0x30;
        o->box2 = 0x18;
        o->box3 = 0x30;
        o->b04++;
        o->wac = 0;
        o->anim = D_8013E740[0];
        AnimLoadDuration(o);
        switch (D_8009CE59) {
        case 1:
        case 3:
            o->step = 0;
            break;
        case 2:
            o->step = 1;
            break;
        }
        break;
    case 1:
        if (o->subtype) {
            ObjCullRegister(o);
            switch (o->step) {
            case 0:
                func_80130298(o);
                break;
            case 1:
                func_80130420(o);
                break;
            case 2:
                func_801305D0(o);
                break;
            case 3:
                func_80130DC4(o);
                break;
            case 4:
                func_80130F4C(o);
                break;
            case 5:
                func_8013109C(o);
                break;
            }
        } else {
            ObjCullRegister(o);
            switch (o->step) {
            case 0:
                if (o->state == 0) {
                    o->anim = D_8013E740[0];
                    AnimLoadDuration(o);
                    o->y.p.whole += 4;
                    if (TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) {
                        o->d84 = (-D_1F80027E << 2) & 0xff;
                    }
                    o->state++;
                }
                break;
            case 1:
                func_8012FF4C(o);
                break;
            case 3:
                func_80130114(o);
                break;
            }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
