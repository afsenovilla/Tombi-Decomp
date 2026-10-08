// FUNC 8003a26c 108 MAIN0
// MATCHING 8003a26c 108
#include "TOBJ.H"
typedef struct { char pad[0x8a]; unsigned short w8a; char pad2[0x1190 - 0x8c]; int d1190; int d1194; int d1198; int d119c; } G;
extern G *D_8009F0F0;
extern TObj *D_8009F2D8[];

void func_8003A26C(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) {
        o->h->p.whole = g->d1194;
        o->y.p.whole = g->d1198;
        o->d->p.whole = g->d119c;
    }
    g->w8a++;
}
