// FUNC 8002cd20 324 MAIN0
// MATCHING 8002cd20 324
#include "tobj.h"
extern TObj *alloc();
typedef struct { int x, y, z; } V3;
extern unsigned short GetClut(int, int);
extern int g2cc, g2d8[];
extern unsigned char g47[];
void FUN_8002cd20(TObj *a, short b, char c)
{
    TObj *o = alloc();
    if (o != 0) {
        o->active = 1;
        o->type = 0x13;
        o->animFrame = a->animFrame;
        *(V3 *)&o->a = *(V3 *)&a->a;
        if (b == 0) {
            o->w1e = 0;
            o->w08 = GetClut(0x80, 0x1f0);
            o->b0d = 0x81;
            o->d3c = g2cc;
            o->b0a = 8;
            *(signed char *)&o->b0f = -7;
            o->subtype = b;
            o->b0c = c;
            o->b1d = 0x4d;
            *(TObj **)&o->d90 = a;
            o->category |= 0x80;
        } else {
            o->w1e = 0x14;
            o->b0d = 0;
            o->d3c = g2d8[0];
            o->b0a = 2;
            o->subtype = b;
            o->b0c = c;
            o->b0f = g47[0] - 1;
            o->category |= 0x80;
            o->b1d = 0x4d;
            *(TObj **)&o->d90 = a;
        }
    }
}
