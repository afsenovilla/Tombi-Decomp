// FUNC 80134ee8 636 X001
/* score 29 (was 85): o reused for list[0] after the sort (o in a2 at entry); subtype read into a temp t for the
   n choice and copied to sub after list[0] = o (lbu a1 + move s4); D_1F8002E8/anim loaded into block locals declared
   before o->w98 / e->step (loads early, stores late as in the game). Left: store/load scheduling in both blocks
   (w1e/b0d vs loads; e-block w1e/animFrame and the sw 3c/24 position) and a 4-byte size difference (nop). */
#include "TOBJ.H"
extern short D_1F800238;
extern short *D_8009C948;
extern int D_1F8002E8;
extern void *D_8013F134;
extern void *D_8013F1BC;
extern TObj *FUN_800183b8(void);
extern void insertionSortU32(int, TObj **);

void func_80134EE8(TObj *o)
{
    TObj *list[6];
    TObj *e;
    short n;
    int i;
    unsigned char sub;
    unsigned char t;

    if (o->b0c) return;
    t = o->subtype;
    n = 4;
    if (!t) n = 6;
    if (D_1F800238 < n - 1) {
        o->b04 = 3;
        D_8009C948[5]--;
        return;
    }
    list[0] = o;
    sub = t;
    for (i = 1; i < n; i++) list[i] = FUN_800183b8();
    insertionSortU32(n, list);
    o = list[0];
    o->active = 2;
    o->type = 0x13;
    o->w1e = 1;
    o->b0d = 0;
    o->step = 1;
    { int d = D_1F8002E8; void *an = D_8013F134;
    o->w98 = 2;
    o->w9a = 2;
    o->b04 = 0;
    o->state = 0;
    o->subtype = sub;
    o->b0c = 0;
    o->b6a = 0;
    o->b68 = 0;
    o->b69 = 0;
    o->b6b = 0;
    o->animFrame = 0;
    o->box0 = 8;
    o->box1 = 0x10;
    o->box2 = 8;
    o->box3 = 0x10;
    o->d90 = 0;
    o->d3c = d;
    o->anim = an; }
    o->d94 = (int)list[1];
    for (i = 1; i < n; i++) {
        e = list[i];
        e->active = 2;
        e->type = 0x13;
        e->w1e = 1;
        e->b0d = 0;
        e->animFrame = 0;
        e->b04 = 0;
        { int d = D_1F8002E8; void *an = D_8013F1BC;
        e->step = 1;
        e->state = 0;
        e->d3c = d;
        e->anim = an; }
        e->b0c = i;
        e->subtype = o->subtype;
        e->b6a = 0;
        e->b68 = 0;
        e->b69 = 0;
        e->w98 = 2;
        e->w9a = 2;
        e->b0a = o->b0a;
        e->a.raw = o->a.raw;
        e->y.raw = o->y.raw;
        e->b.raw = o->b.raw;
        e->box0 = 8;
        e->box1 = 0x10;
        e->box2 = 8;
        e->box3 = 0x10;
        *(TObj **)&e->wa8 = o;
        e->d90 = (int)list[i - 1];
        if (i == n - 1)
            e->d94 = 0;
        else
            e->d94 = (int)list[i + 1];
    }
}
