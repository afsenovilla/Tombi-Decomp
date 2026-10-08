// FUNC 8010bd08 432 X000
#include "TOBJ.H"
extern TObj *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern unsigned char DAT_8009d2b2;

void FUN_8010bd08(TObj *o)
{
    TObj *q;
    Fix16 *h;
    int x;
    DAT_8009c330->b04 = 0;
    o->timer = 6;
    o->velH = 0;
    o->velV = 0;
    o->velX = 0;
    o->velY = 0;
    switch (DAT_8009f0ec->type) {
    case 5:
        DAT_8009f0ec->b6a = 0;
        break;
    case 0x21:
        DAT_8009f0ec->ba7 = 0;
        break;
    }
    if (DAT_8009d2b2 - 5 >= 4) {
        DAT_8009f0ec->active = DAT_8009c330->state;
    } else {
        switch (DAT_8009f0ec->type) {
        case 11:
            if (DAT_8009f0ec->d94)
                DAT_8009f0ec->active = 3;
            else
                DAT_8009f0ec->active = 1;
        case 21:
            if (DAT_8009f0ec->d94 != 0) {
            DAT_8009f0ec->active = 3;
            q = (TObj *)DAT_8009f0ec->d94;
            while (q->d94) {
                q->active = 3;
                q = (TObj *)q->d94;
            }
            q->active = 1;
            } else {
                DAT_8009f0ec->active = 1;
            }
            break;
        case 29:
        case 49:
            DAT_8009f0ec->active = 1;
            break;
        case 48:
        default:
            DAT_8009f0ec->active = 3;
            break;
        }
    }
    h = o->h;
    x = h->p.whole;
    h->p.whole = !(o->animFrame & 1) ? x - 0x10 : x + 0x10;
}
