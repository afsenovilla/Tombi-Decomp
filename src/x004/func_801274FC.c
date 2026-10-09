// FUNC 801274fc 1560 X004
// MATCHING 801274fc 1560
#include "TOBJ.H"

extern int DAT_1f8002d4[];
extern void *D_80134D0C[];
extern unsigned char D_8009CEA4[], D_8009CEFC[], D_8009CEFB, D_80131168[];
extern unsigned short D_8009C962, D_1F8001F8;
extern int FUN_800202b4(TObj *);
extern void func_8011EC78(TObj *);
extern void FUN_8001e4f0(int);
extern void FUN_800ea544(TObj *, short, short, short);
extern unsigned int FUN_8001f9e0(void);
extern void FUN_80018790(TObj *);

#define SPARKLE(o)                                                         \
    switch ((o)->state) {                                                  \
    case 0:                                                                \
        (o)->state++;                                                      \
        (o)->timer = 8;                                                    \
        (o)->w22 = 0;                                                      \
        (o)->b6a = 1;                                                      \
    case 1:                                                                \
        if (--(o)->timer == -1) {                                          \
            if (++(o)->w22 >= 0xc)                                         \
                (o)->w22 = 0;                                              \
            (o)->w08 = ((D_80131168[(o)->w22] + 0x1e0) << 6) | 0x12;       \
            (o)->timer = 0x10;                                             \
        }                                                                  \
        if (D_8009CEFB == 0x16) {                                          \
            (o)->b0d = 0;                                                  \
            (o)->b6a = 0;                                                  \
            (o)->state++;                                                  \
        }                                                                  \
        break;                                                             \
    case 2:                                                                \
        break;                                                             \
    }

static __inline__ int bit(int n)
{
    int q = n / 8;
    int r = n - q * 8;

    return D_8009CEFC[q] & (1 << r);
}

static __inline__ int is16(TObj *o)
{
    if (o->subtype == 0x16)
        return D_8009CEFB != 0x16;
    return 0;
}

void func_801274FC(TObj *o)
{
    int n, m, k;
    unsigned char *c;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 8;
        o->d3c = DAT_1f8002d4[0];
        if (o->subtype == 0x16) {
            o->b0d = 1;
            o->w08 = 0x7812;
        } else {
            o->b0d = 0;
        }
        switch (o->b0c) {
        case 0:
            if (bit(o->subtype)) {
                o->b04 = 3;
                break;
            }
            o->box0 = 0x14;
            o->box1 = 0x28;
            o->box2 = 0x18;
            o->b6a = 0;
            o->box3 = 0x30;
            o->anim = D_80134D0C[0];
            o->d30 = o->h->p.whole;
            o->d34 = o->y.p.whole;
            o->ba7 = o->b0f;
            break;
        case 1:
        case 2:
            if (o->subtype & 1) {
                if (bit(o->subtype >> 1)) {
                    o->b04 = 3;
                    break;
                }
            }
            o->active = 2;
            o->anim = D_80134D0C[o->b0c];
            if (o->b0c == 1)
                m = 1;
            else
                m = -10;
            o->b0f = m;
            break;
        }
        break;
    case 1:
        if (D_8009C962 < 4)
            func_8011EC78(o);
        else
            FUN_800202b4(o);
        if (o->subtype == 0x16)
            goto sparkle;
        break;
    case 2:
        if (D_8009C962 < 4)
            func_8011EC78(o);
        else
            FUN_800202b4(o);
        switch (o->step) {
        case 0:
            o->step = 1;
            break;
        case 1:
            if (is16(o)) {
            sparkle:
                SPARKLE(o);
            }
            break;
        case 2:
            if (is16(o)) {
                o->active = 1;
                o->b04 = 1;
                o->step = 0;
                o->b0f = o->ba7;
                break;
            }
            FUN_8001e4f0(0xf7);
            FUN_800ea544(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            c = &D_8009CEFB;
            *c = *c + 1;
            n = o->subtype;
            k = n / 8 + 0x58;
            D_8009CEA4[k] |= 1 << (n % 8);
            o->b04 = 3;
            break;
        case 3:
            o->w7a = 10;
            o->b0f = o->ba7;
            o->step++;
            break;
        case 4:
            if (--o->w7a == -1)
                o->step++;
            switch (o->state) {
            case 0:
                o->state++;
                o->timer = 8;
                o->w22 = 0;
                o->b6a = 1;
            case 1:
                if (--o->timer == -1) {
                    if (++o->w22 >= 0xc)
                        o->w22 = 0;
                    o->w08 = ((D_80131168[o->w22] + 0x1e0) << 6) | 0x12;
                    o->timer = 0x10;
                }
                if (D_8009CEFB == 0x16) {
                    o->b0d = 0;
                    o->b6a = 0;
                    o->state++;
                }
                break;
            case 2:
                break;
            }
            o->y.p.whole = o->d34 + (D_1F8001F8 & 1);
            k = 2;
            o->h->p.whole = o->d30 - k + (FUN_8001f9e0() & 3);
            break;
        case 5:
            o->active = 1;
            o->b04 = 1;
            o->step = 0;
            o->y.p.whole = o->d34;
            o->h->p.whole = o->d30;
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
