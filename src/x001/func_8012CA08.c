// FUNC 8012ca08 368 X001
// MATCHING 8012ca08 368
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern B D_8013C800[];
extern A *D_8013DAE8[];
extern char D_80077D0C[];
extern void FUN_8001fa88(TObj *, unsigned short);

#define SETANIM(k) \
    o->anim = D_8013DAE8[k]; \
    { unsigned char *p; { B *t = D_8013C800; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

void func_8012CA08(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b0b = 1;
        o->b0f = 4;
        o->active = 2;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->wac = 9;
        o->state++;
        SETANIM(9);
        break;
    case 1:
        FUN_8001fa88(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1)
        o->d8c = (o->d8c + 0x14) & 0xff;
    else
        o->d8c = (o->d8c - 0x14) & 0xff;
}
