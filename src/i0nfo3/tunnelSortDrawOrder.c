// FUNC 800ebc5c 624 I0NFO3
// MATCHING 800ebc5c 624
#include "TOBJ.H"
extern signed char GAME_PLAYER_MODE;
extern TObj PLAYER;

#define LST(i) (*(TObj **)((int)list + ((i) << 2)))
void tunnelSortDrawOrder(void)
{
    TObj *list[64];
    TObj **pp;
    TObj *o;
    TObj *q;
    short i, j, n, best, by;
    short cnt;
    short k, m;
    unsigned char b;

    for (i = 0; i < 64; i++) LST(i) = 0;
    i = 0;
    cnt = *(unsigned short *)0x1F800246;
    pp = *(TObj ***)0x1F80021C;
    k = cnt;
    if (k != 0) {
        do {
            TObj *o = *pp++;
            k--;
            if (o->type == 0x44 || o->type == 0x45 || o->type == 0x4C) goto sw;
        fail:
            cnt--;
            continue;
        sw:
            b = *(unsigned char *)((char *)o + 0xE);
            if (b == 0) goto c0;
            if (b == 1) goto c1;
            if (b == 2) goto c2;
            goto fail;
        c1:
            *(signed char *)&o->b0f = -0xC;
            goto fail;
        c2:
            *(signed char *)&o->b0f = -1;
            goto fail;
        c0:
            LST(i++) = o;
        } while (k != 0);
    }
    if (GAME_PLAYER_MODE != 3) {
        cnt++;
        LST(i) = &PLAYER;
    }
    for (m = 0; m < cnt; m++) {
        best = m;
        by = LST(m)->y.p.whole;
        for (j = m; j < cnt; j++) {
            if (LST(j)->y.p.whole > by) {
                best = j;
                by = LST(j)->y.p.whole;
            }
        }
        q = list[m];
        list[m] = list[best];
        list[best] = q;
    }
    for (i = 0; LST(i) != 0; i++) {
        o = LST(i);
        q = (TObj *)o->d94;
        *(signed char *)&o->b0f = i - 10;
        if (q != 0) *(signed char *)&q->b0f = i - 9;
    }
    if (GAME_PLAYER_MODE == 3) *(signed char *)&PLAYER.b0f = -0xC;
}
