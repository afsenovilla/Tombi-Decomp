// FUNC 80132760 516 X001
/* score 9: (o38: sched1 dump shows lbu boosted after sll and picked before the stores; game needs lbu unready until sw movetab is picked (a dep lbu->sw40) plus lower luid; tried volatile read/store, raw store, q/inline bases, do{}while(0) barrier (hoists lbu to block top), one temp for both indexes: none) only the order of 'lbu subtype' vs 'la movetab; sw movetab' at +0x74 differs (game loads subtype first, then la movetab into v1). Tried: statement permutations, early index/pointer temps, volatile subtype read, raw-offset movetab store, table pointer local. o20: it is regalloc, not sched: game gives subtype v0 and movetab la v1 (overlapping), local-alloc priority (2 refs/short life) always gives the la v0 first; maybe the la pseudo was global (multi-block). Tried 400 random orders with m=D_80077CF4 temp, off<<2 index, b0f/timer store forms. */
#include "TOBJ.H"
typedef struct { short a, b; } S2;
extern S2 D_8013C9C0[];
extern void *D_8013E6E4[];
extern int D_1F8002D4[];
extern char D_80077CF4[];
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern void AnimLoadDuration(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern int FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void func_80132760(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        *(signed char *)&o->b0f = -20;
        o->b0a = 2;
        o->timer = 0x3c;
        o->movetab = D_80077CF4;
        o->d8c = 0;
        o->b0d = 0;
        o->animFrame = 0;
        { S2 *t = D_8013C9C0; S2 *p = &t[o->subtype]; o->w22 = p->a; o->velV = p->b; }
        o->w1e = 10;
        o->d3c = D_1F8002D4[0];
        o->anim = D_8013E6E4[o->subtype];
        AnimLoadDuration(o);
        break;
    case 1:
        o->velV += 0x28;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->w22 & 1) o->d8c = (unsigned char)(o->d8c + 8);
        else o->d8c = (unsigned char)(o->d8c - 8);
        if (o->w22 != 2) FUN_8001fa88(o, o->w22);
        if (o->timer >= 0x1e || ((D_1F8001F8 + D_1F800198) & 1)) FUN_800202b4(o);
        if (--o->timer == -1) o->b04 = 2;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
