// FUNC 80124ffc 456 X003
// MATCHING 80124ffc 456
// VERSION SLPS_01144 x03 80123E68 448
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; } A;
typedef struct B { unsigned short c[4]; } B;
extern A *SPRING_PAD_ANIMS[];
extern B SPRING_PAD_ANIM_BOXES[];
extern int FUN_800202b4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001e4f0(int);
extern void freeObjectLayer2(TObj *);

void func_80123E68(TObj *o)
{
    unsigned short *p;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 8;
        o->b0d = 0;
        o->b69 = 0;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->anim = 0;
        break;
    case 1:
        if (FUN_800202b4(o)) {
            switch (o->step) {
            case 0:
                if (o->b69) {
                    FUN_8001e4f0(0x73);
                    o->active = 2;
                    o->step++;
                    o->anim = SPRING_PAD_ANIMS[o->subtype];
                    FUN_8001fe6c(o);
                }
                break;
            case 1:
                if (FUN_8001fec0(o)) {
                    o->active = 1;
                    o->b69 = 0;
                    o->anim = 0;
                    o->step--;
                }
                break;
            }
            if (o->anim) p = SPRING_PAD_ANIM_BOXES[((A *)o->anim)->w2].c;
            else p = SPRING_PAD_ANIM_BOXES[SPRING_PAD_ANIMS[o->subtype]->w2].c;
            o->box0 = *p++;
            o->box1 = *p++;
            o->box2 = *p;
            o->box3 = p[1];
        }
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#else
#include "spad.h"
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; } A;
typedef struct B { unsigned short c[4]; } B;
extern A *SPRING_PAD_ANIMS[];
extern B SPRING_PAD_ANIM_BOXES[];
extern int FUN_800202b4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001e4f0(int);
extern void freeObjectLayer2(TObj *);

void updateSpringPad(TObj *o)
{
    unsigned short *p;

    switch (o->b04) {
    case 0:
        o->b04++;
        *(signed char *)&o->b0f = -6;
        o->w1e = 8;
        o->b0d = 0;
        o->b69 = 0;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->anim = 0;
        break;
    case 1:
        if (FUN_800202b4(o)) {
            switch (o->step) {
            case 0:
                if (o->b69) {
                    FUN_8001e4f0(0x73);
                    o->active = 2;
                    o->step++;
                    o->anim = SPRING_PAD_ANIMS[o->subtype];
                    FUN_8001fe6c(o);
                }
                break;
            case 1:
                if (FUN_8001fec0(o)) {
                    o->active = 1;
                    o->b69 = 0;
                    o->anim = 0;
                    o->step--;
                }
                break;
            }
            if (o->anim) p = SPRING_PAD_ANIM_BOXES[((A *)o->anim)->w2].c;
            else p = SPRING_PAD_ANIM_BOXES[SPRING_PAD_ANIMS[o->subtype]->w2].c;
            o->box0 = *p++;
            o->box1 = *p++;
            o->box2 = *p;
            o->box3 = p[1];
        }
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
