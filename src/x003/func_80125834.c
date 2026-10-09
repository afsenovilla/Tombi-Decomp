// FUNC 80125834 376 X003
// MATCHING 80125834 376
#include "TOBJ.H"
extern unsigned short D_1F80027E;
extern short FUN_80040278(TObj *, int, int);
extern void AnimLoadDuration(TObj *);
extern void *D_80139578;
extern unsigned char D_80135CB0[];

void func_80125834(TObj *o)
{
    unsigned char *p;
    short yy;
    unsigned int v;

    switch (o->substep) {
    case 0:
        o->w98 = 3;
        o->wac = 0x1e;
        o->wb4 = 0;
        o->wb6 = 0;
        o->substep++;
        o->anim = D_80139578;
        p = &D_80135CB0[o->wac * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        AnimLoadDuration(o);
        break;
    case 1:
        yy = o->y.p.whole;
        o->y.p.whole = yy + 1;
        if (o->b69 == 1) {
            o->b69 = 0;
        } else if (FUN_80040278(o, o->h->p.whole, (short)(yy + 0xf))) {
            if (o->wac >= 0xc) {
                o->d8c = 0;
            } else {
                v = ((D_1F80027E << 2) + o->d8c) & 0xff;
                if (v != 0) {
                    if (v < 0x80) o->d8c = o->d8c - 1;
                    else o->d8c = o->d8c + 1;
                    o->d8c = *(unsigned char *)&o->d8c;
                }
            }
        }
        if (o->visible) {
            o->state = 2;
            o->substep = 0;
        }
        break;
    }
}
