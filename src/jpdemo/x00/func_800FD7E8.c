// JPDEMO x00 800FD7E8 168
// Demo version of PAL x000 801043F4 (FUN_801043f4.c): player animation change inline (PAL calls PlayerSetAnimIfChanged). Also calls func_80026800(1, 6).
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
extern void FUN_8001e4f0(int);
extern void FUN_8001f96c(int a, int b, int c, int d);

extern void func_80026800(int, int);
void FUN_801043f4(char *o)
{
    *(short *)(o + 0x20) = 0;
    func_80026800(1, 6);
    FUN_8001e4f0(9);
    PlayerSetAnimIfChanged(o, 0xd);
    FUN_8001f96c(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
    o[6]++;
}
