// FUNC 80127820 1680 X000
// MATCHING 80127820 1680
#include "TOBJ.H"
typedef struct { unsigned char _p[0xa]; unsigned char ba, bb; short wc, we; } EX;
extern void FUN_8001faf4(TObj *);
extern void FUN_80127214(TObj *);
extern int FUN_801274cc(TObj *);
extern TObj *FUN_80018448(void);
extern char DAT_80077cdc[];
extern char DAT_80077cf4[];
extern unsigned char DAT_80138fd8[];
extern unsigned short *PTR_DAT_8013a1bc[];
extern unsigned short *PTR_DAT_8013a1d4[];
extern unsigned short *PTR_DAT_8013a1d8[];
extern unsigned short *PTR_DAT_8013a1dc[];
extern unsigned short *PTR_DAT_8013a1d4_b[]; /* same table as PTR_DAT_8013a1d4: second name keeps the case tails apart */
extern unsigned short *PTR_DAT_8013a1d8_b[];

void FUN_80127820(TObj *o)
{
    EX *x = (EX *)((char *)o + 0xb4);
    unsigned char *p;
    unsigned short *a;
    TObj *q;
    int t;

    switch (o->substep) {
    case 0:
        t = o->d8c;
        o->b9c = 2;
        x->we = 0;
        x->wc = t;
        o->b69 = 0;
        o->velV = 0;
        o->substep++;
        if (o->movetab == 0) o->movetab = DAT_80077cf4;
        break;
    case 1:
        FUN_8001faf4(o);
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        x->wc += 8;
        if (x->wc >= 0x33) {
            o->b69 = 0;
            o->substep++;
            x->wc = 0xc0;
            o->wac = 0x1e;
            a = PTR_DAT_8013a1d8[0];
            goto common;
        }
        break;
    case 2:
        FUN_8001faf4(o);
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        x->wc += 5;
        if (x->wc > 0xff) x->wc = 0x100;
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
            o->b69 = 0;
            x->wc = 0x100;
            o->movetab = DAT_80077cdc;
            o->timer = 2;
            o->wac = 0x1d;
            o->substep++;
            a = PTR_DAT_8013a1d4[0];
            goto common;
        }
        break;
    case 3:
        if (--o->timer == -1) {
            o->timer = 0;
            o->b69 = 0;
            x->wc = 0;
            o->velV = -0x300;
            o->b9c = 1;
            o->wac = 0x1e;
            o->substep++;
            a = PTR_DAT_8013a1d8[0];
            goto common;
        }
        break;
    case 4:
        FUN_8001faf4(o);
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
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
                o->timer = 2;
                o->substep++;
                o->wac = 0x1d;
                a = PTR_DAT_8013a1d4_b[0];
                goto common;
            }
        }
        break;
    case 5:
        if (--o->timer == -1) {
            o->timer = 0;
            x->wc = 0;
            o->b9c = 1;
            o->velV = -0x200;
            o->b69 = 0;
            o->substep++;
            o->wac = 0x1e;
            a = PTR_DAT_8013a1d8_b[0];
            goto common;
        }
        break;
    case 6:
        FUN_8001faf4(o);
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (o->timer == 0) {
            x->wc += 2;
            if (x->wc > 0x15) {
                x->wc = 0xe0;
                o->timer = 1;
                o->wac = 0x1f;
                a = PTR_DAT_8013a1dc[0];
                o->anim = a;
                {
                unsigned char *pp = &DAT_80138fd8[a[1] * 4];
                o->box0 = *pp++;
                o->box1 = *pp++;
                o->box2 = *pp++;
                o->box3 = *pp++;
                }
                o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
            }
        } else {
            x->wc += 4;
            if (x->wc > 0xff) x->wc = 0x100;
        }
        if (o->velV > 0) {
            o->b9c = 2;
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
                o->b69 = 0;
                x->wc = 0;
                o->timer = 0x14;
                o->wac = 0x17;
                o->substep++;
                a = PTR_DAT_8013a1bc[0];
            common:
                o->anim = a;
                {
                unsigned char *pp = &DAT_80138fd8[a[1] * 4];
                o->box0 = *pp++;
                o->box1 = *pp++;
                o->box2 = *pp++;
                o->box3 = *pp++;
                }
                o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
            }
        }
        break;
    case 7:
        FUN_80127214(o);
        if (--o->timer == -1) {
            o->state = x->ba;
            o->substep = x->bb;
            o->b68 = 0;
        }
        break;
    }
    if (o->animFrame) o->d8c = (unsigned char)x->wc;
    else o->d8c = -x->wc & 0xff;
}
