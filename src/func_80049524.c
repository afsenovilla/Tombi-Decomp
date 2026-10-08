// FUNC 80049524 824 MAIN0
// MATCHING 80049524 824
#include "TOBJ.H"

extern unsigned short D_8009C960;
extern unsigned short D_1F800248;
extern short D_1F80024C;
extern TObj **D_1F800228;
extern TObj **D_1F800224;
extern short D_1F80019E;
void func_800489D8(TObj *o, TObj *e);
void func_80126B50(TObj *o, TObj *e);
void func_80126C60(TObj *o, TObj *e);
void func_80126D04(TObj *o, TObj *e);
void func_8012038C(TObj *o, TObj *e);
void func_80126D7C(TObj *o, TObj *e);
void func_801280DC(TObj *o, TObj *e);
void func_80127FDC(TObj *o, TObj *e);
void func_801216A0(TObj *o, TObj *e);
void func_8011FAE8(TObj *o, TObj *e);
void func_8011FA74(TObj *o, TObj *e);
void func_801203D0(TObj *o, TObj *e);
void func_8012788C(TObj *o, TObj *e);
void func_801210B4(TObj *o, TObj *e);
void func_80121A40(TObj *o, TObj *e);
void func_80121994(TObj *o, TObj *e);

static __inline__ short hit(TObj *o, TObj *p)
{
    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    if ((unsigned short)((o->h->p.whole - p->h->p.whole) + (p->box0 + (o->box1 - o->box0))) > p->box1 + o->box1)
        return 0;
    if ((unsigned short)((o->y.p.whole - p->y.p.whole) + (p->box2 + (o->box3 - o->box2))) > o->box3 + p->box3)
        return 0;
    return 1;
}

void func_80049524(void)
{
    short n;
    TObj **pp, **qq;
    TObj *o, *e;

    n = D_1F800248;
    pp = D_1F800228;
    if (D_1F80024C == 0)
        return;
    while (n != 0) {
        o = *pp++;
        n--;
        if (!(o->active & 1))
            continue;
        qq = D_1F800224;
        for (D_1F80019E = D_1F80024C; D_1F80019E != 0; ) {
            e = *qq++;
            D_1F80019E--;
            if (!(e->active & 1))
                continue;
            switch (e->type) {
            case 0: func_800489D8(o, e); break;
            case 1: func_80126B50(o, e); break;
            case 2: func_80126C60(o, e); break;
            case 4:
                switch (D_8009C960) {
                case 0: func_80126D04(o, e); break;
                case 3: func_8012038C(o, e); break;
                }
                break;
            case 6: func_80126D7C(o, e); break;
            case 0xf: func_801280DC(o, e); break;
            case 0x14: func_80127FDC(o, e); break;
            case 0x1c: func_801216A0(o, e); break;
            case 0x23: func_8012788C(o, e); break;
            case 0x28: func_801210B4(o, e); break;
            case 0x1f: func_8011FA74(o, e); break;
            case 0x22: func_801203D0(o, e); break;
            case 0x1b:
                if (hit(o, e))
                    e->b68 = 1;
                break;
            case 0x34: func_80121994(o, e); break;
            case 0x33: func_80121A40(o, e); break;
            case 0x1e: func_8011FAE8(o, e); break;
            }
        }
    }
}
