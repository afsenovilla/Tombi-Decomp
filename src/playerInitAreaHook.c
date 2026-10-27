// FUNC 801228bc 212 X000
// MATCHING 801228bc 212
#include "TOBJ.H"
extern unsigned short GAME_SELECTED_SECTION;
extern int GAME_AREA00_EVENT_CONTROL;
extern unsigned char DAT_8009cda6;
extern unsigned char DAT_8009d2af;
extern int GRABBED_OBJECT;
extern void spawnFrog(int, int, int);
extern void spawnA00Tornado(int, int, int, int);

void playerInitAreaHook(TObj *o)
{
    unsigned short *p = &GAME_SELECTED_SECTION;
    if (*p == 4)
        o->b0f = 8;
    if (*p == 5)
        *(signed char *)&o->b0f = -5;
    if ((GAME_AREA00_EVENT_CONTROL & 2) != 0)
        spawnFrog(o->a.p.whole, o->y.p.whole, o->b.p.whole);
    if (DAT_8009cda6 != 0xff)
        spawnA00Tornado(0, 0, 0, 6);
    if (DAT_8009d2af == 0) {
        o->b04 = 5;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
    }
    GRABBED_OBJECT = 0;
}
