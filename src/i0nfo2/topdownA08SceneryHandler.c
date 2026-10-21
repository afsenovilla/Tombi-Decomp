// FUNC 800f1a60 332 I0NFO2
// MATCHING 800f1a60 332
#include "TOBJ.H"
extern struct { unsigned short v; } D_800FEE70;
extern struct { short v; } D_800FEE74;
extern struct { void **v; } D_800FEE78;
extern int D_1F8002C8[];
extern void func_8001FE6C(TObj *o);
extern void func_80018CA4(TObj *o);
extern void func_800187E4(TObj *o);

void topdownA08SceneryHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = D_800FEE70.v;
        o->d3c = D_1F8002C8[D_800FEE74.v];
        o->animFrame = 0;
        o->b0d = 1;
        o->w08 = 0x7FC8;
        o->anim = D_800FEE78.v[o->b0c & 0x7F];
        func_8001FE6C(o);
        o->b6b = 0x80;
        o->b0a = 0xD;
        *(signed char *)&o->b0f = -0xC;
        o->d8c = 0;
        o->category |= 0x80;
        o->b04++;
        if (o->b0c & 0x80) o->b04 = 3;
        break;
    case 1:
        o->visible = 1;
        func_80018CA4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_800187E4(o);
        break;
    }
}
