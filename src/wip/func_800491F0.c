// FUNC 800491f0 820 MAIN0
/* score 18: only regs in the case-32 box test. Game: o->h ptr v0, e->h ptr a0, o-val v1, e-val a0, box v0, d=v1, sum=box+d in v0 (o-ptr NOT tied to o-val). `d = o->h; d -= e->h;` 22->18. b31 tried: d/s types int/short/ushort, s=box; s+=d (right addu order, regs wrong, 38), temps for e-val, -(e-o), inline chk() variants (22-88), direct expr forms (36). Outer loop must be goto (no loop.c hoisting). b46: -dl dump: function-scope d is a global pseudo (dies in 2 places) so o-val can't tie with d; block-local d1 per test + `e->box0 + d1` gives the right shape (box+d, d tied to o-val) but o-val qty wins local-alloc priority over the box/sum qty (looks like an exact tie broken by qty order) -> 36. Tried: lim=e->box1 read first, sibling form t = e->box0 + (a - b), oh/eh pointer locals, int/ushort d1, separate e-val temp (all 18-38). */
#include "TOBJ.H"

extern short D_1F80019E;
extern unsigned char D_8007B63C[];
extern unsigned short D_8009C960;
void func_800489D8(TObj *o);
void func_80126B50(TObj *o);
void func_80126C60(TObj *o);
void func_80126CA4(TObj *o);
void func_8012032C(TObj *o);
void func_80126D7C(TObj *o);
void func_80126D48(TObj *o);
void func_801280DC(TObj *o);
void func_80127FDC(TObj *o);
void func_801216A0(TObj *o);
void func_8011FAE8(TObj *o);
void func_8011FA74(TObj *o);
void func_80128054(TObj *o);
void func_801203D0(TObj *o);
void func_8012788C(TObj *o);
void func_801210B4(TObj *o);
void func_80121A40(TObj *o);
void func_80121994(TObj *o);
void func_8011D4FC(TObj *o);

void func_800491F0(void)
{
    short n;
    TObj **list;
    TObj **l2;
    TObj *o;
    TObj *e;
    short d;
    n = *(unsigned short *)0x1F800246;
    list = *(TObj ***)0x1F80021C;
    if (*(short *)0x1F80024C == 0) return;
    if (n == 0) return;
outer:
        o = *list++;
        n--;
        if (o->active != 2 && D_8007B63C[o->type] != 0) {
        l2 = *(TObj ***)0x1F800224;
        if ((D_1F80019E = *(unsigned short *)0x1F80024C) != 0)
        do {
            e = *l2++;
            D_1F80019E--;
            if (!(e->active & 1)) continue;
            switch (e->type) {
            case 0: func_800489D8(o); break;
            case 1: func_80126B50(o); break;
            case 2: func_80126C60(o); break;
            case 4:
                switch (D_8009C960) {
                case 0: func_80126CA4(o); break;
                case 3: func_8012032C(o); break;
                }
                break;
            case 6: func_80126D7C(o); break;
            case 10: func_80126D48(o); break;
            case 15: func_801280DC(o); break;
            case 20: func_80127FDC(o); break;
            case 28: func_801216A0(o); break;
            case 32:
                if (o->type == 0x22 || o->type == 0x2c) {
                    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90) break;
                    d = o->h->p.whole; d -= e->h->p.whole;
                    if ((unsigned short)(d + e->box0) > e->box1) break;
                    d = o->y.p.whole; d -= e->y.p.whole;
                    if ((unsigned short)(d + e->box2) > e->box3) break;
                    o->active = 2;
                    o->b04 = 2;
                    o->step = 0;
                    o->state = 0;
                    e->state++;
                }
                break;
            case 33: func_80128054(o); break;
            case 35: func_8012788C(o); break;
            case 40: func_801210B4(o); break;
            case 31: func_8011FA74(o); break;
            case 34: func_801203D0(o); break;
            case 66:
            case 67: func_8011D4FC(o); break;

            case 52: func_80121994(o); break;
            case 51: func_80121A40(o); break;
            case 30: func_8011FAE8(o); break;
            }
        } while (D_1F80019E != 0);
        }
    if (n != 0) goto outer;
}
