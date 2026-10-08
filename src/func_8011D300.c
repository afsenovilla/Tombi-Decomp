// FUNC 8011d300 880 X000
// MATCHING 8011d300 880
#include "TOBJ.H"

#define TO80(x) if ((x)->animFrame != 0x80) { if ((x)->animFrame < 0x81) (x)->animFrame++; else (x)->animFrame--; }
#define TO87(x) if ((x)->animFrame != 0x87) { if ((x)->animFrame < 0x87) (x)->animFrame++; else (x)->animFrame--; }
#define UP(x) (x)->animFrame += 2; if ((x)->animFrame >= 0x90) (x)->animFrame = 0x8f;

void func_8011D300(TObj *o)
{
    TObj *a, *b, *c;
    switch (o->step) {
    case 0:
        o->timer = 0;
        o->b6b = 0;
        o->step++;
    case 1:
        a = ((TObj *)((TObj *)o->d94)->d94);
        b = (TObj *)a->d94;
        c = (TObj *)b->d94;
        switch ((unsigned char)(a->b69 | ((c->b69 << 2) | (b->b69 << 1)))) {
        case 0:
            o->timer = 0;
            TO80(a);
            TO80(b);
            TO80(c);
            break;
        case 1:
            o->timer = 0;
            TO87(a);
            TO80(b);
            TO80(c);
            break;
        case 2:
            o->timer++;
            TO87(a);
            TO80(b);
            TO80(c);
            break;
        case 3:
            o->timer++;
            TO87(a);
            TO80(b);
            TO80(c);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            if (o->timer++ < 10) {
                UP(a);
                UP(b);
                UP(c);
            } else {
                TO87(a);
                TO87(b);
                TO87(c);
            }
            break;
        }
        break;
    }
}
