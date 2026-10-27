// FUNC 800ebc30 684 I0NFO
// MATCHING 800ebc30 684
#include "TOBJ.H"
#include "raw7.h"
extern signed char GAME_PLAYER_MODE;
extern TObj PLAYER;
extern signed char D_800A6047;

#define LST(i) (*(TObj **)((int)list + ((i) << 2)))
void overheadSortDrawOrder(void)
{
    TObj *list[64];
    TObj **pp;
    TObj *o;
    TObj *q;
    short i, j, best;
    short cnt, by;
    short k, m;

    for (i = 0; i < 64; i++) LST(i) = 0;
    i = 0;
    cnt = (k = *(unsigned short *)0x1F800246);
    pp = *(TObj ***)0x1F80021C;
    while (k != 0) {
        TObj *o = *pp++;
        k--;
        if (!(o->type == 0x19 || (unsigned)(o->type - 0x25) < 2)) { cnt--; continue; }
        if (o->_pad0e[0] != 0) { cnt--; continue; }
        LST(i++) = o;
    }
    k = *(unsigned short *)0x1F80024A;
    pp = *(TObj ***)0x1F800220;
    cnt += k;
    while (k != 0) {
        TObj *o = *pp++;
        k--;
        if (!(o->type == 0x2E || o->type == 0x1D)) { cnt--; continue; }
        if (o->_pad0e[0] != 0) { cnt--; continue; }
        LST(i++) = o;
    }
    if (GAME_PLAYER_MODE != 3) {
        cnt++;
        LST(i) = &PLAYER;
    }
    for (m = 0; m < cnt; m++) {
        best = m;
        by = LST(m)->y.p.whole;
        for (i = m; i < cnt; i++) {
            if (LST(i)->y.p.whole > by) {
                best = i;
                by = LST(i)->y.p.whole;
            }
        }
        q = list[m];
        list[m] = list[best];
        list[best] = q;
    }
    for (i = 0; LST(i) != 0; i++) {
        o = LST(i);
        o->b0f = i - 10;
        if (((TObj *)o->d94) != 0) ((TObj *)o->d94)->b0f = i - 9;
    }
    if (GAME_PLAYER_MODE == 3) D_800A6047 = -0xC;
}
