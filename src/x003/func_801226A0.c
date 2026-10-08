// FUNC 801226a0 1444 X003
// MATCHING 801226a0 1444
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { unsigned char b0, b1; char p2[0xa - 2]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char D_80077CF4[];
extern unsigned char D_80135B10[];
extern unsigned char D_80135B20[];
extern B D_80135B30[];
extern A *D_801386F8[];
extern int Rand(void);
extern void SfxPlay2(int, int);
extern void ObjSetFacingToPlayer(TObj *);
extern void FUN_8001fb20(TObj *);
extern int AnimAdvanceWithBox(TObj *);
extern void func_801205A4(TObj *);
extern int func_801206F0(TObj *);
extern int func_8012080C(TObj *);
extern unsigned char FUN_801208f8(TObj *);

#define SETANIM(k) \
    o->anim = D_801386F8[k]; \
    { unsigned char *p; { B *t = D_80135B30; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

#define STEP(k) \
    if (o->animTimer == 1) { \
        k = AnimAdvanceWithBox(o); \
        s = ((A *)o->anim)->w4; \
        if (s != 0) SfxPlay2((short)s >> 8, s & 0xff); \
    } else { \
        k = AnimAdvanceWithBox(o); \
    }

void func_801226A0(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);
    unsigned char r;
    short s;
    int k;
    unsigned char a;

    switch (o->substep) {
    case 12:
        o->animFrame = 1 - o->animFrame;
        o->substep = 0;
    case 0:
        o->movetab = D_80077CF4;
        x->b1 = 0;
        o->substep++;
    case 1:
        x->w0e = 0;
        o->b69 = 0;
        o->b9d = 0;
        o->timer = 0x38;
        o->substep++;
        if (D_80135B10[Rand() & 0xf] != 0) {
            o->timer = 0x60;
        }
        o->wac = 7;
        SETANIM(7);
        break;
    case 2:
        if (--o->timer == -1) {
            o->substep++;
        }
        func_801205A4(o);
        o->y.p.whole += 4;
        if (func_801206F0(o) == 0 && x->w0e++ >= 4) {
            x->b0b = 6;
            x->b0a = 0;
            o->state = 7;
            o->substep = 0;
            break;
        }
        if (func_8012080C(o) != 0) {
            o->substep = 8;
            break;
        }
        r = FUN_801208f8(o);
        if (r != 0xff) {
            o->state = r;
            o->substep = 0;
        }
        break;
    case 3:
        a = x->b1;
        if (a == 3) {
            o->substep = 6;
            break;
        }
        x->b1 = a + 1;
        o->substep++;
        x->b0 = 0;
        o->wac = D_80135B20[Rand() & 0xf];
        SETANIM(o->wac);
        break;
    case 4:
        FUN_8001fb20(o);
        func_801206F0(o);
        STEP(k);
        if (k == 0) break;
        if (x->b0 == 0) {
            r = FUN_801208f8(o);
            if (r != 0xff) {
                o->state = r;
                o->substep = 0;
            } else {
                o->substep = 1;
            }
            break;
        }
        o->wac = 3;
        o->substep++;
        if (x->b0 == 1) {
            o->wac = 5;
        }
        SETANIM(o->wac);
        break;
    case 5:
        FUN_8001fb20(o);
        func_801206F0(o);
        STEP(k);
        if (k == 0) break;
        r = FUN_801208f8(o);
        if (r != 0xff) {
            o->state = r;
            o->substep = 0;
        } else {
            o->substep = 1;
        }
        break;
    case 6:
        o->timer = 0x5a;
        o->movetab = D_80077CF4;
        o->b9d = 0;
        o->b9c = 0;
        o->wac = 9;
        o->substep++;
        SETANIM(9);
        break;
    case 7:
        if (--o->timer == -1) {
            ObjSetFacingToPlayer(o);
            o->state = 0;
            o->substep = 0;
            r = FUN_801208f8(o);
            if (r == 0xff) break;
            o->state = r;
        }
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        func_801206F0(o);
        break;
    case 8:
        o->substep++;
        o->wac = 6;
        SETANIM(6);
        break;
    case 9:
        STEP(k);
        if (k != 0) {
            o->substep++;
        }
        FUN_8001fb20(o);
        func_801206F0(o);
        break;
    case 10:
        o->timer = 0x3c;
        o->b9d = 0;
        o->wac = 9;
        o->substep++;
        SETANIM(9);
        break;
    case 11:
        if (--o->timer == -1) {
            o->substep = 12;
            r = FUN_801208f8(o);
            if (r == 0xff) break;
            o->state = r;
            o->substep = 0;
        }
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        func_801206F0(o);
        break;
    }
}
