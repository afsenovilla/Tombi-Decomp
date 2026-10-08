// FUNC 801216b4 1680 X004
// MATCHING 801216b4 1680
#include "TOBJ.H"
typedef struct { char p0; unsigned char b01; char p2[2]; short w04; char p6[4]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char D_80077CF4[];
extern unsigned char D_80130FD4[];
extern unsigned char D_80130FA4[];
extern unsigned short *D_80133B98[], *D_80133B9C[];
extern void ObjSetFacingToPlayer(TObj *);
extern void FUN_8001fab4(TObj *);
extern void FUN_8001fa20(TObj *, unsigned short);
extern unsigned FUN_8001f9e0(void);
extern short FUN_80041240(TObj *, short, short);
extern int func_8011FDD4(TObj *);
extern int func_8011FEF0(TObj *);
extern int AnimAdvanceWithBox(TObj *);

#define SETANIM(o, T) \
    { \
        unsigned short *a; \
        unsigned char *p; \
        a = T[0]; \
        o->anim = a; \
        p = &D_80130FD4[a[1] * 4]; \
        o->box0 = *p++; \
        o->box1 = *p++; \
        o->box2 = p[0]; \
        o->box3 = p[1]; \
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff; \
    }

static __inline__ int inRange(TObj *o)
{
    if ((unsigned short)(o->d->p.whole - *(unsigned short *)0x1F800172 + 45) >= 91) return 0;
    if ((unsigned short)(o->y.p.whole - *(unsigned short *)0x1F80016E + 70) >= 111) return 0;
    return (unsigned short)(o->h->p.whole - *(unsigned short *)0x1F80016A + 128) < 257;
}

void func_801216B4(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->b9c = 0;
        x->w0e = 0;
        x->b01 = 0;
        o->timer = 0x3c;
        o->substep++;
        ObjSetFacingToPlayer(o);
        o->movetab = D_80077CF4;
        o->wac = 0xf;
        SETANIM(o, D_80133B98);
    case 1:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if (inRange(o)) {
                o->timer = 8;
                o->wac = 0x10;
                o->substep++;
                SETANIM(o, D_80133B9C);
            } else {
                o->state = 0;
                o->substep = 0;
            }
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    case 2:
        AnimAdvanceWithBox(o);
        FUN_8001fab4(o);
        if (func_8011FDD4(o) == 0 && x->w0e++ > 3) {
            x->b0b = 6;
            x->b0a = 0;
            o->state = 7;
            o->substep = 0;
            break;
        }
        if (--o->timer == -1 || func_8011FEF0(o) != 0) {
            o->timer = 0x3c;
            o->wac = 0xf;
            o->substep++;
            SETANIM(o, D_80133B98);
        }
        break;
    case 3:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if (inRange(o)) {
                o->substep++;
                x->w0e = 0;
                o->timer = 8;
                o->wac = 0x10;
                SETANIM(o, D_80133B9C);
            } else {
                o->state = 0;
                o->substep = 0;
            }
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    case 4:
        AnimAdvanceWithBox(o);
        FUN_8001fa20(o, 1 - o->animFrame);
        if (func_8011FDD4(o) == 0 && x->w0e++ > 3) {
            x->b0b = 6;
            x->b0a = 0;
            o->state = 7;
            o->substep = 0;
            break;
        }
        if (o->b9d & 2) {
            if (o->animFrame != (o->b9d & 1)) o->timer = 0;
        }
        if (o->animFrame == 1) x->w04 = 0x10;
        else x->w04 = -0x10;
        if (FUN_80041240(o, o->h->p.whole + x->w04, o->y.p.whole) != 0) o->timer = 0;
        if (--o->timer == -1) {
            o->substep++;
            ObjSetFacingToPlayer(o);
            o->timer = 0x3c;
            o->wac = 0xf;
            SETANIM(o, D_80133B98);
        }
        break;
    case 5:
        AnimAdvanceWithBox(o);
        if (inRange(o)) {
            if (--o->timer == -1) o->substep++;
        } else {
            o->state = 0;
            o->substep = 0;
        }
        o->y.p.whole += 2;
        func_8011FDD4(o);
        break;
    case 6:
        o->y.p.whole += 2;
        func_8011FDD4(o);
        if (x->b01 != 2) {
            if (D_80130FA4[FUN_8001f9e0() & 0xf] != 0) goto ok;
        }
        o->state = 3;
        o->substep = 0;
        break;
    ok:
        o->substep = 2;
        x->b01++;
        o->timer = 8;
        o->wac = 0x10;
        SETANIM(o, D_80133B9C);
        break;
    }
}
