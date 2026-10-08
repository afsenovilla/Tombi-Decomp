// FUNC 80116928 152 X002
// MATCHING 80116928 152
#include "TOBJ.H"
typedef struct { void **anims; int x; } AT;
extern AT D_8011C6DC[];
extern void AnimJump(TObj *, short);

static __inline__ void setanim(TObj *o, void *p, short b)
{
    o->anim = p;
    AnimJump(o, b);
}

void func_80116928(TObj *o, unsigned short a, short b)
{
    if (o->subtype == 1) {
        if (a == 4) {
            o->w1e = 5;
            setanim(o, D_8011C6DC[9].anims[0], b);
        } else {
            o->w1e = 2;
            setanim(o, D_8011C6DC[1].anims[(short)a], b);
        }
    } else {
        setanim(o, D_8011C6DC[o->subtype].anims[(short)a], b);
    }
}
