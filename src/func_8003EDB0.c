// FUNC 8003edb0 628 MAIN0
// MATCHING 8003edb0 628
#include "TOBJ.H"
typedef struct { unsigned short a, b; } VP;
extern VP D_8007B380[];
extern unsigned char D_8007A7F0[];
extern void (*D_8007A890[])(TObj *);
extern void FUN_8003c980(TObj *);
extern int ObjCullRegister(TObj *);
extern short TileCollideAt(TObj *, int, int);
extern void ObjApplyVelocity(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_800188e0(TObj *);

void func_8003EDB0(TObj *o)
{
    char pad;
    switch (o->b04) {
    case 0:
        FUN_8003c980(o);
        o->active = 4;
        if (o->velV != 0) {
            o->wb4 = 1;
        } else {
            o->wb4 = 0;
        }
        o->timer = 30;
        o->b04++;
        break;
    case 1:
        if (!ObjCullRegister(o)) break;
        if (o->timer != 0 && --o->timer == 0) {
            o->active = 1;
        }
        if (o->b68 & 1) {
            o->b68 = 0;
            o->wb4 = 1;
            o->velH = D_8007B380[o->animFrame].a;
            o->velV = D_8007B380[o->animFrame].b;
        } else if (o->b69 & 1) {
            o->b69 = 0;
            if (o->velV <= 0x100) {
                o->wb4 = 0;
                o->active = 2;
            } else {
                o->velV = -o->velV / 4;
            }
        } else if (TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 8))) {
            o->active = 2;
            if (o->velV <= 0x100) {
                o->wb4 = 0;
                o->active = 2;
            } else {
                o->velV = -o->velV / 4;
            }
        }
        if (*(unsigned short *)&o->wb4) {
            ObjApplyVelocity(o);
            if (o->velV < 0x800) {
                o->velV += 0x20;
            }
            if (o->y.p.whole - ((short *)&o->d34)[1] >= -7) {
                o->wb4 = 0;
                o->velV = 0;
            }
        }
        AnimAdvance(o);
        break;
    case 2:
        D_8007A890[D_8007A7F0[o->subtype]](o);
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
