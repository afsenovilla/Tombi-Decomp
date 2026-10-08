// FUNC 8012ff4c 456 X001
// MATCHING 8012ff4c 456
#include "TOBJ.H"
typedef struct { unsigned char b[8]; } C8;
extern C8 D_80116258;
extern void *D_8013E740, *D_8013E74C;
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);

void func_8012FF4C(TObj *o)
{
    C8 t = D_80116258;

    switch (o->state) {
    case 0:
        o->active = 2;
        o->state++;
    case 1:
        switch (t.b[Rand() & 7]) {
        case 0:
            o->anim = D_8013E740;
            AnimLoadDuration(o);
            o->timer = 0x1e;
            o->state = 2;
            break;
        case 1:
            o->anim = D_8013E74C;
            AnimLoadDuration(o);
            { short v; if ((o->animFrame = Rand() & 1) != 0) v = -0x200; else v = 0x200; o->timer = 0x78; o->velX = v; o->state = 3; }
            break;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        if ((unsigned short)(o->h->p.whole - 0x4d8) > 0xd0) {
            o->animFrame ^= 1;
            o->velX = -o->velX;
            o->h->raw += o->velX << 8;
        }
    case 2:
        if (--o->timer <= 0) o->state = 1;
        break;
    }
}
