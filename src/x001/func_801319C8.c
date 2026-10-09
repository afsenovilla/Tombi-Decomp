// FUNC 801319c8 732 X001
// MATCHING 801319c8 732
/* Matching debt: register asm("$4") for the o->da0 pointer before the loop (local-alloc gives v0; tried temp
   vars, a shared d with the step-0 call, volatile/raw reads, statement permutations). */
#include "TOBJ.H"

typedef struct {
    int tag;
    short x0, y0;
    int c1;
    short x1, y1;
    int c2;
    short x2, y2;
    char pad[0x28 - 0x18];
} P40;

extern char D_80077CDC[];
extern P40 D_800E3E28[];
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern int D_1F800334;
extern void FUN_80018d2c(TObj *);
extern void FUN_80025aa8(int, P40 *);
extern int FUN_800202b4(TObj *);
extern void FUN_8001888c(TObj *);

void func_801319C8(TObj *o)
{
    TObj *p;
    P40 *e;
    int n, k;

    switch (o->b04) {
    case 0:
        p = (TObj *)o->d90;
        o->movetab = D_80077CDC;
        o->b04++;
        o->a.p.whole = p->a.p.whole;
        o->y.p.whole = p->y.p.whole;
        o->b.p.whole = p->b.p.whole;
        o->da0 = *(volatile int *)&D_1F800334 + ((int *)*(volatile int *)&D_1F800334)[11];
        o->ba4 = 1;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->visible == 0)
            break;
        o->visible = 1;
        FUN_80018d2c(o);
        o->a.p.whole = p->a.p.whole;
        o->y.p.whole = p->y.p.whole;
        o->b.p.whole = p->b.p.whole;
        o->d8c = p->d8c;
        break;
    case 2:
        switch (o->step) {
        case 0:
            o->b0a = 0x12;
            o->d8c = 0;
            *(P40 **)&o->wa8 = D_800E3E28;
            o->velV = -0x400;
            FUN_80025aa8(o->da0, D_800E3E28);
            o->timer = 60;
            o->step++;
        case 1:
            k = 0;
            e = D_800E3E28;
            {
                register int *dp asm("$4"); /* debt: game keeps the o->da0 pointer in a0 */
                dp = (int *)o->da0;
                n = dp[4];
            }
            do {
                if (e->x0 < 0) {
                    e->x0--;
                    e->x1--;
                    e->x2--;
                    e->y0 += k >> 16;
                    e->y1 += k >> 16;
                    e->y2 += k >> 16;
                } else {
                    e->x0++;
                    e->x1++;
                    e->x2++;
                    e->y0 += k >> 16;
                    e->y1 += k >> 16;
                    e->y2 += k >> 16;
                }
                k += 0x2000;
                e++;
            } while (--n != 0);
            o->velV += 0x28;
            if (o->velV > 0x400)
                o->velV = 0x400;
            o->y.raw += o->velV << 8;
            if (o->timer >= 0x1e || ((D_1F8001F8 + D_1F800198) & 1))
                FUN_800202b4(o);
            if (--o->timer == -1)
                o->b04++;
            break;
        }
        break;
    case 3:
        FUN_8001888c(o);
        break;
    }
}
