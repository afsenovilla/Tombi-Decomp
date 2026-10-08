// FUNC 8012230c 368 X004
// MATCHING 8012230c 368
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern char D_80077D0C[];
extern B D_80130FD4[];
extern A *D_80133BF4[];
extern void FUN_8001fa88(TObj *, unsigned short);

void func_8012230C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b0b = 1;
        o->b0f = 4;
        o->active = 2;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->wac = 0x26;
        o->state++;
        o->anim = D_80133BF4[0];
        { unsigned char *p; { B *t = D_80130FD4; p = t[((A *)o->anim)->w2].c; }
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1]; }
        o->animTimer = ((A *)o->anim)->w6 & 0x3fff;
        break;
    case 1:
        FUN_8001fa88(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1)
        o->d8c = (o->d8c + 0x14) & 0xff;
    else
        o->d8c = (o->d8c - 0x14) & 0xff;
}
