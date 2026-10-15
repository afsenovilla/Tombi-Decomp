// FUNC 801229d8 64 X000
// MATCHING 801229d8 64
// VERSION SLPM_80165 x00 800FCD58 124
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 801229D8 (playerHitAnimWithImpact.c): player animation change inline (PAL calls
// PlayerSetAnimIfChanged).
typedef struct { char p_[0x2c]; unsigned short animTimer, animFrame; } AnimP_;
extern AnimP_ *D_8009C330_anim;
extern void func_800EDA04(void *);
extern void func_80020554(void *);
#define PlayerSetAnimIfChanged(o, n) {                                                   \
    AnimP_ *p_ = D_8009C330_anim;                                     \
    p_->animTimer = (n);                                              \
    if (p_->animFrame != (n)) {                                       \
        func_800EDA04(o);                                             \
        func_80020554(o);                                             \
        D_8009C330_anim->animFrame = D_8009C330_anim->animTimer;      \
    }                                                                 \
}
extern void FUN_8001f96c(int a, int b, int c, int d);

void playerHitAnimWithImpact(char *o)
{
    PlayerSetAnimIfChanged(o, 0xd);
    FUN_8001f96c(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
}
#else
extern void PlayerSetAnimIfChanged(void *o, int a);
extern void spawnImpactEffect(int a, int b, int c, int d);

void playerHitAnimWithImpact(char *o)
{
    PlayerSetAnimIfChanged(o, 0xd);
    spawnImpactEffect(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
}
#endif
