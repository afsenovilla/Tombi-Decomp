// FUNC 800eeae4 120 X000
// MATCHING 800eeae4 120
// VERSION SLPM_80165 x00 800EBFA8 120
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 800EEAE4 (playerSetAnimIfChangedAt.c): SLPS_01144 C of src/playerSetAnimIfChangedAt.c (playerSetAnimIfChangedAt, twin score 1.020), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct P { char p0[0x2c]; unsigned short a; unsigned short b; } P;
extern P *PLAYER_EXT;
extern void FUN_800efc04(void *);
extern void FUN_8001fe94(void *, int);
void playerSetAnimIfChangedAt(void *o, unsigned short x, short y)
{
    P *p = PLAYER_EXT;
        p->a = x;
    p->a = x;
    if (p->b != x) {
        FUN_800efc04(o);
        FUN_8001fe94(o, y);
        PLAYER_EXT->b = PLAYER_EXT->a;
    }
}
#else
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
#endif
