// FUNC 80134ee8 636 X001
/* score 85: `short n` brings the n copies (s3 -> a0/s2, s0 -> a0). Left: game copies o to a2 at entry and loads
   sub into a1 (copied to s4 later), the n copy for the compare lands in a0 (ours a1); the link loop recomputes
   &list[i] each iteration (no strength reduction) and the p setup store order differs. Tried nested static
   inlines for check/spawn/sort/link (no effect), -fno-strength-reduce (worse), local type brute force. */
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
    TObj *p, *e;
    short n;
    int i;
    unsigned char sub;

    if (o->b0c) return;
    sub = o->subtype;
    n = 4;
    if (!sub) n = 6;
    if (D_1F800238 < n - 1) {
        o->b04 = 3;
        D_8009C948[5]--;
        return;
    }
    list[0] = o;
    for (i = 1; i < n; i++) list[i] = FUN_800183b8();
    insertionSortU32(n, list);
    p = list[0];
    p->active = 2;
    p->type = 0x13;
    p->w1e = 1;
    p->b0d = 0;
    p->step = 1;
    p->w98 = 2;
    p->w9a = 2;
    p->b04 = 0;
    p->state = 0;
    p->subtype = sub;
    p->b0c = 0;
    p->b6a = 0;
    p->b68 = 0;
    p->b69 = 0;
    p->b6b = 0;
    p->animFrame = 0;
    p->box0 = 8;
    p->box1 = 0x10;
    p->box2 = 8;
    p->box3 = 0x10;
    p->d90 = 0;
    p->d3c = D_1F8002E8;
    p->anim = D_8013F134;
    p->d94 = (int)list[1];
    for (i = 1; i < n; i++) {
        e = list[i];
        e->active = 2;
        e->type = 0x13;
        e->w1e = 1;
        e->b0d = 0;
        e->animFrame = 0;
        e->b04 = 0;
        e->step = 1;
        e->state = 0;
        e->d3c = D_1F8002E8;
        e->anim = D_8013F1BC;
        e->b0c = i;
        e->subtype = p->subtype;
        e->b6a = 0;
        e->b68 = 0;
        e->b69 = 0;
        e->w98 = 2;
        e->w9a = 2;
        e->b0a = p->b0a;
        e->a.raw = p->a.raw;
        e->y.raw = p->y.raw;
        e->b.raw = p->b.raw;
        e->box0 = 8;
        e->box1 = 0x10;
        e->box2 = 8;
        e->box3 = 0x10;
        *(TObj **)&e->wa8 = p;
        e->d90 = (int)list[i - 1];
        if (i == n - 1)
            e->d94 = 0;
        else
            e->d94 = (int)list[i + 1];
    }
}
