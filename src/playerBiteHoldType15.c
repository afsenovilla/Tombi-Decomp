// FUNC 80122a18 504 X000
// MATCHING 80122a18 504
// VERSION SLPM_80165 x00 800FCDD4 628
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 80122A18 (FUN_80122a18.c): player animation change inline (PAL calls
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
typedef struct { int raw; } H0;
typedef struct { short s0, s2; } H;
typedef struct O {
    char p0[6]; unsigned char b6; char p1[0x12 - 7]; short x12; char p2[2]; short y16; char p3[2]; short z1a;
    char p4[0x40 - 0x1c]; H *h; char p5[0x70 - 0x44]; short s70; char p6[0x7c - 0x72]; short vx, vy;
    char p7[0x9c - 0x80]; unsigned char b9c; char p8[0xac - 0x9d]; unsigned char bac;
} O;
extern O *DAT_8009d2e8;
extern void FUN_8010eaf8(void);
extern void FUN_8001fd94(O *);
extern void FUN_8001f96c(int, int, int, int);
extern short FUN_8003fd78(O *, int, int);

void FUN_80122a18(O *o)
{
    short flag;
    O *p;
    if (o->b6 != 1) return;
    FUN_8010eaf8();
    *(int *)o->h += o->vx << 8;
    o->vy += 8;
    if (o->vy > 0x680) o->vy = 0x680;
    if (o->vy < -0x680) o->vy = -0x680;
    FUN_8001fd94(o);
    flag = 0;
    o->y16 += 8;
    if (o->y16 + DAT_8009d2e8->s70 >= DAT_8009d2e8->y16) {
        o->y16 = DAT_8009d2e8->y16 - DAT_8009d2e8->s70;
        flag = 1;
    }
    if (o->vx < 0) {
        o->h->s2 -= 8;
        if (o->h->s2 <= DAT_8009d2e8->h->s2) {
            o->h->s2 = DAT_8009d2e8->h->s2;
            if (flag) {
                FUN_800eeb5c(o, 0xd);
                FUN_8001f96c(2, o->x12, o->y16, o->z1a);
            }
        }
    } else {
        o->h->s2 += 8;
        if (o->h->s2 >= DAT_8009d2e8->h->s2) {
            o->h->s2 = DAT_8009d2e8->h->s2;
            if (flag) {
                FUN_800eeb5c(o, 0xd);
                FUN_8001f96c(2, o->x12, o->y16, o->z1a);
            }
        }
    }
    if (FUN_8003fd78(o, 0x10e, 1)) {
        FUN_800eeb5c(o, 0xd);
        o->bac = 3;
        o->b9c = 0;
        o->vx = 0;
        o->vy = 0;
        p = DAT_8009d2e8;
        o->h->s2 = p->h->s2;
        o->y16 = p->y16 - p->s70;
        o->b6 = 2;
    }
}
#else
typedef struct { int raw; } H0;
typedef struct { short s0, s2; } H;
typedef struct O {
    char p0[6]; unsigned char b6; char p1[0x12 - 7]; short x12; char p2[2]; short y16; char p3[2]; short z1a;
    char p4[0x40 - 0x1c]; H *h; char p5[0x70 - 0x44]; short s70; char p6[0x7c - 0x72]; short vx, vy;
    char p7[0x9c - 0x80]; unsigned char b9c; char p8[0xac - 0x9d]; unsigned char bac;
} O;
extern O *DAT_8009d2e8;
extern void playerAirAccelerateX(void);
extern void FUN_8001fd94(O *);
extern void FUN_800eeb5c(O *, int);
extern void spawnImpactEffect(int, int, int, int);
extern short FUN_8003fd78(O *, int, int);

void playerBiteHoldType15(O *o)
{
    short flag;
    O *p;
    if (o->b6 != 1) return;
    playerAirAccelerateX();
    *(int *)o->h += o->vx << 8;
    o->vy += 8;
    if (o->vy > 0x680) o->vy = 0x680;
    if (o->vy < -0x680) o->vy = -0x680;
    FUN_8001fd94(o);
    flag = 0;
    o->y16 += 8;
    if (o->y16 + DAT_8009d2e8->s70 >= DAT_8009d2e8->y16) {
        o->y16 = DAT_8009d2e8->y16 - DAT_8009d2e8->s70;
        flag = 1;
    }
    if (o->vx < 0) {
        o->h->s2 -= 8;
        if (o->h->s2 <= DAT_8009d2e8->h->s2) {
            o->h->s2 = DAT_8009d2e8->h->s2;
            if (flag) {
                FUN_800eeb5c(o, 0xd);
                spawnImpactEffect(2, o->x12, o->y16, o->z1a);
            }
        }
    } else {
        o->h->s2 += 8;
        if (o->h->s2 >= DAT_8009d2e8->h->s2) {
            o->h->s2 = DAT_8009d2e8->h->s2;
            if (flag) {
                FUN_800eeb5c(o, 0xd);
                spawnImpactEffect(2, o->x12, o->y16, o->z1a);
            }
        }
    }
    if (FUN_8003fd78(o, 0x10e, 1)) {
        FUN_800eeb5c(o, 0xd);
        o->bac = 3;
        o->b9c = 0;
        o->vx = 0;
        o->vy = 0;
        p = DAT_8009d2e8;
        o->h->s2 = p->h->s2;
        o->y16 = p->y16 - p->s70;
        o->b6 = 2;
    }
}
#endif
