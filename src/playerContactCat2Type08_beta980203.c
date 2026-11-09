// VERSION BETA_980203 x00 80129C74 564
#if defined(VERSION_BETA_980203)
// Beta version of src/playerContactCat2Type08.c (playerContactCat2Type08, PAL body): the object link field 0xe4 is at 0xd8.
#define DEBUG_INVINCIBLE D_1F8001A4
#include "TOBJ.H"
#include "raw7.h"
extern unsigned char DEBUG_INVINCIBLE;
extern unsigned char PLAYER[];
extern short objectBoxOverlapSideFacing(TObj *, TObj *);
extern void spawnImpactEffect(int, int, int, int);
extern void decreaseObjectTimer(TObj *, int);

void playerContactCat2Type08(TObj *a, TObj *b)
{
    short r;

    r = objectBoxOverlapSideFacing(a, b);
    if (r == -1)
        return;
    switch ((*(unsigned char*)&a->wac)) {
    case 1:
        if (r < 3) {
            b->b69 = 0;
            b->active = 2;
            b->b04 = 2;
            b->step = 1;
            b->state = 0;
            b->animFrame = a->animFrame & 1;
            S32(a, 0xd8) = (int)b;
            (*(unsigned char*)&a->wac) = 2;
            spawnImpactEffect(2, a->a.p.whole, a->y.p.whole, a->b.p.whole);
            break;
        }
    case 0:
    case 3:
        if (DEBUG_INVINCIBLE)
            return;
        if (b->active & 2)
            return;
        if (a->active & 2)
            return;
        if ((*(unsigned char*)&a->wac) != 3 && r == 3 && b->b6a == 1) {
            if ((*(unsigned char*)&a->wac) == 1)
                (*(unsigned char*)&a->wac) = 0;
            b->active = 4;
            b->b6a = 2;
            a->active = 2;
            PLAYER[4] = 1;
            PLAYER[5] = 0x40;
            PLAYER[6] = 0;
            PLAYER[7] = 0;
            return;
        }
        {
            int ah, bh;
            a->active = 2;
            bh = b->h->p.whole;
            ah = a->h->p.whole;
            a->b04 = 2;
            a->step = 0;
            a->state = 0;
            a->animFrame = ah < bh;
            decreaseObjectTimer(a, 1);
        }
        break;
    case 2:
        if (b->active == 3)
            return;
        b->active = 3;
        if (b->b6a == 2) {
            PLAYER[0] = 1;
            PLAYER[4] = 1;
            PLAYER[5] = 0;
            PLAYER[6] = 0;
        }
        b->b6a = 0;
        b->b04 = 2;
        b->step = 0;
        b->state = 0;
        break;
    }
}
#endif
