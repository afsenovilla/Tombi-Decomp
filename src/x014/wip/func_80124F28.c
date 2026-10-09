// FUNC 80124f28 652 X014
/* score 72: logic/layout match; in the three box copies (*t++ from D_80126758/D_80126728) the game keeps the
   pointer in v1 and the values in v0, ours swaps them (and in case 1 the visible=1 store schedules differently).
   Tried: block/function-scope pointers (one or three), ushort types, value temps, setbox inline, t[1] form. */
#include "TOBJ.H"

typedef struct { short w0; unsigned short w2; } AH;

extern short D_80126758[];
extern short D_80126728[];
extern unsigned short D_8009C962;
extern void playSFX(int);
extern int AnimAdvance(TObj *o);

void func_80124F28(TObj *o)
{
    unsigned int v;

    switch (o->step) {
    case 0:
        if (o->b0c) {
            short *t = D_80126758;
            o->box0 = *t++;
            o->box1 = *t++;
            o->box2 = *t++;
            o->box3 = *t;
        }
        if (D_8009C962 == 7) playSFX(0xf4);
        else playSFX(0xe4);
        o->step++;
    case 1:
        if (o->b0c) {
            o->visible = 0;
            o->y.p.whole--;
        } else {
            short *t = &D_80126728[((AH *)o->anim)->w2 * 4];
            o->box0 = *t++;
            o->box1 = *t++;
            o->box2 = *t++;
            o->visible = 1;
            o->box3 = *t;
        }
        if (AnimAdvance(o)) {
            o->timer = 0x3c;
            o->step++;
        }
        break;
    case 2:
        if (o->b0c) {
            short *t;
            o->visible = 1;
            o->y.p.whole--;
            o->anim = 0;
            v = o->ba5 + 4;
            if (v >= 0x100) *(signed char *)&o->ba5 = -1;
            else o->ba5 = v;
            t = &D_80126758[(o->ba5 >> 4) * 4];
            o->box0 = *t++;
            o->box1 = *t++;
            o->box2 = *t++;
            o->box3 = *t;
        } else {
            o->active = 2;
            o->visible = 1;
            o->velX -= 2;
            if (o->velX <= 0) o->velX = 0;
            v = o->ba5 + 4;
            if (v >= 0x100) *(signed char *)&o->ba5 = -1;
            else o->ba5 = v;
            o->ba6 = o->ba5;
        }
        if (--o->timer == -1) {
            o->active = 2;
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
