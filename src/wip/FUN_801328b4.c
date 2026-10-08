// FUNC 801328b4 764 X000
/* score 24: only case 0 scheduling after the GetClut call differs (game stores anim right after loading it and loads b0c for the copy before the b69/b6b/step stores). Tried full permutation/hill-climb of the statements, raw stores, early row pointer, -fno-expensive-optimizations. */
#include "TOBJ.H"

typedef struct { unsigned char b[8]; } V801328B4;

extern V801328B4 D_801392EC[][4];
extern void *D_8013B24C[];
extern void (*D_801392E0[])(TObj *);
extern int D_1F8002D4;
extern unsigned short FUN_8005e420(int, int);
extern int ObjCullRegister(TObj *);
extern void AnimLoadDuration(TObj *);
extern void freeObjectLayer2(TObj *);

#define COPY() \
    ((V801328B4 *)&o->wb4)[0] = D_801392EC[o->b0c][0]; \
    ((V801328B4 *)&o->wb4)[1] = D_801392EC[o->b0c][1]; \
    ((V801328B4 *)&o->wb4)[2] = D_801392EC[o->b0c][2]; \
    ((V801328B4 *)&o->wb4)[3] = D_801392EC[o->b0c][3];

void FUN_801328b4(TObj *o)
{
    V801328B4 *t;

    switch (o->b04) {
    case 0:
        o->box0 = 0x20;
        o->box1 = 0x40;
        o->box2 = 8;
        o->box3 = 0x10;
        o->active = 1;
        if (o->b0c == 0)
            o->active = 2;
        o->w1e = 0xf;
        o->b0d = 1;
        o->w08 = FUN_8005e420(0x90, 0x1e6);
        o->b6b = 0;
        o->anim = D_8013B24C[o->b0c];
        t = D_801392EC[o->b0c];
        o->step = 0;
        o->b69 = 0;
        o->d3c = D_1F8002D4;
        o->b04++;
        ((V801328B4 *)&o->wb4)[0] = t[0];
        ((V801328B4 *)&o->wb4)[1] = D_801392EC[o->b0c][1];
        ((V801328B4 *)&o->wb4)[2] = D_801392EC[o->b0c][2];
        ((V801328B4 *)&o->wb4)[3] = D_801392EC[o->b0c][3];
        AnimLoadDuration(o);
        break;
    case 1:
        if (ObjCullRegister(o)) {
            D_801392E0[o->subtype](o);
        } else if (o->step) {
            COPY();
            o->step = 0;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
