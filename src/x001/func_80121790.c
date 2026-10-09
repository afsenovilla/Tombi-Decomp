// FUNC 80121790 640 X001
// MATCHING 80121790 640
#include "TOBJ.H"

typedef struct { TObj o; unsigned char pc0[6]; unsigned char c6, c7; unsigned char pc8[0x1b]; unsigned char e3; } TX;
#define X(o) ((TX *)(o))
extern TObj *D_8009C330;
extern void AnimAdvance(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_800eea3c(TObj *);
extern void FUN_800ee428(TObj *);
extern void FUN_800f00ac(TObj *);

static __inline__ short inc(unsigned char x)
{
    if (x < 0xff) return x + 1;
    return x;
}

void func_80121790(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C330->b0b = o->animFrame;
        if (o->animFrame &= 1) {
            if (o->wb2 > 0) {
                o->wb2 = -o->wb2;
            }
        } else {
            if (o->wb2 < 0) {
                o->wb2 = -o->wb2;
            }
        }
        *(unsigned char *)&o->wac = 0;
        o->b9c = 0;
        o->b6b = 0;
        X(o)->c7 = 1;
        o->b9d = 0;
        X(o)->c6 = 0;
        X(o)->e3 = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0x100;
        D_8009C330->active = 0;
        o->ba4 = 1;
        if ((o->bbe & 1) != o->animFrame) {
            FUN_800eeb5c(o, 8);
        } else {
            FUN_800eeb5c(o, 0x11);
        }
        o->state++;
    case 1:
        o->b6b = inc(o->b6b);
        AnimAdvance(o);
        FUN_800eea3c(o);
        o->y.raw += o->velY << 8;
        o->velY += 8;
        switch (D_8009C330->animTimer) {
        case 8:
            o->d8c = (o->animFrame & 1) ? 0x20 : 0xe0;
            break;
        case 0x11:
            o->d8c = (o->animFrame & 1) ? 0x100 - (o->b6b >> 2) : o->b6b >> 2;
            break;
        }
        if (o->bbe == 0) {
            if (o->animFrame != (o->bbe & 1)) {
                o->wb2 = 0;
            }
            o->ba4 = 0;
            if (D_8009C330->active == 0) {
                *(unsigned char *)&o->wac = 1;
                FUN_800ee428(o);
                o->step = 2;
            } else {
                FUN_800ee428(o);
                o->step = 4;
            }
            o->state = 3;
        }
        if (o->bbe & 2) {
            o->step = 0x13;
            o->state = 0;
        }
        FUN_800f00ac(o);
        break;
    }
}
