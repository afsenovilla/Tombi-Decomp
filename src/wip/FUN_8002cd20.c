// FUNC 8002cd20 324 MAIN0
#include "tobj.h"
extern TObj *alloc();
extern unsigned short GetClut(int, int);
extern int g2cc, g2d8;
extern signed char g47;
void FUN_8002cd20(TObj *a, short b, char c)
{
    TObj *o = alloc();
    Fix16 y, bb;
    if (o != 0) {
        o->active = 1;
        o->type = 0x13;
        o->animFrame = a->animFrame;
        y = a->y;
        bb = a->b;
        o->a = a->a;
        o->y = y;
        o->b = bb;
        if (b == 0) {
            o->w1e = 0;
            o->w08 = GetClut(0x80, 0x1f0);
            o->b0d = 0x81;
            o->b0a = 8;
            o->b0f = 0xf9;
            o->subtype = 0;
            o->b0c = c;
            o->b1d = 0x4d;
            *(TObj **)&o->d90 = a;
            o->d3c = g2cc;
            o->category |= 0x80;
        } else {
            o->w1e = 0x14;
            o->b0d = 0;
            o->b0a = 2;
            o->subtype = b;
            o->b0c = c;
            o->d3c = g2d8;
            o->category |= 0x80;
            o->b1d = 0x4d;
            *(TObj **)&o->d90 = a;
            o->b0f = g47 - 1;
        }
    }
}
