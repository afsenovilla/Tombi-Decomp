// VERSION SLPS_01144 x14 801189B4 940
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct { short s0, s2; } H;
typedef struct O {
    char p0[4]; unsigned char b4, b5, b6; char p1[0x16 - 7]; short y16; char p3[2]; short z1a;
    char p4a[4]; short s20; char p4b[0x2e - 0x22]; unsigned short w2e; char p4[0x40 - 0x30]; H *h;
    char p5[0x69 - 0x44]; unsigned char b69; char p5b[0x70 - 0x6a]; short s70; char p6[0x7c - 0x72]; short vx, vy;
    char p7[0x8c - 0x80]; int a8c; char p7b[0x9c - 0x90]; unsigned char b9c; char p8[0xac - 0x9d]; unsigned char bac;
    char p9[0xb2 - 0xad]; unsigned short wb2;
} O;
typedef struct { char p[0x2c]; unsigned short w2c, w2e; } E;
extern O *BITTEN_OBJECT;
extern E *D_8009DEE0;
extern unsigned short D_8009F200;
extern short D_8009E4EC, D_8009E4EE;
extern void func_80103138(O *);
extern void func_80103284(O *);
extern void func_800EFD70(O *);
extern void AnimLoadDuration(O *);
extern int AnimAdvance(O *);
extern void func_801049B4(O *);
extern void func_8001E388(int, int);
extern void func_800EED18(O *, int);

#define SPAD16(x) (*(unsigned short *)(0x1F800000 + (x)))

void func_801189B4_slps01144(O *o)
{
    short flag;
    unsigned short t;
    volatile unsigned short *pad;

    switch (o->b6) {
    case 1:
        o->s20++;
        flag = 0;
        o->y16 += 8;
        if (o->y16 + BITTEN_OBJECT->s70 > BITTEN_OBJECT->y16) {
            o->y16 = BITTEN_OBJECT->y16 - BITTEN_OBJECT->s70;
            flag = 1;
        }
        if (o->vx < 0) {
            o->h->s2 -= 8;
            if (o->h->s2 <= BITTEN_OBJECT->h->s2) {
                o->h->s2 = BITTEN_OBJECT->h->s2;
                if (flag) {
                    func_80103138(o);
                    func_80103284(o);
                }
            }
        } else {
            o->h->s2 += 8;
            if (o->h->s2 >= BITTEN_OBJECT->h->s2) {
                o->h->s2 = BITTEN_OBJECT->h->s2;
                if (flag) {
                    func_80103138(o);
                    func_80103284(o);
                }
            }
        }
        if (o->s20 > 10) {
            func_80103138(o);
            func_80103284(o);
        }
        break;
    case 2:
        pad = &D_8009F200;
        *(int *)o->h += D_8009E4EC << 8;
        *(int *)((char *)o + 0x14) += D_8009E4EE << 8;
        if (*pad & 0x8000) o->w2e = 1;
        if (*pad & 0x2000) o->w2e = 0;
        BITTEN_OBJECT->w2e = o->w2e & 1;
        BITTEN_OBJECT->h->s2 = o->h->s2;
        BITTEN_OBJECT->y16 = o->y16 + BITTEN_OBJECT->s70;
        if (SPAD16(0x1fc) & SPAD16(0x3c6)) {
            o->b69 = 0;
            o->wb2 = 0;
            o->vx = 0;
            o->vy = 0;
            if (*pad & 0x4000) D_8009DEE0->w2c = 15;
            else D_8009DEE0->w2c = 14;
            if (D_8009DEE0->w2e != D_8009DEE0->w2c) {
                BITTEN_OBJECT->b6 = 7;
                func_800EFD70(o);
                AnimLoadDuration(o);
                D_8009DEE0->w2e = D_8009DEE0->w2c;
            }
            o->b9c = 0;
            *(volatile short *)&o->vy = 0;
            o->b6 = 3;
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            func_8001E388(0x22, 0x23);
            o->vy = 0;
            func_800EED18(o, 0x1d);
            o->a8c = 0x200;
            o->b6 = 4;
        } else {
            func_801049B4(o);
        }
        break;
    case 4:
        BITTEN_OBJECT->b4 = 2;
        BITTEN_OBJECT->b5 = 3;
        BITTEN_OBJECT->b6 = 0;
        t = o->w2e & 1;
        BITTEN_OBJECT->w2e = t;
        if (*(volatile unsigned short *)&D_8009F200 & 0x4000)
            BITTEN_OBJECT->w2e = t | 2;
        o->b9c = 0;
        o->bac = 0;
        o->b5 = 0x3e;
        o->b6 = 0;
        break;
    }
}
#endif
