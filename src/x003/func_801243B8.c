// FUNC 801243b8 1984 X003
// MATCHING 801243b8 1984
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { short w0, w2, w4, w6, w8, wa, wc; } X;
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C942[];
extern unsigned char D_800A603C[];
extern unsigned char D_800A603D[];
extern unsigned char D_800A603E[];
extern unsigned char D_800A4553[];
extern int D_800A4568[];
extern int D_1F80018C;
extern int D_1F800190;
extern char D_80077CF4[], D_80077CDC[];
extern B D_80135B30[];
extern A *D_801386F8[];
extern unsigned char D_8009CDC3;
extern int D_1F80018C, D_1F800190;
extern TObj *ObjAlloc(void);
extern int func_801206F0(TObj *);
extern void func_80120438(TObj *);
extern void FUN_8005a8a8(int, int, int);

#define SETANIM(k) \
    o->anim = D_801386F8[k]; \
    { unsigned char *p; { B *t = D_80135B30; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

#define DUST() \
    if (!(o->b69 & 8)) { \
        TObj *e = ObjAlloc(); \
        if (e) { \
            e->active = 1; \
            e->type = 0x10; \
            e->subtype = 1; \
            e->a.p.whole = o->a.p.whole; \
            e->y.p.whole = o->y.p.whole + 0x10; \
            e->b.p.whole = o->b.p.whole; \
        } \
    }

void func_801243B8(TObj *o)
{
    X *x = (X *)&o->wb4;

    switch (o->state) {
    case 0:
        if (D_8009CDC3) {
            o->b04 = 3;
            break;
        }
        o->d8c = 0x100;
        o->anim = 0;
        o->state++;
        break;
    case 1:
        break;
    case 2:
        o->state++;
        x->wc = 0x100;
        o->b9c = 2;
        o->movetab = D_80077CF4;
        o->b69 = 0;
        o->velV = 0;
        o->wac = 0x17;
        SETANIM(0x17);
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        D_800A4553[0] = 3;
        D_800A4568[0] = 0;
        D_1F80018C = o->h->raw;
        D_1F800190 = o->y.raw;
        break;
    case 3:
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (func_801206F0(o)) {
            DUST();
            o->b69 = 0;
            x->wc = 0x100;
            o->movetab = D_80077CDC;
            o->timer = 2;
            o->wac = 0x16;
            o->state++;
            SETANIM(0x16);
        }
        goto tail;
    case 4:
        if (--o->timer == -1) {
            o->timer = 0;
            o->b69 = 0;
            x->wc = 0;
            o->velV = -0x300;
            o->b9c = 1;
            o->wac = 0x17;
            o->state++;
            SETANIM(0x17);
        }
        goto tail;
    case 5:
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            if (func_801206F0(o)) {
                DUST();
                o->timer = 2;
                o->wac = 0x16;
                o->state++;
                SETANIM(0x16);
            }
        }
        goto tail;
    case 6:
        if (--o->timer == -1) {
            o->timer = 0;
            x->wc = 0;
            o->b9c = 1;
            o->velV = -0x200;
            o->b69 = 0;
            o->wac = 0x17;
            o->state++;
            SETANIM(0x17);
        }
        goto tail;
    case 7:
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (o->timer == 0) {
            x->wc += 2;
            if (x->wc >= 0x16) {
                x->wc = 0xe0;
                o->timer = 1;
                o->wac = 0x18;
                SETANIM(0x18);
            }
        } else {
            x->wc += 4;
            if (x->wc >= 0x100) x->wc = 0x100;
        }
        if (o->velV > 0) {
            o->b9c = 2;
            if (func_801206F0(o)) {
                DUST();
                D_8009C93F[0] = 0;
                D_8009C942[0] = 0;
                FUN_8005a8a8(0x1f, 0, 1);
                o->b69 = 0;
                x->wc = 0;
                o->timer = 0x64;
                o->wac = 0x10;
                o->state++;
                SETANIM(0x10);
            }
        }
    tail:
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        break;
    case 8:
        func_80120438(o);
        if (!D_8009C93F[0]) {
            o->state++;
            D_800A4553[0] = 6;
            o->wac = 9;
            SETANIM(9);
        }
        break;
    case 9:
        func_80120438(o);
        if (--o->timer == -1) o->state++;
        break;
    case 10:
        o->active = 1;
        o->step = 1;
        o->subtype = 0;
        o->state = 0;
        o->substep = 6;
        o->b69 = 0;
        o->b68 = 0;
        break;
    }
    if (o->animFrame) o->d8c = *(unsigned char *)&x->wc;
    else o->d8c = (-x->wc) & 0xff;
}
