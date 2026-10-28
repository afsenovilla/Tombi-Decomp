// FUNC 80102a5c 100 X000
// MATCHING 80102a5c 100
// probably: player bite-attach entry: anim 0xD, spawnImpactEffect 2, marks the bitten object D_8009D2E8 (+6 = 2) when its type is 2; unreferenced
// VERSION SLPM_80165 x00 800FB2E8 164
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 80102A5C (playerBiteEntryImpactMark.c): player animation change inline (PAL calls
// PlayerSetAnimIfChanged).
typedef struct { char p_[0x2c]; unsigned short animTimer, animFrame; } AnimP_;
extern AnimP_ *D_8009C330_anim;
extern void func_800EDA04(void *);
extern void func_80020554(void *);
#define FUN_800eeb5c(o, n) {                                                   \
    AnimP_ *p_ = D_8009C330_anim;                                     \
    p_->animTimer = (n);                                              \
    if (p_->animFrame != (n)) {                                       \
        func_800EDA04(o);                                             \
        func_80020554(o);                                             \
        D_8009C330_anim->animFrame = D_8009C330_anim->animTimer;      \
    }                                                                 \
}
typedef struct O { char p0[0x12]; short x; char p1[2]; short y; char p2[2]; short z; char p3[4]; short t; } O;
extern char *BITTEN_OBJECT;
extern void FUN_8001f96c(int, int, int, int);
void playerBiteEntryImpactMark(O *o)
{
    o->t = 0;
    FUN_800eeb5c(o, 0xd);
    FUN_8001f96c(2, o->x, o->y, o->z);
    if (BITTEN_OBJECT[2] == 2)
        BITTEN_OBJECT[6] = 2;
}
#else
typedef struct O { char p0[0x12]; short x; char p1[2]; short y; char p2[2]; short z; char p3[4]; short t; } O;
extern char *BITTEN_OBJECT;
extern void FUN_800eeb5c(O *, int);
extern void spawnImpactEffect(int, int, int, int);
void playerBiteEntryImpactMark(O *o)
{
    o->t = 0;
    FUN_800eeb5c(o, 0xd);
    spawnImpactEffect(2, o->x, o->y, o->z);
    if (BITTEN_OBJECT[2] == 2)
        BITTEN_OBJECT[6] = 2;
}
#endif
