// FUNC 8002c578 220 MAIN0
#include "TOBJ.H"
extern TObj *ObjAlloc(void);
extern short *DAT_8009c338;
extern int DAT_1f8002cc;

void FUN_8002c578(TObj *p, int x, int y, int z)
{
    TObj *o = ObjAlloc();
    if (o != 0) {
        short *t;
        short w;
        o->active = 1;
        o->type = 0x12;
        t = DAT_8009c338;
        w = p->animFrame;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->animFrame = w;
        o->w1e = t[4];
        w = t[5];
        o->b0a = 8;
        *(signed char *)&o->b0f = -28;
        o->b0d = 1;
        o->subtype = 0;
        o->w08 = w;
        o->category |= 0x80;
        o->b1d = 0x4d;
        *(TObj **)((char *)o + 0x90) = p;
        o->d3c = DAT_1f8002cc;
    }
}
