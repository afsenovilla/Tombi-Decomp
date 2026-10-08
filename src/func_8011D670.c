// FUNC 8011d670 872 X000
// MATCHING 8011d670 872
#include "TOBJ.H"

#define APPROACH(p, t, l) { unsigned short u = (p)->animFrame; if (u != (t)) { if (u < (l)) (p)->animFrame = u + 1; else (p)->animFrame = u - 1; } }
#define GROW(p) { (p)->animFrame += 2; if ((p)->animFrame >= 0x88) (p)->animFrame = 0x87; }

void func_8011D670(TObj *o)
{
    TObj *a, *b, *c;
    switch (o->step) {
    case 0:
        o->timer = 0;
        o->b6b = 0;
        o->step++;
    case 1:
        a = (TObj *)o->d94;
        b = (TObj *)a->d94;
        c = (TObj *)b->d94;
        switch ((unsigned char)(a->b69 | ((c->b69 << 2) | (b->b69 << 1)))) {
        case 0:
            o->timer = 0;
            APPROACH(a, 0x80, 0x81);
            APPROACH(b, 0x80, 0x81);
            APPROACH(c, 0x80, 0x81);
            break;
        case 1:
            o->timer = 0;
            APPROACH(a, 0x83, 0x83);
            APPROACH(b, 0x80, 0x81);
            APPROACH(c, 0x80, 0x81);
            break;
        case 2:
        case 6:
            o->timer++;
            APPROACH(a, 0x83, 0x83);
            APPROACH(b, 0x83, 0x83);
            APPROACH(c, 0x80, 0x81);
            break;
        case 3:
            o->timer++;
            APPROACH(a, 0x83, 0x83);
            APPROACH(b, 0x83, 0x83);
            APPROACH(c, 0x80, 0x81);
            break;
        case 4:
        case 5:
        case 7:
            if (o->timer++ < 10) {
                GROW(a);
                GROW(b);
                GROW(c);
            } else {
                APPROACH(a, 0x83, 0x83);
                APPROACH(b, 0x83, 0x83);
                APPROACH(c, 0x83, 0x83);
            }
            break;
        }
        break;
    }
}
