/* --score 8 but only 4 words differ (n53; the previous wip, commit 7fa0811^.., scored 4 with 6 words wrong): only the head of case 0 differs: game `la v1; lw v0,0(v1) [n base]; lw a1,0(v1) [base]`, ours
   `la v0; lw a1 [base]; lw v0 [n base]` (local-alloc ties the load where the la pseudo dies with it; the game's
   la is untied and gets v1, i.e. the second load's dest was not a tie-able local pseudo). What this version fixed
   vs the old wip (6): a volatile READ of o->b04 (`t = *(volatile unsigned char *)&o->b04`, unsigned char t, t++,
   plain store) placed after the two volatile scratchpad reads: volatile reads keep their mutual order, so the lbu
   lands after the lw's and the b04 store order (d84 before b69/b04) comes out right. Tried on top of that: base/n
   read order, base += n, n = *pp; n = *(int*)(n+0x20), pp at function scope (la hoisted), D_1F800334[0] array,
   cast forms, t int/uchar, inc placements, da0 operand order (~700 variants, best 4). Includes csv piece 80116D50.
   n55: deref-first forms (`n = *(int*)(*pp+0x20); base = *pp;`, multi-set n/base, `n + base`) give the game's lw/lw order
   but the la stays in v0 and the deref sinks below lbu/addiu (14); sched1 trace: the deref is a non-struct varying
   load so every later struct store is its successor (ready only after `sh 110`), and li 8 would need to be ready
   after the addiu. Game store order (active before ba4) as source order: 24/55. */
// FUNC 80116d14 320 X011
#include "TOBJ.H"
extern unsigned char D_8009D100;
extern unsigned char D_8009CE3A;
extern int D_1F800334;
extern int ObjCullRegister(TObj *);
extern void addItemToInventory(int, int, int);
extern void FUN_800188e0(TObj *);

void func_80116D14(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009D100 != 0 || D_8009CE3A == 0) {
            o->b04 = 3;
            break;
        }
        {
            volatile int *pp = &D_1F800334;
            int n, base; unsigned char t;
            base = *pp;
            n = *(int *)(*pp + 0x20);
            t = *(volatile unsigned char *)&o->b04;
            t++;
            o->box1 = 0x10;
            o->box3 = 0x10;
            o->ba4 = 0;
            o->box0 = 8;
            o->active = 1;
            o->box2 = 8;
            o->d84 = 0x80;
            o->b69 = 0;
            o->b04 = t;
            o->da0 = base + n;
        }
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        if (D_8009D100 == 0) addItemToInventory(0x5c, 1, 1);
        o->b04++;
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
