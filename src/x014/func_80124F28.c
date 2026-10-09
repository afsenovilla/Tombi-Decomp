// FUNC 80124f28 652 X014
// MATCHING 80124f28 652
#include "TOBJ.H"

typedef struct { short w0; unsigned short w2; } AH;

typedef struct B { unsigned short c[4]; } B;
extern B D_80126758[];
extern B D_80126728[];
extern unsigned short D_8009C962;
extern void playSFX(int);
extern int AnimAdvance(TObj *o);

void func_80124F28(TObj *o)
{
    unsigned int v;
    unsigned short *p;

    switch (o->step) {
    case 0:
        if (o->b0c) {
            p = D_80126758[0].c;
            o->box0 = *p++;
            o->box1 = *p++;
            o->box2 = *p++;
            o->box3 = *p;
        }
        if (D_8009C962 == 7) playSFX(0xf4);
        else playSFX(0xe4);
        o->step++;
    case 1:
        if (o->b0c) {
            o->visible = 0;
            o->y.p.whole--;
        } else {
            p = D_80126728[((AH *)o->anim)->w2].c;
            o->box0 = *p++;
            o->box1 = *p++;
            o->box2 = *p;
            o->box3 = p[1];
            o->visible = 1;
        }
        if (AnimAdvance(o)) {
            o->timer = 0x3c;
            o->step++;
        }
        break;
    case 2:
        if (o->b0c) {
            o->visible = 1;
            o->anim = 0;
            v = o->ba5 + 4;
            o->y.p.whole--;
            if (v >= 0x100) *(signed char *)&o->ba5 = -1;
            else o->ba5 = v;
            p = D_80126758[o->ba5 >> 4].c;
            o->box0 = *p++;
            o->box1 = *p++;
            o->box2 = *p;
            o->box3 = p[1];
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
