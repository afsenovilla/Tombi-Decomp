// FUNC 8003e960 848 MAIN0
// MATCHING 8003e960 848
#include "TOBJ.H"
typedef struct { short h; short v; } VT;
extern VT D_8007B374[];
extern unsigned char D_8007A7F0[];
extern void (*D_8007A890[])(TObj *);
extern unsigned short D_8009C962;
extern void FUN_8003c980(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjApplyVelocity(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern void AnimAdvance(TObj *);
extern void FUN_80020c04(int);
extern void FUN_800188e0(TObj *);

void func_8003E960(TObj *o)
{
    int k;
    switch (o->b04) {
    case 0:
        FUN_8003c980(o);
        o->timer = 30;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        k = 8;
        if (o->subtype == 2 && o->b0c >= 4)
            k = 0x1c;
        if (ObjCullRegister(o) == 0)
            break;
        switch (o->step) {
        case 0:
            ObjApplyVelocity(o);
            if (o->velV <= 0x800)
                o->velV += 0x20;
            if (o->b68 & 1) {
                o->b68 = 0;
                o->velH = D_8007B374[o->animFrame].h;
                o->velV = D_8007B374[o->animFrame].v;
            } else if (o->b69 & 1) {
                o->b69 = 0;
                if (o->velV <= 0x100) {
                    o->active = 2;
                    o->step++;
                } else {
                    o->velV = -o->velV / 4;
                }
            } else if (o->velV >= 0) {
                if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + k)) {
                    if (o->velV <= 0x100) {
                        o->active = 2;
                        o->step++;
                    } else {
                        o->velV = -o->velV / 4;
                    }
                }
            }
            if (o->subtype == 0x94 && D_8009C962 == 3 && o->y.p.whole >= -0x3bf) {
                o->y.p.whole = -0x3c0;
                o->active = 2;
                o->step++;
            }
            break;
        case 1:
            if (o->b68 & 1) {
                o->b68 = 0;
                o->velH = D_8007B374[o->animFrame].h;
                o->velV = D_8007B374[o->animFrame].v;
                o->step = 0;
            }
            break;
        }
        AnimAdvance(o);
        if (o->timer && --o->timer == 0)
            o->active = 1;
        break;
    case 2:
        if (o->b0c & 0x80) {
            FUN_80020c04(o->b6b);
            o->b0c &= 0x7f;
        }
        D_8007A890[D_8007A7F0[o->subtype]](o);
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
