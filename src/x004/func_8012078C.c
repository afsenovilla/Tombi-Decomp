// FUNC 8012078c 752 X004
// MATCHING 8012078c 752
#include "TOBJ.H"
typedef struct { unsigned char b0, b1; char p2[8]; unsigned char b0a, b0b; short w0c; short w0e; } X;
extern char D_80077CDC[];
extern unsigned char D_80130FD4[];
extern unsigned short *D_80133BB4[], *D_80133BB8[];
extern TObj *ObjAlloc(void);
extern void FUN_8001faf4(TObj *);
extern int func_8011FDD4(TObj *);
extern int func_8011FEF0(TObj *);
extern int AnimAdvanceWithBox(TObj *);

static __inline__ void SETBOX(TObj *o, int idx)
{
    unsigned char *b = D_80130FD4 + idx * 4;
    o->box0 = *b++;
    o->box1 = *b++;
    o->box2 = *b;
    o->box3 = b[1];
}

void func_8012078C(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);
    unsigned short *a;
    TObj *e;

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->substep++;
    case 1:
        x->w0c = o->d8c;
        x->w0e = 0;
        o->movetab = D_80077CDC;
        o->velV = -0x280;
        o->b9c = 1;
        o->b69 = 0;
        o->wac = 0x16;
        o->substep++;
        a = D_80133BB4[0];
        o->anim = a;
        SETBOX(o, a[1]);
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 2:
        FUN_8001faf4(o);
        func_8011FEF0(o);
        AnimAdvanceWithBox(o);
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
            func_8011FEF0(o);
        }
        AnimAdvanceWithBox(o);
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (func_8011FDD4(o)) {
            if (!(o->b69 & 8)) {
                e = ObjAlloc();
                if (e) {
                    e->active = 1;
                    e->type = 0x10;
                    e->subtype = 1;
                    e->a.p.whole = o->a.p.whole;
                    e->y.p.whole = o->y.p.whole + 0x10;
                    e->b.p.whole = o->b.p.whole;
                }
            }
            o->substep++;
        } else if (o->timer++ > 0x34) {
            o->state = 7;
            o->substep = 1;
        }
        break;
    case 4:
        o->timer = 0x1e;
        o->wac = 0x17;
        o->substep++;
        a = D_80133BB8[0];
        o->anim = a;
        SETBOX(o, a[1]);
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 5:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->state = x->b0a;
            o->substep = x->b0b;
            o->b68 = 0;
        }
        break;
    }
}
