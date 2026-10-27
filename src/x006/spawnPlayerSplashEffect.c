// FUNC 800ef05c 244 X006
// MATCHING 800ef05c 244
#include "TOBJ.H"
extern short spawnPlayerFootstepEffect(void);
extern TObj *FUN_80018448(void);
extern short FREE_OBJECT_COUNT;
extern unsigned short TICK_COUNT;
static short DAT_801151e0[32] = {
    0, -6, -6, 0, 0, 6, 6, 0, -4, -4, -4, 4, 4, 4, 4, -4,
    0, -3, -3, 0, 0, 3, 3, 0, 0, -3, -3, 0, 0, 3, 3,
};
void spawnPlayerSplashEffect(unsigned int a, short x, short y, short z)
{
    TObj *p;
    short *t;
    if (spawnPlayerFootstepEffect() == 0 && FREE_OBJECT_COUNT > 5 && (p = FUN_80018448()) != 0) {
        p->active = 1;
        p->type = 0x31;
        p->subtype = 1;
        p->a.p.whole = x;
        p->y.p.whole = y;
        p->b.p.whole = z;
        t = DAT_801151e0 + (a & 1) * 16 + (TICK_COUNT & 7) * 2;
        p->h->p.whole += t[0];
        p->y.p.whole += t[1];
    }
}
