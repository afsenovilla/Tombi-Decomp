// JPDEMO x00 800FD890 724
// Demo version of PAL x000 80104454 (FUN_80104454.c): player animation change inline (PAL calls
// PlayerSetAnimIfChanged). Also calls func_80026800(1, 6) on landing.
typedef struct { char p_[0x2c]; unsigned short animTimer, animFrame; } AnimP_;
extern AnimP_ *D_8009C330_anim;
extern extern void func_80026800(int, int);
void func_800EDA04(void *);
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
    char p4a[4]; short s20; char p4[0x40 - 0x22]; H *h; char p5[0x70 - 0x44]; short s70; char p6[0x7c - 0x72]; short vx, vy;
    char p7[0x9c - 0x80]; unsigned char b9c; char p8[0xac - 0x9d]; unsigned char bac;
} O;
extern O *DAT_8009d2e8;
extern void FUN_8010eaf8(void);
extern void FUN_8001fd94(O *);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_8001e4f0(int);

void FUN_80104454(O *o)
{
    short flag;
    if (o->b6 != 1) return;
    o->s20++;
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
                o->s20 = 0;
                func_80026800(1, 6);
                FUN_8001e4f0(9);
                FUN_800eeb5c(o, 0xd);
                FUN_8001f96c(2, o->x12, o->y16, o->z1a);
                o->b6++;
            }
        }
    } else {
        o->y16 += 8;
        if (o->y16 + DAT_8009d2e8->s70 >= DAT_8009d2e8->y16) {
            o->y16 = DAT_8009d2e8->y16 - DAT_8009d2e8->s70;
            flag++;
        }
        o->h->s2 += 8;
        if (o->h->s2 >= DAT_8009d2e8->h->s2) {
            o->h->s2 = DAT_8009d2e8->h->s2;
            if (flag) {
                o->s20 = 0;
                func_80026800(1, 6);
                FUN_8001e4f0(9);
                FUN_800eeb5c(o, 0xd);
                FUN_8001f96c(2, o->x12, o->y16, o->z1a);
                o->b6++;
            }
        }
    }
    if (o->s20 > 10) {
        o->s20 = 0;
        func_80026800(1, 6);
        FUN_8001e4f0(9);
        FUN_800eeb5c(o, 0xd);
        FUN_8001f96c(2, o->x12, o->y16, o->z1a);
        o->b6++;
    }
}
