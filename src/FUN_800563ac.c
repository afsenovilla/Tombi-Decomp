// FUNC 800563ac 116 MAIN0
// MATCHING 800563ac 116
typedef struct O { char p0[0x1e]; short v1e; char p1[0x9e]; unsigned short a, b, c, d; char p2[4]; unsigned short e; } O;
typedef struct L { short x0, x1, x2, x3; short a, b, c, d; short m; } L;
extern void FUN_800591c0(int);
extern void FUN_80056420(L *, int, int);
void FUN_800563ac(O *o)
{
    char pad[0x60]; L l; L *p = &l;
    FUN_800591c0(o->e);
    p->m = -1;
    p->a = o->a;
    p->b = o->b;
    p->c = o->c;
    p->d = o->d;
    FUN_80056420(p, 0, o->v1e);
}
