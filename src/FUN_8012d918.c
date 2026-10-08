// FUNC 8012d918 228 X000
// MATCHING 8012d918 228
#include "tobj.h"
extern unsigned short *animp[];
extern unsigned char tab[];
extern void adv(void *), g(void *);
extern char mt[];
void FUN_8012d918(TObj *o)
{
    unsigned char *q;
    unsigned short *a;
    switch (o->state) {
    case 0:
        o->b69 = 1;
        o->movetab = mt;
        o->wac = 0x3a;
        o->state++;
        o->anim = a = animp[0];
        q = tab + a[1] * 4;
        o->box0 = *q++;
        o->box1 = *q++;
        o->box2 = *q;
        o->box3 = q[1];
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 1:
        adv(o);
        g(o);
    }
}
