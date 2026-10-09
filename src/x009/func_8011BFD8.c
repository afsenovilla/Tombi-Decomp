// FUNC 8011bfd8 820 X009
// MATCHING 8011bfd8 820
#include "TOBJ.H"

#define E(o, k) (((TObj **)((char *)(o) + 0xb4))[k])
extern unsigned char D_800A60DA;
extern TObj *ObjAlloc(void);

void func_8011BFD8(TObj *o)
{
    TObj *e;
    unsigned char one, five;

    switch (o->state) {
    case 0:
        if (o->b69 != 1 || D_800A60DA != o->b69)
            break;
        one = 1;
        e = ObjAlloc();
        if (e) {
            e->type = 0x44;
            e->a.raw = 0x07480000;
            e->y.raw = 0xfe200000;
            e->active = one;
            e->b0c = one;
            e->b.raw = 0x0ba40000;
        }
        E(o, 0) = e;
        five = 5;
        e = ObjAlloc();
        if (e) {
            e->type = 0x44;
            e->a.raw = 0x07580000;
            e->y.raw = 0xfe200000;
            e->active = one;
            e->b0c = five;
            e->b.raw = 0x0ba40000;
        }
        E(o, 1) = e;
        e = ObjAlloc();
        if (e) {
            e->type = 0x44;
            e->a.raw = 0x07680000;
            e->y.raw = 0xfe200000;
            e->active = one;
            e->b0c = 0;
            e->b.raw = 0x0ba40000;
        }
        E(o, 2) = e;
        e = ObjAlloc();
        if (e) {
            e->type = 0x44;
            e->a.raw = 0x07780000;
            e->y.raw = 0xfe200000;
            e->active = one;
            e->b0c = 0;
            e->b.raw = 0x0ba40000;
        }
        E(o, 3) = e;
        e = ObjAlloc();
        if (e) {
            e->type = 0x44;
            e->a.raw = 0x07880000;
            e->y.raw = 0xfe200000;
            e->active = one;
            e->b0c = 0;
            e->b.raw = 0x0ba40000;
        }
        E(o, 4) = e;
        e = ObjAlloc();
        if (e) {
            e->type = 0x44;
            e->a.raw = 0x07980000;
            e->y.raw = 0xfe200000;
            e->active = one;
            e->b0c = 0;
            e->b.raw = 0x0ba40000;
        }
        E(o, 5) = e;
        o->state++;
        break;
    case 1:
        if (o->b69 == 1) {
            if (D_800A60DA == 1)
                break;
            E(o, 0)->b04 = 2;
            E(o, 1)->b04 = 2;
            E(o, 2)->b04 = 2;
            E(o, 3)->b04 = 2;
            E(o, 4)->b04 = 2;
            E(o, 5)->b04 = 2;
            o->w22 = 0x1e;
            o->state++;
        }
        E(o, 0)->b04 = 2;
        E(o, 1)->b04 = 2;
        E(o, 2)->b04 = 2;
        E(o, 3)->b04 = 2;
        E(o, 4)->b04 = 2;
        E(o, 5)->b04 = 2;
        o->w22 = 0x1e;
        o->state++;
        break;
    case 2:
        if (--o->w22 == -1) {
            o->state++;
        }
    case 3:
        if (o->b69 != 1 || D_800A60DA != 1) {
            o->state = 0;
        }
        break;
    }
}
