// FUNC 8011a544 384 X003
// MATCHING 8011a544 384
#include "TOBJ.H"

extern unsigned short D_8009C962;
extern void ObjFreeDup(TObj *);
extern void func_80119CE8(TObj *);
extern void func_80119E08(TObj *);
extern void func_80119F90(TObj *);

void func_8011A544(TObj *o)
{
    TObj *e;
    int v;
    char pad[0x48];

    unsigned char s = o->b04;

    v = s;
    switch (v) {
    case 0:
        o->ba4 = 0;
        v = D_8009C962;
        if (v < 0) goto d0;
        if (v < 2) {
            o->active = 2;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->velX = 0;
            e = (TObj *)o->d90;
            if (e) {
                o->d30 = o->a.raw - e->a.raw;
                o->d34 = o->y.raw - e->y.raw;
                o->d38 = o->b.raw - e->b.raw;
                if (o->d94 == 0) {
                    o->box0 = 0x14;
                    o->box1 = 0x28;
                    o->box2 = 0xf;
                    o->box3 = 0x1e;
                    o->active = 1;
                }
            }
        } else {
        d0:
            func_80119CE8(o);
        }
        o->b04++;
        break;
    case 1:
        v = D_8009C962;
        if (v < 0) goto d1;
        if (v < 2) func_80119E08(o);
        else { d1: func_80119F90(o); }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
