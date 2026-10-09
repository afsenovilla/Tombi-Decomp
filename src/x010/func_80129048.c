// FUNC 80129048 380 X010
// MATCHING 80129048 380
#include "TOBJ.H"

extern unsigned char D_8012F448[];
extern unsigned char D_8012F420[];
extern void *D_80132384[];
extern void *D_8013235C[];
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001fe6c(TObj *);
extern short FUN_80040278(TObj *, short, short);

void func_80129048(TObj *o)
{
    int b0, b1, b2, b3;

    switch (o->state) {
    case 0:
        FUN_8001f8e4(o);
        o->timer = 8;
        o->state++;
        if (*(unsigned short *)&o->wb4) {
            b0 = D_8012F448[0];
            b1 = D_8012F448[1];
            b2 = D_8012F448[2];
            b3 = D_8012F448[3];
            o->wac = 0x1d;
            o->box0 = b0;
            o->box1 = b1;
            o->box2 = b2;
            o->box3 = b3;
            o->anim = D_80132384[0];
            FUN_8001fe6c(o);
        } else {
            b0 = D_8012F420[0];
            b1 = D_8012F420[1];
            b2 = D_8012F420[2];
            b3 = D_8012F420[3];
            o->wac = 0x13;
            o->box0 = b0;
            o->box1 = b1;
            o->box2 = b2;
            o->box3 = b3;
            o->anim = D_8013235C[0];
            FUN_8001fe6c(o);
        }
        break;
    case 1:
        o->y.p.whole -= 2;
        if (--o->timer == 0) {
            o->active = 1;
            o->y.p.whole += 8;
            if (o->b69 == 1 || FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x1a)) o->b69 = 0;
            o->state = 0;
            o->step++;
        }
        break;
    }
}
