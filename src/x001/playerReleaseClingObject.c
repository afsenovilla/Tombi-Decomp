// FUNC 8010bd08 432 X001
// MATCHING 8010bd08 432
#include "TOBJ.H"
extern TObj *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern unsigned char DAT_8009d2b2;

void playerReleaseClingObject(TObj *o)
{
    TObj *q;
    Fix16 *h;
    int x;
    PLAYER_EXT->b04 = 0;
    o->timer = 6;
    o->velH = 0;
    o->velV = 0;
    o->velX = 0;
    o->velY = 0;
    switch (GRABBED_OBJECT->type) {
    case 5:
        GRABBED_OBJECT->b6a = 0;
        break;
    case 0x21:
        GRABBED_OBJECT->ba7 = 0;
        break;
    }
    if (DAT_8009d2b2 - 5 >= 4) {
        GRABBED_OBJECT->active = PLAYER_EXT->state;
    } else {
        switch (GRABBED_OBJECT->type) {
        case 11:
            if (GRABBED_OBJECT->d94)
                GRABBED_OBJECT->active = 3;
            else
                GRABBED_OBJECT->active = 1;
        case 21:
            if (GRABBED_OBJECT->d94 != 0) {
            GRABBED_OBJECT->active = 3;
            q = (TObj *)GRABBED_OBJECT->d94;
            if (q->d94) goto loop;
            q->active = 1;
            break;
        loop:
            q->active = 3;
            q = (TObj *)q->d94;
            if (q->d94) goto loop;
            q->active = 1;
            } else {
                GRABBED_OBJECT->active = 1;
            }
            break;
        case 29:
        case 49:
            GRABBED_OBJECT->active = 1;
            break;
        case 48:
        default:
            GRABBED_OBJECT->active = 3;
            break;
        }
    }
    h = o->h;
    x = h->p.whole;
    h->p.whole = !(o->animFrame & 1) ? x - 0x10 : x + 0x10;
}
