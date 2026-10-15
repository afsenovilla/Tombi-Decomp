// JPDEMO x00 800FCA2C 812
// Demo version of PAL x000 80103D40 (FUN_80103d40.c): d-pad bits 0x1000-0x8000 (PAL 0x10-0x80). player animation
// change inline (PAL calls PlayerSetAnimIfChanged). No angle copy, no position follow in state 2, fixed pad bit 0x40.
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
typedef struct { short s0, s2; } H;
typedef struct O {
    char p0[5]; unsigned char b5, b6; char p1[0x12 - 7]; short x12; char p2[2]; short y16; char p3[2]; short z1a;
    char p4a[4]; short s20; char p4b[0x2e - 0x22]; unsigned short w2e; char p4[0x40 - 0x30]; H *h;
    char p5[0x69 - 0x44]; unsigned char b69; char p5b[0x70 - 0x6a]; short s70; char p6[0x7c - 0x72]; short vx, vy;
    char p7[0x8c - 0x80]; int a8c; char p7b[0x9c - 0x90]; unsigned char b9c; char p8[0xac - 0x9d]; unsigned char bac;
    char p9[0xb2 - 0xad]; unsigned short wb2;
} O;
extern O *DAT_8009d2e8;
extern O *DAT_8009c330;
extern unsigned short DAT_8009d670;
extern unsigned short D_1f8003c6, D_1f8001fc;
extern void FUN_8010eaf8(O *);
extern void FUN_8001fd94(O *);
extern void playerSetAnim(O *, int, int);
extern short FUN_8003fd78(O *, int, int);

void FUN_80103d40(O *o)
{
    short flag;
    O *p;
    volatile unsigned short *pad;
    switch (o->b6) {
    case 1:
        o->s20++;
        FUN_8010eaf8(o);
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
                }
            }
        } else {
            o->h->s2 += 8;
            if (o->h->s2 >= DAT_8009d2e8->h->s2) {
                o->h->s2 = DAT_8009d2e8->h->s2;
                if (flag) {
                    FUN_800eeb5c(o, 0xd);
                }
            }
        }
        if (o->b69 != 0 || FUN_8003fd78(o, (short)(DAT_8009d2e8->y16 - o->y16 - o->s70), 1) || o->s20 > 10) {
            o->s20 = 0;
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
        break;
    case 2:
        pad = &DAT_8009d670;
        if (*pad & 0x8000) o->w2e = 1;
        if (*pad & 0x2000) o->w2e = 0;
        if (D_1f8001fc & 0x40) {
            o->b9c = 1;
            o->b69 = 0;
            o->wb2 = 0;
            o->b5 = 0xf;
            o->b6 = 0;
        }
        break;
    }
}
