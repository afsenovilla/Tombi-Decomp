// FUNC 80128fac 1360 X000
// MATCHING 80128fac 1360
#include "TOBJ.H"
typedef struct { char p0; unsigned char b01; char p2[2]; short w04; char p6[4]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char D_80077D3C[];
extern unsigned char D_80138FD8[];
extern unsigned short *D_8013A184[], *D_8013A1A4[], *D_8013A1A8[], *D_8013A1AC[], *D_8013A1FC[];
extern short D_1F80016A;
extern void ObjSetFacingToPlayer(TObj *);
extern void FUN_8001fb20(TObj *);
extern int FUN_801274cc(TObj *);
extern int FUN_801275e4(TObj *);
extern int AnimAdvanceWithBox(TObj *);

#define SETANIM(o, T) \
    { \
        unsigned short *a; \
        unsigned char *p; \
        a = T[0]; \
        o->anim = a; \
        p = &D_80138FD8[a[1] * 4]; \
        o->box0 = *p++; \
        o->box1 = *p++; \
        o->box2 = p[0]; \
        o->box3 = p[1]; \
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff; \
    }

static __inline__ short past(TObj *o)
{
    if (o->animFrame) {
        if (o->h->p.whole >= D_1F80016A - 0x30) return 0;
        return 1;
    }
    if (D_1F80016A + 0x30 < o->h->p.whole) return 1;
    return 0;
}

void FUN_80128fac(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->b9c = 0;
        x->w0e = 0;
        o->substep++;
        ObjSetFacingToPlayer(o);
        o->timer = 0x28;
        o->wac = 9;
        SETANIM(o, D_8013A184);
        FUN_801274cc(o);
        break;
    case 1:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->b68 = 1;
            o->velH = 0x280;
            o->movetab = D_80077D3C;
            o->b69 = 0;
            o->wac = 0x11;
            o->substep++;
            SETANIM(o, D_8013A1A4);
            break;
        }
        o->y.p.whole += 2;
        FUN_801274cc(o);
        break;
    case 2:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (o->animFrame)
            o->h->raw = o->h->raw - (o->velH << 8);
        else
            o->h->raw = o->h->raw + (o->velH << 8);
        if (FUN_801274cc(o) == 0 && x->w0e++ > 3) {
            x->b0a = 1;
            x->b0b = 2;
            o->state = 7;
            o->substep = 0;
            break;
        }
        if (FUN_801275e4(o) != 0) {
            o->state = 4;
            o->substep = 0;
            break;
        }
        if (past(o)) {
            o->substep = 3;
            x->w0e = 0;
        }
        o->velH += 0x10;
        if (o->velH > 0x300) o->velH = 0x300;
        break;
    case 3:
        o->velH = 0x200;
        o->b68 = 0;
        o->wac = 0x12;
        o->substep++;
        SETANIM(o, D_8013A1A8);
    case 4:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (o->animFrame)
            o->h->raw = o->h->raw - (o->velH << 8);
        else
            o->h->raw = o->h->raw + (o->velH << 8);
        FUN_801275e4(o);
        if (FUN_801274cc(o) == 0 && x->w0e++ > 3) {
            x->b0a = 1;
            x->b0b = 2;
            o->state = 7;
            o->substep = 0;
            break;
        }
        o->velH -= 0x10;
        if (o->velH < 0) {
            o->timer = 0x28;
            o->wac = 0x13;
            o->substep++;
            SETANIM(o, D_8013A1AC);
        }
        break;
    case 5:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x5a;
            o->wac = 0x27;
            o->substep++;
            SETANIM(o, D_8013A1FC);
        }
        o->y.p.whole += 2;
        FUN_801274cc(o);
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->state = 1;
            o->substep = 2;
            o->animFrame = 1 - o->animFrame;
        }
        o->y.p.whole += 2;
        FUN_801274cc(o);
        break;
    }
}
