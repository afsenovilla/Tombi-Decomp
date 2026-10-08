// FUNC 8011d9d8 856 X000
// MATCHING 8011d9d8 856
#include "TOBJ.H"

#define APPROACH(p, t, l) { unsigned short u = (p)->animFrame; if (u != (t)) { if (u < (l)) (p)->animFrame = u + 1; else (p)->animFrame = u - 1; } }
#define GROW(p) { (p)->animFrame += 2; if ((p)->animFrame >= 0x90) (p)->animFrame = 0x8f; }

void func_8011D9D8(TObj *o)
{
    TObj *a, *b;
    switch (o->step) {
    case 0:
        o->timer = 0;
        o->b6b = 0;
        o->step++;
    case 1:
        a = (TObj *)o->d94;
        b = (TObj *)a->d94;
        switch ((unsigned char)(o->b69 | ((b->b69 << 2) | (a->b69 << 1)))) {
        case 0:
            o->timer = 0;
            APPROACH(o, 0x80, 0x81);
            APPROACH(a, 0x80, 0x81);
            APPROACH(b, 0x80, 0x81);
            break;
        case 1:
            o->timer = 0;
            APPROACH(o, 0x80, 0x81);
            APPROACH(a, 0x80, 0x81);
            APPROACH(b, 0x80, 0x81);
            break;
        case 2:
            o->timer++;
            APPROACH(o, 0x87, 0x87);
            APPROACH(a, 0x87, 0x87);
            APPROACH(b, 0x80, 0x81);
            break;
        case 3:
            o->timer++;
            APPROACH(o, 0x87, 0x87);
            APPROACH(a, 0x87, 0x87);
            APPROACH(b, 0x80, 0x81);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            if (o->timer++ < 10) {
                GROW(o);
                GROW(a);
                GROW(b);
            } else {
                APPROACH(o, 0x87, 0x87);
                APPROACH(a, 0x87, 0x87);
                APPROACH(b, 0x87, 0x87);
            }
            break;
        }
        break;
    }
}
