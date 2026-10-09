// FUNC 80134ee8 636 X001
// MATCHING 80134ee8 636
/* o is reused for list[0] after the sort (o in a2 at entry); subtype goes through a temp t (lbu a1 + move s4).
   The D_1F8002E8/anim loads are block locals declared right after b0d, and w1e is a raw store so the scalar loads
   cannot be hoisted above it (raw store pins later loads; in-struct stores don't). */
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
    *(short *)((char *)o + 0x1e) = 1;
    o->b0d = 0;
    {
        int d = D_1F8002E8;
        void *an = D_8013F134;
        o->step = 1;
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
        o->anim = an;
    }
    o->d94 = (int)list[1];
    for (i = 1; i < n; i++) {
        e = list[i];
        e->active = 2;
        e->type = 0x13;
        *(short *)((char *)e + 0x1e) = 1;
        e->b0d = 0;
        {
            int d = D_1F8002E8;
            void *an = D_8013F1BC;
            e->animFrame = 0;
            e->b04 = 0;
            e->step = 1;
            e->state = 0;
            e->d3c = d;
            e->anim = an;
        }
        e->subtype = o->subtype;
        e->b0c = i;
        e->b0a = o->b0a;
        e->b6a = 0;
        e->b68 = 0;
        e->b69 = 0;
        e->w98 = 2;
        e->w9a = 2;
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
