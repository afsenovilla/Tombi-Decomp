// FUNC 801210a8 752 X003
// MATCHING 801210a8 752
#include "TOBJ.H"
typedef struct { unsigned char _p[0xa]; unsigned char ba, bb; short wc, we; } EX;
extern void FUN_8001faf4(TObj *);
extern void func_80120438(TObj *);
extern int func_801206F0(TObj *);
extern TObj *FUN_80018448(void);
extern char D_80077CDC[];
extern unsigned char D_80135B30[];
extern unsigned short *D_80138734[];
extern unsigned short *D_80138738[];

void func_801210A8(TObj *o)
{
    EX *x = (EX *)((char *)o + 0xb4);
    unsigned char *p;
    unsigned short *a;
    TObj *q;

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->substep++;
    case 1:
        x->wc = o->d8c;
        x->we = 0;
        o->movetab = D_80077CDC;
        o->velV = -0x280;
        o->b9c = 1;
        o->b69 = 0;
        o->wac = 0xf;
        o->substep++;
        a = D_80138734[0];
        goto common;
    case 2:
        FUN_8001faf4(o);
        func_8012080C(o);
        func_80120438(o);
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) o->b9c = 2;
        o->b69 = 0;
        o->timer = 0;
        o->substep++;
        break;
    case 3:
        if (o->velV < 0x400) {
            FUN_8001faf4(o);
            func_8012080C(o);
        }
        func_80120438(o);
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (func_801206F0(o)) {
            if (!(o->b69 & 8)) {
                q = FUN_80018448();
                if (q != 0) {
                    q->active = 1;
                    q->type = 0x10;
                    q->subtype = 1;
                    q->a.p.whole = o->a.p.whole;
                    q->y.p.whole = o->y.p.whole + 0x10;
                    q->b.p.whole = o->b.p.whole;
                }
            }
            o->substep++;
        } else {
            if (o->timer++ >= 0x35) {
                o->state = 7;
                o->substep = 1;
            }
        }
        break;
    case 4:
        o->timer = 0x1e;
        o->wac = 0x10;
        o->substep++;
        a = D_80138738[0];
    common:
        o->anim = a;
        p = &D_80135B30[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 5:
        func_80120438(o);
        if (--o->timer == -1) {
            o->state = x->ba;
            o->substep = x->bb;
            o->b68 = 0;
        }
        break;
    }
}
