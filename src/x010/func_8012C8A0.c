// FUNC 8012c8a0 688 X010
// MATCHING 8012c8a0 688
#include "TOBJ.H"
typedef struct { short x, y, w, h; } RECT;

extern unsigned char D_8009CDE3;
extern void *D_801322E8[], *D_801322F4[];
extern int D_1F8002D0[];
extern short D_8007A3F0[];
extern void MoveImage(RECT *, int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);
extern void func_8012B8C0(TObj *);
extern void func_8012BA44(TObj *);
extern void func_8012BDE4(TObj *);
extern void func_8012BF24(TObj *);
extern void func_8012C0DC(TObj *);
extern void func_8012C220(TObj *);
extern void func_8012C5AC(TObj *);
extern void func_8012C6EC(TObj *);

void func_8012C8A0(TObj *o)
{
    RECT r;

    switch (o->b04) {
    case 0:
        o->b6b = 0;
        if (D_8009CDE3 == 0xff) {
            r.x = 0x120;
            r.y = 0x1e4;
            r.w = 0x10;
            r.h = 4;
            MoveImage(&r, 0x120, 0x1e0);
            o->b6b = 5;
        }
        o->box0 = 4;
        o->box1 = 8;
        o->box2 = 6;
        o->box3 = 0xc;
        *(signed char *)&o->b0f = -6;
        o->w1e = 3;
        o->animFrame = 0;
        o->b0d = 0;
        o->wac = 2;
        o->b04++;
        o->anim = D_801322E8[0];
        o->d3c = D_1F8002D0[0];
        AnimLoadDuration(o);
        break;
    case 1:
        func_8012B8C0(o);
        ObjCullRegister(o);
        AnimAdvance(o);
        switch (o->step) {
        case 0:
            switch (o->state) {
            case 0:
                o->active = 5;
                o->velV = 0x100;
                o->d88 = 0;
                o->wac = 5;
                o->anim = D_801322F4[0];
                AnimLoadDuration(o);
                o->state++;
                break;
            case 1:
                AnimAdvance(o);
                o->y.raw += (D_8007A3F0[o->d88] * o->velV) >> 4;
                o->d88 = (o->d88 + 2) & 0xff;
                break;
            }
            break;
        case 1:
            func_8012BA44(o);
            break;
        case 2:
            func_8012BDE4(o);
            break;
        case 3:
            func_8012BF24(o);
            break;
        case 4:
            func_8012C0DC(o);
            break;
        case 5:
            func_8012C220(o);
            break;
        case 6:
            func_8012C5AC(o);
            break;
        }
        break;
    case 2:
        ObjCullRegister(o);
        func_8012C6EC(o);
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
