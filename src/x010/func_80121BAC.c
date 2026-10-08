// FUNC 80121bac 780 X010
// MATCHING 80121bac 780
#include "TOBJ.H"

extern void *D_80131CC8[];
extern unsigned short D_8012F378[];
extern unsigned char D_800A6047;
extern unsigned char D_800A6038;
extern int ObjCullRegister(TObj *);
extern void FUN_80121eb8(TObj *);
extern void FUN_80122084(TObj *);
extern void FUN_80122804(TObj *);
extern void FUN_80122b40(TObj *);
extern void FUN_80018790(TObj *);

static __inline__ int nearCam(TObj *o)
{
    short dv;
    if ((unsigned short)(o->d->p.whole - *(unsigned short *)0x1F800172 + 45) >= 91) return 0;
    dv = *(unsigned short *)0x1F80016E - o->y.p.whole;
    if (dv >= -31 || (unsigned short)(dv + 172) >= 173) return 0;
    return (unsigned short)(o->h->p.whole - *(unsigned short *)0x1F80016A + 16) < 33;
}

void func_80121BAC(TObj *o)
{
    unsigned short *b;
    int r;

    switch (o->b04) {
    case 0:
        o->w1e = 12;
        o->animFrame = 1;
        o->b0d = 0;
        o->b0a = 2;
        o->anim = D_80131CC8[o->subtype];
        b = &D_8012F378[o->subtype * 4];
        o->d3c = ((struct { char pad[0x2d4]; int v; } *)0x1F800000)->v;
        o->box0 = *b++;
        o->box1 = *b++;
        o->box2 = b[0];
        o->box3 = b[1];
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b6b = 0;
        o->b6a = 0;
        o->b69 = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        o->d34 = o->y.p.whole;
        o->b04++;
        switch (o->subtype) {
        case 0:
            o->w7a = 2;
            break;
        case 1:
            o->w7a = -2;
            break;
        case 2:
            o->w7a = -1;
            o->active = 2;
            break;
        }
        break;
    case 1:
        o->b0f = D_800A6047 + *(unsigned char *)&o->w7a;
        ObjCullRegister(o);
        if (o->step == 0) {
            r = nearCam(o);
            if (r) {
                if (D_800A6038 == 1) {
                    o->step = 1;
                    o->state = 0;
                }
            } else if (o->b68) {
                o->step = 2;
                o->state = 0;
            }
        }
        switch (o->step) {
        case 0:
            FUN_80121eb8(o);
            break;
        case 1:
            FUN_80122084(o);
            break;
        case 2:
            FUN_80122804(o);
            break;
        case 3:
            FUN_80122b40(o);
            break;
        }
        o->b68 = 0;
        o->b69 = 0;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
