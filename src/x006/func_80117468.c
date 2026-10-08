// FUNC 80117468 584 X006
// MATCHING 80117468 584
#include "TOBJ.H"
extern void *D_80122BF8[];
extern int D_1F8002D4;
extern unsigned short D_1F800176, D_1F800186[];
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_80018ca4(TObj *);
extern void FUN_800187e4(TObj *);

void func_80117468(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->b0a = 7;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        switch (o->subtype) {
        case 0:
            o->b0d = 1;
            o->w08 = FUN_8005e420(0x80, 0x1ff);
            o->w1e = 2;
            o->anim = D_80122BF8[o->b0c];
            break;
        case 1:
            o->b0d = 1;
            o->w08 = FUN_8005e420(0x80, 0x1ff);
            o->w1e = 4;
            o->anim = D_80122BF8[o->b0c];
            break;
        case 2:
            o->b0d = 1;
            o->w08 = FUN_8005e420(0x80, 0x1ff);
            o->w1e = 4;
            o->anim = D_80122BF8[o->b0c];
            break;
        case 3:
            o->b0d = 1;
            o->w08 = FUN_8005e420(0x80, 0x1ff);
            o->w1e = 4;
            o->anim = D_80122BF8[o->b0c];
            break;
        case 4:
            o->b0d = 1;
            o->w08 = FUN_8005e420(0x80, 0x1ff);
            o->w1e = 6;
            o->anim = D_80122BF8[o->b0c];
            break;
        case 5:
            o->b0d = 1;
            o->w08 = FUN_8005e420(0x90, 0x1e0);
            o->w1e = 8;
            o->anim = D_80122BF8[o->b0c];
            break;
        case 6:
            o->b0d = 1;
            o->w08 = FUN_8005e420(0x80, 0x1fe);
            o->w1e = 0x10;
            o->anim = D_80122BF8[o->b0c];
            break;
        }
        o->d3c = D_1F8002D4;
        FUN_8001fe6c(o);
        break;
    case 1:
        o->a.p.whole = o->d30 - D_1F800176;
        o->y.p.whole = o->d34 - D_1F800186[0];
        o->visible = 1;
        FUN_80018ca4(o);
        FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
