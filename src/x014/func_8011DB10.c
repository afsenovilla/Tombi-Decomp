// FUNC 8011db10 332 X014
// MATCHING 8011db10 332
#include "TOBJ.H"
typedef struct { char p[4]; unsigned short t; char q[2]; } E8;
typedef struct {
    TObj o;
    char pc0[6];
    short wc6;
} T2;
extern unsigned short D_8009C962;
extern unsigned short D_1F80016A[], D_1F80016E;
extern unsigned char D_80125D80[];
extern void AnimAdvance(T2 *);
extern int Rand(void);

static __inline__ int check(T2 *o)
{
    if (D_8009C962 != 7) return 0;
    o->wc6 = 0;
    if ((unsigned short)(o->o.h->p.whole - D_1F80016A[0] + 0x40) > 0x80) return 0;
    if ((unsigned short)(o->o.y.p.whole - D_1F80016E + 0x40) > 0x80) return 0;
    if (D_80125D80[Rand() & 0xf]) o->wc6 = 1;
    else o->wc6 = 2;
    return 1;
}

void func_8011DB10(T2 *o)
{
    E8 *t;

    AnimAdvance(o);
    switch (o->o.state) {
    case 0:
        o->o.d8c = 0;
        t = (E8 *)o->o.d94;
        t += o->o.wae;
        o->o.timer = t->t;
        o->o.state++;
    case 1:
        if (--o->o.timer == -1) {
            o->o.step = 0;
            o->o.state = 0;
            o->o.substep = 0;
        }
        if (check(o)) {
            o->o.step = 1;
            o->o.state = 0;
            o->o.substep = 0;
        }
        break;
    }
}
