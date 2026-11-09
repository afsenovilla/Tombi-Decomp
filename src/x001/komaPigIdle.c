// FUNC 8012c8ec 284 X001
// MATCHING 8012c8ec 284
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern B KOMA_PIG_ANIM_BOXES[];
extern A *KOMA_PIG_ANIMS[];
extern int AnimAdvanceWithBox(TObj *);

#define SETANIM(k) \
    o->anim = KOMA_PIG_ANIMS[k]; \
    { unsigned char *p; { B *t = KOMA_PIG_ANIM_BOXES; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

void komaPigIdle(TObj *o)
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
