// FUNC 8011c1b8 904 X004
// MATCHING 8011c1b8 904
#include "TOBJ.H"
typedef struct { short x, y; } P;
extern P D_80130CCC[];
extern short D_1F80016A[];
extern int ObjCullRegister(TObj *);
extern unsigned char FUN_8001e4f0(int);
extern void FUN_8001eaa4(int);
extern void FUN_8001fce4(TObj *);
extern void FUN_80018838(TObj *);

void func_8011C1B8(TObj *o)
{
    unsigned char b = o->b04;
    P *q;

    switch (b) {
    case 0:
        o->active = 1;
        o->box0 = 0x27;
        o->box1 = 0x4e;
        o->box2 = 8;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velH = 0;
        o->velV = 0;
        o->box3 = 0x10;
        o->b69 = 0;
        o->b6a = 0;
        if (D_1F80016A[0] > o->a.p.whole) {
            o->animFrame = 1;
            o->a.raw = D_80130CCC[1].x << 16;
            o->y.raw = D_80130CCC[o->animFrame].y << 16;
        } else {
            o->animFrame = 0;
        }
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (o->b69) {
                o->timer = 8;
                o->b6a = 0;
                o->step++;
                o->ba7 = FUN_8001e4f0(0x83);
            }
            break;
        case 1:
            if (--o->timer == -1) {
                if (o->animFrame == 0) {
                    o->velV = 0x80;
                    o->velH = 0x80;
                    o->step = 2;
                } else {
                    o->velV = -0x80;
                    o->velH = -0x80;
                    o->step = 3;
                }
                o->state = 0;
                FUN_8001fce4(o);
            }
            break;
        case 2:
            switch (o->state) {
            case 0:
                q = &D_80130CCC[1];
                if (o->a.p.whole < q->x) {
                    FUN_8001fce4(o);
                    break;
                }
                o->velV = 0;
                o->velH = 0;
                o->animFrame = 1;
                o->a.raw = q->x << 16;
                { short *s = &D_80130CCC[0].y; o->y.raw = s[o->animFrame * 2] << 16; }
                o->state++;
                o->b6a = 1;
                FUN_8001eaa4(o->ba7);
                FUN_8001e4f0(0x84);
                FUN_8001fce4(o);
                break;
            case 1:
                if (o->a.p.whole + o->box0 + 0x10 < D_1F80016A[0]) o->step = 0;
                break;
            }
            break;
        case 3:
            switch (o->state) {
            case 0:
                q = &D_80130CCC[0];
                if (o->a.p.whole > q->x) {
                    FUN_8001fce4(o);
                    break;
                }
                o->animFrame = 0;
                o->velV = 0;
                o->velH = 0;
                o->a.p.whole = q->x;
                { short *s = &q->y; o->y.p.whole = s[o->animFrame * 2]; }
                o->state++;
                o->b6a = 1;
                FUN_8001eaa4(o->ba7);
                FUN_8001e4f0(0x84);
                FUN_8001fce4(o);
                break;
            case 1:
                { int k = 0x10; if (D_1F80016A[0] < o->a.p.whole - k - o->box0) o->step = 0; }
                break;
            }
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
