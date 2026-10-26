// FUNC 800eeae4 120 X014
// MATCHING 800eeae4 120
typedef struct P { char p0[0x2c]; unsigned short a; unsigned short b; } P;
extern P *PLAYER_EXT;
extern void FUN_800efc04(void *);
extern void FUN_8001fe94(void *, int);
void playerSetAnimIfChangedAt(void *o, unsigned short x, short y)
{
    P *p = PLAYER_EXT;
    p->a = x;
    if (p->b != x) {
        p->a = x;
        FUN_800efc04(o);
        FUN_8001fe94(o, y);
        PLAYER_EXT->b = PLAYER_EXT->a;
    }
}
