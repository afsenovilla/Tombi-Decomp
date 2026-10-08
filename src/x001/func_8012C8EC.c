// FUNC 8012c8ec 284 X001
// MATCHING 8012c8ec 284
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern B D_8013C800[];
extern A *D_8013DAE8[];
extern int AnimAdvanceWithBox(TObj *);

#define SETANIM(k) \
    o->anim = D_8013DAE8[k]; \
    { unsigned char *p; { B *t = D_8013C800; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

void func_8012C8EC(TObj *o)
{
    switch (o->substep) {
    case 0:
        o->timer = 0x5a;
        o->b9d = 0;
        o->wac = 1;
        o->substep++;
        SETANIM(1);
        break;
    case 1:
        if (--o->timer == 0) o->substep++;
        AnimAdvanceWithBox(o);
        break;
    case 2:
        o->state = 8;
        o->substep = 0;
        break;
    }
}
