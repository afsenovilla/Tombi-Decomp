// FUNC 801209f8 1712 X003
// MATCHING 801209f8 1712
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { short w0, w2, w4, w6, w8; unsigned char bA, bB; short wC, wE; } X;
extern char D_80077CDC[];
extern char D_80077CF4[];
extern B D_80135B30[];
extern A *D_801386F8[];
extern void FUN_8001faf4(TObj *);
extern void func_8012080C(TObj *);
extern int func_801206F0(TObj *);
extern int AnimAdvanceWithBox(TObj *);
extern TObj *ObjAlloc(void);

#define SETANIM(k) \
    { A *a = D_801386F8[k]; unsigned char *p; \
    o->anim = a; \
    p = D_80135B30[a->w2].c; \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff; }

#define SPAWN() \
    { TObj *e = ObjAlloc(); \
    if (e) { \
        e->active = 1; \
        e->type = 0x10; \
        e->subtype = 1; \
        e->a.p.whole = o->a.p.whole; \
        e->y.p.whole = o->y.p.whole + 0x10; \
        e->b.p.whole = o->b.p.whole; \
    } }

void func_801209F8(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->substep) {
    case 0:
        {
            int t = o->d8c;
            o->b9c = 2;
            x->wE = 0;
            x->wC = t;
        }
        o->substep++;
        o->b69 = 0;
        o->velV = 0;
        if (o->movetab == 0) o->movetab = D_80077CF4;
        break;
    case 1:
        FUN_8001faf4(o);
        func_8012080C(o);
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        x->wC += 8;
        if (x->wC < 0x33) break;
        o->b69 = 0;
        o->substep++;
        x->wC = 0xc0;
        o->wac = 0x17;
        SETANIM(0x17);
        break;
    case 2:
        FUN_8001faf4(o);
        func_8012080C(o);
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        x->wC += 5;
        if (x->wC >= 0x100) x->wC = 0x100;
        if (!func_801206F0(o)) break;
        if (!(o->b69 & 8)) SPAWN();
        o->b69 = 0;
        x->wC = 0x100;
        o->movetab = D_80077CDC;
        o->timer = 2;
        o->wac = 0x16;
        o->substep++;
        SETANIM(0x16);
        break;
    case 3:
        if (--o->timer != -1) break;
        o->timer = 0;
        o->b69 = 0;
        x->wC = 0;
        o->velV = -0x300;
        o->b9c = 1;
        o->wac = 0x17;
        o->substep++;
        SETANIM(0x17);
        break;
    case 4:
        FUN_8001faf4(o);
        func_8012080C(o);
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (o->velV <= 0) break;
        o->b9c = 2;
        if (!func_801206F0(o)) break;
        if (!(o->b69 & 8)) SPAWN();
        o->timer = 2;
        o->wac = 0x16;
        o->substep++;
        SETANIM(0x16);
        break;
    case 5:
        if (--o->timer != -1) break;
        o->timer = 0;
        x->wC = 0;
        o->b9c = 1;
        o->velV = -0x200;
        o->b69 = 0;
        o->wac = 0x17;
        o->substep++;
        SETANIM(0x17);
        break;
    case 6:
        FUN_8001faf4(o);
        func_8012080C(o);
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (o->timer == 0) {
            x->wC += 2;
            if (x->wC >= 0x16) {
                x->wC = 0xe0;
                o->timer = 1;
                o->wac = 0x18;
                SETANIM(0x18);
            }
        } else {
            x->wC += 4;
            if (x->wC >= 0x100) x->wC = 0x100;
        }
        if (o->velV <= 0) break;
        o->b9c = 2;
        if (!func_801206F0(o)) break;
        if (!(o->b69 & 8)) SPAWN();
        o->b69 = 0;
        x->wC = 0;
        o->timer = 0x14;
        o->wac = 0x10;
        o->substep++;
        SETANIM(0x10);
        break;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer != -1) break;
        o->state = x->bA;
        o->substep = x->bB;
        o->b68 = 0;
        break;
    }
    if (o->animFrame) o->d8c = (unsigned char)x->wC;
    else o->d8c = -x->wC & 0xff;
}
