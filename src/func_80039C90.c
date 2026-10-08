// FUNC 80039c90 204 MAIN0
// MATCHING 80039c90 204
#include "TOBJ.H"
typedef struct { char pad[0x8a]; unsigned short w8a; char pad2[0x1190 - 0x8c]; int d1190; int d1194; int d1198; } G;
extern G *D_8009F0F0;
extern TObj *D_8009F2D8[];
extern void func_80113754(o);
extern void func_800ECBBC(o);
extern void func_800E8348(o);

void func_80039C90(void)
{
    G *g = D_8009F0F0;
    TObj *o;
    int b, a;
    o = D_8009F2D8[g->d1190];
    a = g->d1194;
    b = g->d1198;
    if (o != 0) {
        o->w74 = a;
        o->w76 = b;
        switch (o->type & 0x7f) {
        case 0x18:
            func_80113754(o);
            break;
        case 0x19:
            func_800ECBBC(o);
            break;
        case 0x2e:
            func_800E8348(o);
            break;
        }
    }
    g->w8a++;
}
