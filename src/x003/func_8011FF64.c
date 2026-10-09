// FUNC 8011ff64 476 X003
// MATCHING 8011ff64 476
#include "TOBJ.H"

extern short D_1F80019E;
extern short func_8004461C(TObj *a, TObj *b);
extern int FUN_80042b98(TObj *, TObj *);
extern int FUN_80042d5c(TObj *, TObj *);
extern void FUN_800e9f74(int, int, int, int);

void func_8011FF64(TObj *o, TObj *p)
{
    int r;

    if (func_8004461C(o, p) == -1) return;
    p->w7a = o->animFrame & 1;
    if (*(unsigned short *)&p->wb4) {
        if (*(unsigned short *)&p->wb6) {
            r = FUN_80042b98(o, p);
            if (r) {
                if (r == 2) p->b68 = 5;
                else if ((o->type == 0 || o->type == 5) && o->w98 == 1) p->b68 = 1;
                else p->b68 = 2;
                p->active = 3;
                p->b04 = 2;
                p->step = 0;
                p->state = 0;
            }
        } else {
            r = FUN_80042b98(o, p);
            if (r) {
                if (r == 1) {
                    p->active = 3;
                    p->b04 = 2;
                    p->step = 0;
                    p->state = 0;
                } else {
                    FUN_800e9f74(0x1f4, p->a.p.whole, p->y.p.whole, p->b.p.whole);
                    p->active = 2;
                    {unsigned short u = o->animFrame;
                    p->b04 = 2;
                    p->step = 2;
                    p->state = 0;
                    p->animFrame = 1 - (u & 1);}
                }
            }
        }
    } else if (FUN_80042d5c(o, p) && p->w98 == 0 && p->active != 5) {
        if (p->subtype == 3) {
            FUN_800e9f74(0x1f4, p->a.p.whole, p->y.p.whole, p->b.p.whole);
            p->active = 2;
            {unsigned short u = o->animFrame;
            p->b04 = 2;
            p->step = 2;
            p->state = 0;
            p->animFrame = ~u & 1;}
        } else {
            p->active = 3;
            p->b68 = 0;
            p->b04 = 2;
            p->step = 0;
            p->state = 0;
        }
    }
    D_1F80019E = 0;
}
