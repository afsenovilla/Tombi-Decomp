// FUNC 800ebc5c 624 I0NFO3
#include "TOBJ.H"
extern signed char D_8009D2B0;
extern TObj D_800A6038;

void func_800EBC5C(void)
{
    TObj *list[64];
    TObj **pp;
    TObj *o, *t;
    TObj *q;
    short i, j, n, best, by;
    short cnt;
    short k, m;
    unsigned char b;

    for (i = 0; i < 64; i++) list[i] = 0;
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
            list[i++] = o;
        } while (k != 0);
    }
    if (D_8009D2B0 != 3) {
        cnt++;
        list[i] = &D_800A6038;
    }
    for (m = 0; m < cnt; m++) {
        best = m;
        by = list[m]->y.p.whole;
        for (j = m; j < cnt; j++) {
            if (by < list[j]->y.p.whole) {
                best = j;
                by = list[j]->y.p.whole;
            }
        }
        t = list[m];
        list[m] = list[best];
        list[best] = t;
    }
    for (i = 0; list[i] != 0; i++) {
        o = list[i];
        q = (TObj *)o->d94;
        *(signed char *)&o->b0f = i - 10;
        if (q != 0) *(signed char *)&q->b0f = i - 9;
    }
    if (D_8009D2B0 == 3) *(signed char *)&D_800A6038.b0f = -0xC;
}
