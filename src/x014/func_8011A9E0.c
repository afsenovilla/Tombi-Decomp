// FUNC 8011a9e0 700 X014
// MATCHING 8011a9e0 700
#include "TOBJ.H"

extern TObj *D_8009C954;
extern int D_1F8002D4[];
extern void *D_80129F94;
extern unsigned char D_8009C93A, D_8009C93F;
extern Fix16 *D_800A6078;
extern unsigned short D_800A604E;
extern short D_8009C944[], D_8009C946[];
extern short D_8007A3F0[], D_8007A5F0[];
extern void FUN_8001fe6c(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern int FUN_800638cc(int);
extern int FUN_800205d8(int, int);

static __inline__ void setup(TObj *p)
{
    D_8009C954 = p;
    p->d3c = D_1F8002D4[0];
    p->anim = D_80129F94;
    FUN_8001fe6c(p);
}

void func_8011A9E0(TObj *o)
{
    int t = o->b04;
    unsigned char s;
    short dx, dy;
    int d, a;

    switch (t) {
    case 0:
        o->active = 2;
        o->b0a = 2;
        o->w1e = 0x10;
        o->b0d = 0x80;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->timer = 0x14;
        o->b04++;
        setup(o);
        break;
    case 1:
        ObjCullRegister(o);
        o->d8c = (o->d8c + 2) & 0xff;
        s = o->step;
        switch (s) {
        case 0:
            if (--o->timer == -1) o->step++;
            break;
        case 1:
            if (D_8009C93A == 1) o->step = s + 1;
            break;
        case 2:
            if (D_8009C93F == 0) {
                dx = D_800A6078->p.whole - o->h->p.whole;
                dy = D_800A604E - o->y.p.whole;
                d = FUN_800638cc(dx * dx + dy * dy);
                if (d >= 11) {
                    if (d > 0xa0) d = 0xa0;
                    d = 0xaa - d;
                    a = FUN_800205d8(dx, dy) & 0xff;
                    o->d38 = a;
                    d <<= 2;
                    D_8009C946[0] = (d * D_8007A3F0[(a + 0x48) & 0xff]) >> 12;
                    D_8009C944[0] = (d * D_8007A5F0[(o->d38 + 0x48) & 0xff]) >> 12;
                    break;
                }
            }
            D_8009C944[0] = 0;
            D_8009C946[0] = 0;
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
