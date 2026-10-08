// FUNC 80123b20 320 X003
// MATCHING 80123b20 320
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern char D_80077CF4[];
extern unsigned char D_80135C5C[];
extern A *D_801386F8[];
extern B D_80135B30[];
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8001fb20(TObj *);
extern int func_801206F0(TObj *);

void func_80123B20(TObj *o)
{
    unsigned char *p;

    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->b69 = 0;
        o->state++;
        o->wac = D_80135C5C[o->b0c];
        o->anim = D_801386F8[o->wac];
        { B *t = D_80135B30; p = t[((A *)o->anim)->w2].c; }
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        o->animTimer = ((A *)o->anim)->w6 & 0x3fff;
        break;
    case 1:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (func_801206F0(o) != 0) {
            o->timer = 0;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        break;
    }
}
