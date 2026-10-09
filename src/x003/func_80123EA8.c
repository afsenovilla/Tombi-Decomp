// FUNC 80123ea8 792 X003
// MATCHING 80123ea8 792
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern char D_80077CF4[];
extern B D_80135B30[];
extern A *D_801386F8[];
extern unsigned char D_80135C5C[];
extern int func_80120438(TObj *);
extern int func_801206F0(TObj *);

#define SETANIM(k) \
    o->anim = D_801386F8[k]; \
    { unsigned char *p; { B *t = D_80135B30; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

#define DSINK() \
    { short d = o->d->p.whole; \
    if (d) { \
        o->d->p.whole = d - 8; \
        if ((short)(d - 8) < 0) { \
            o->active = 1; \
            o->d->p.whole = 0; \
        } \
    } }

void func_80123EA8(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b0f = 0;
        o->state++;
        o->wac = D_80135C5C[o->b0c];
        SETANIM(o->wac);
        break;
    case 1:
        if (func_80120438(o)) o->state++;
        break;
    case 2:
        o->b9c = 1;
        o->movetab = D_80077CF4;
        o->velV = -0x200;
        o->b69 = 0;
        o->wac = 0x11;
        o->state++;
        SETANIM(0x11);
        break;
    case 3:
        DSINK();
    case 6:
        func_80120438(o);
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            *(signed char *)&o->b0f = -9;
            o->b9c = 2;
            o->state++;
        }
        break;
    case 4:
        DSINK();
    case 7:
        func_80120438(o);
        o->velV += 0x30;
        if (o->velV > 0x800) o->velV = 0x800;
        o->y.raw += o->velV << 8;
        if (func_801206F0(o)) {
            o->b9c = 0;
            o->state++;
        }
        break;
    case 5:
        o->b9c = 1;
        o->velV = -0x200;
        o->timer = 0x3c;
        o->wac = 0x13;
        o->state++;
        SETANIM(0x13);
        break;
    case 8:
        o->b9c = 1;
        o->timer = 0x3c;
        o->wac = 0x15;
        o->state++;
        SETANIM(0x15);
        break;
    case 9:
        func_80120438(o);
        if (--o->timer == -1) {
            o->step = 1;
            o->state = 0;
            o->substep = 6;
            o->b69 = 0;
            o->b68 = 0;
        }
        break;
    }
}
