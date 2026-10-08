// FUNC 8002c654 232 MAIN0
// MATCHING 8002c654 232
#include "TOBJ.H"
extern unsigned char DAT_8009cef8;
extern TObj *ObjAlloc(void);
extern unsigned short GetClut(int, int);
extern unsigned short DAT_800a6066[];
extern int DAT_800a6048[];
extern unsigned char DAT_800a6047[];
extern int DAT_1f8002cc;

void FUN_8002c654(void)
{
    TObj *o;
    int v;

    if (DAT_8009cef8 != 0 && (o = ObjAlloc()) != 0) {
        o->active = 1;
        o->type = 0x12;
        o->animFrame = DAT_800a6066[0] & 1;
        o->a.raw = DAT_800a6048[0];
        o->y.raw = DAT_800a6048[1];
        o->b.raw = DAT_800a6048[2];
        o->w1e = 0;
        o->w08 = GetClut(0x80, 499);
        o->b0d = 1;
        o->b0a = 8;
        o->subtype = 1;
        o->b0f = DAT_800a6047[0] + 1;
        v = DAT_1f8002cc;
        o->category |= 0x80;
        o->b1d = 0x4d;
        o->d3c = v;
    }
}
