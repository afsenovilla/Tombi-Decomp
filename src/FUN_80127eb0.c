// FUNC 80127eb0 736 X000
// MATCHING 80127eb0 736
#include "TOBJ.H"
typedef struct { unsigned char _p[0xa]; unsigned char ba, bb; short wc, we; } EX;
extern void FUN_8001faf4(TObj *);
extern void FUN_80127214(TObj *);
extern int FUN_801274cc(TObj *);
extern TObj *FUN_80018448(void);
extern char DAT_80077cdc[];
extern unsigned char DAT_80138fd8[];
extern unsigned short *PTR_DAT_8013a1b8[];
extern unsigned short *PTR_DAT_8013a1bc[];

void FUN_80127eb0(TObj *o)
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
        o->movetab = DAT_80077cdc;
        o->velV = -0x280;
        o->b9c = 1;
        o->b69 = 0;
        o->wac = 0x16;
        o->substep++;
        a = PTR_DAT_8013a1b8[0];
        goto common;
    case 2:
        FUN_8001faf4(o);
        FUN_80127214(o);
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) o->b9c = 2;
        o->b69 = 0;
        o->timer = 0;
        o->substep++;
        break;
    case 3:
        if (o->velV < 0x400) FUN_8001faf4(o);
        FUN_80127214(o);
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (FUN_801274cc(o)) {
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
        o->wac = 0x17;
        o->substep++;
        a = PTR_DAT_8013a1bc[0];
    common:
        o->anim = a;
        p = &DAT_80138fd8[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 5:
        FUN_80127214(o);
        if (--o->timer == -1) {
            o->state = x->ba;
            o->substep = x->bb;
            o->b68 = 0;
        }
        break;
    }
}
