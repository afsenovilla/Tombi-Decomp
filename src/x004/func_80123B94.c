// FUNC 80123b94 1244 X004
// MATCHING 80123b94 1244
#include "TOBJ.H"

extern unsigned char D_8009D2C3;
extern unsigned short D_8009C962;
extern void *D_80134D38[];
extern unsigned short D_80131108[];
extern unsigned char D_800A6038;
extern int func_8012747C(int);
extern void func_8011EC78(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80124070(TObj *);
extern void FUN_8012423c(TObj *);
extern void FUN_80124ad8(TObj *);
extern void FUN_80124e14(TObj *);
extern void FUN_80018790(TObj *);

static __inline__ int nearCam(TObj *o)
{
    short dv;
    if ((unsigned short)(o->d->p.whole - *(unsigned short *)0x1F800172 + 45) >= 91) return 0;
    dv = *(unsigned short *)0x1F80016E - o->y.p.whole;
    if (dv >= -31 || (unsigned short)(dv + 172) >= 173) return 0;
    return (unsigned short)(o->h->p.whole - *(unsigned short *)0x1F80016A + 16) < 33;
}

void func_80123B94(TObj *o)
{
    unsigned short *b;
    int r;
    unsigned char k = o->b04;

    switch (k) {
    case 0:
        if (!(D_8009D2C3 & 8) && D_8009C962 < 4) {
            if (!func_8012747C(o->b0c)) break;
            o->step = 4;
            o->state = 0;
            o->b04++;
        } else {
            o->b04 = k + 1;
            o->step = 0;
            o->state = 0;
        }
        o->w1e = 8;
        if (o->b0c == 1) o->w98 = o->b6b;
        o->animFrame = 1;
        o->b0d = 0;
        o->b0a = 2;
        o->anim = D_80134D38[o->subtype];
        b = &D_80131108[o->subtype * 4];
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
        o->d34 = o->y.p.whole;
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
        o->b0f = o->w7a;
        break;
    case 1:
        if (D_8009C962 < 4) func_8011EC78(o);
        else ObjCullRegister(o);
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
            FUN_80124070(o);
            if (o->b0c == 7 && o->subtype == 0) o->visible = 0;
            break;
        case 1:
            FUN_8012423c(o);
            break;
        case 2:
            FUN_80124ad8(o);
            if (o->b0c == 7 && o->subtype == 0) o->visible = 0;
            break;
        case 3:
            FUN_80124e14(o);
            break;
        case 4:
            if (o->b0c == 7 && o->subtype == 0) o->visible = 0;
            switch (o->state) {
            case 0:
                o->state++;
                o->timer = 0x20;
                if (o->b0c == 7) o->y.p.whole += 0x1c;
                else o->y.p.whole += 0x30;
                break;
            case 1:
                if (--o->timer == -1) {
                    o->state++;
                    if (o->b0c == 7) o->timer = 0x38;
                    else o->timer = 0x60;
                }
                break;
            case 2:
                o->y.raw += -0x8000;
                if (--o->timer == -1) {
                    o->step = 0;
                    o->state = 0;
                }
                break;
            }
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
