// FUNC 80118e00 380 X009
// MATCHING 80118e00 380
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);

void func_80118E00(TObj *o)
{
    TObj *n;

    switch (o->state) {
    case 0:
        o->state++;
        o->timer = 0x3c;
        break;
    case 1:
        if (--o->timer == -1)
            o->state++;
        break;
    case 2:
        n = FUN_800183b8();
        if (n) {
            n->active = 1;
            n->type = 0x2c;
            n->h->raw = o->h->raw - 0x160000;
            n->y.raw = o->y.raw;
            n->d->raw = o->d->raw - 0x1a0000;
            n->subtype = 1;
            n->d8c = 0xc0;
        }
        n = FUN_800183b8();
        if (n) {
            n->active = 1;
            n->type = 0x2c;
            n->h->raw = o->h->raw + 0xe0000;
            n->y.raw = o->y.raw;
            n->d->raw = o->d->raw - 0x160000;
            n->subtype = 0;
            n->d8c = 0x40;
        }
        o->state = 0;
        break;
    }
}
