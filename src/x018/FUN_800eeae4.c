// FUNC 800eeae4 120 X018
// MATCHING 800eeae4 120
typedef struct P { char p0[0x2c]; unsigned short a; unsigned short b; } P;
extern P *DAT_8009c330;
extern void FUN_800efc04(void *);
extern void FUN_8001fe94(void *, int);
void FUN_800eeae4(void *o, unsigned short x, short y)
{
    P *p = DAT_8009c330;
    p->a = x;
    if (p->b != x) {
        p->a = x;
        FUN_800efc04(o);
        FUN_8001fe94(o, y);
        DAT_8009c330->b = DAT_8009c330->a;
    }
}
