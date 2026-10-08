// FUNC 801231d0 320 X004
// MATCHING 801231d0 320
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern char D_80077CF4[];
extern unsigned char D_801310E8[];
extern A *D_80133B5C[];
extern B D_80130FD4[];
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8001fb20(TObj *);
extern int func_8011FDD4(TObj *);

void func_801231D0(TObj *o)
{
    unsigned char *p;

    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->b69 = 0;
        o->state++;
        o->wac = D_801310E8[o->b0c];
        o->anim = D_80133B5C[o->wac];
        { B *t = D_80130FD4; p = t[((A *)o->anim)->w2].c; }
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        o->animTimer = ((A *)o->anim)->w6 & 0x3fff;
        break;
    case 1:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (func_8011FDD4(o) != 0) {
            o->timer = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        break;
    }
}
