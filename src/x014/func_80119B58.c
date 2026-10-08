// FUNC 80119b58 2368 X014
// MATCHING 80119b58 2368
#include "TOBJ.H"
typedef struct { unsigned char b0, b1, b2, b3, b4, b5, b6, b7; } S8;
extern S8 *D_8009C948;
extern unsigned short D_8009C962[];
extern short D_80125C40[], D_80125C50[];
extern unsigned char D_80125C60[], D_80125C68[], D_80125684[];
extern unsigned char D_8009C93E;
extern unsigned char D_800A60D6;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_800A4555;
extern unsigned char D_8009D2C3;
extern void *D_80129F7C[], *D_80129F80[], *D_80129F8C[];
extern short D_8007A1F0[], D_8007A5F0[];
extern int D_1F800168, D_1F80016C[];
extern short D_1F80016A, D_1F8000EE;
extern int D_1F80018C, D_1F800190;
extern int func_80119A4C(TObj *, S8 *);
extern int func_80119914(TObj *, S8 *);
extern int func_801196A4(TObj *, unsigned char);
extern void func_801178CC(TObj *, int, int, int);
extern void FUN_8001e4f0(int);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001d63c(int);
extern void FUN_8001f110(int);
extern void FUN_80026e0c(int, int);

#define turn(o, T)                                  \
    {                                               \
        int n = o->d8c;                             \
        unsigned char d = (T) - n;                  \
        if (d != 0) {                               \
            if (d < 0x81) {                         \
                if (d < 2) o->d8c = n + 1;          \
                else o->d8c = n + 2;                \
            } else {                                \
                if (d < 0xff) o->d8c = n - 2;       \
                else o->d8c = n - 1;                \
            }                                       \
            o->d8c = *(unsigned char *)&o->d8c;     \
        }                                           \
    }

static __inline__ void setAnim(TObj *p, void *a)
{
    p->anim = a;
    FUN_8001fe6c(p);
}

#define MOVE(o)                                                     \
    o->velY = (o->velH * D_8007A1F0[o->wb0]) >> 12;                 \
    o->velX = (o->velH * D_8007A5F0[o->wb0]) >> 12;                 \
    o->h->raw += o->velX << 8;                                      \
    o->y.raw += o->velY << 8;

#define ROT(o) o->wb0 = (unsigned char)func_801196A4(o, (unsigned short)o->wb0 >> 4) << 4

#define MSG(o)                      \
    if (D_800A60D6) {               \
        D_800A60D6 = 0;             \
        D_800A603C = 5;             \
        D_800A603D = 0;             \
        D_800A603E = 0;             \
    }

void func_80119B58(TObj *o)
{
    S8 *s = D_8009C948;
    int v;
    unsigned int u;

    switch (o->state) {
    case 0:
        if (!func_80119A4C(o, s)) break;
        FUN_8001e4f0(D_80125C40[D_8009C962[0]]);
        o->velH = 0x500;
        o->state++;
        s->b0 = 2;
        s->b4 = 2;
        s->b5 = 4;
        s->b6 = 0;
        s->b7 = 0;
        o->b6a = 1;
        break;
    case 1:
        turn(o, *(unsigned char *)&o->d38);
        if (!func_80119914(o, s)) break;
        o->b0b = 1;
        o->b0f = 4;
        o->state++;
        D_8009C93E = 0;
        MSG(o);
        s->b4 = 2;
        s->b1 = 0;
        s->b5 = 4;
        s->b6 = 1;
        o->ba5 = 0x40;
        setAnim(o, D_80129F7C[0]);
        break;
    case 2:
        u = o->ba5 + 2;
        if (u >= 0x80) *(signed char *)&o->ba5 = -0x80;
        else o->ba5 = u;
        turn(o, *(unsigned char *)&o->d38);
        if (FUN_8001fec0(o)) {
            o->velH = 0x500;
            *(signed char *)&o->ba5 = -0x80;
            o->state++;
            o->wb0 = (o->wb0 + 0x80) & 0xff;
            setAnim(o, D_80129F8C[0]);
        }
        MSG(o);
        break;
    case 3:
        MSG(o);
        u = o->ba5 + 2;
        if (u >= 0x100) *(signed char *)&o->ba5 = -1;
        else o->ba5 = u;
        FUN_8001fec0(o);
        MOVE(o);
        o->velH += 0x40;
        if (o->velH > 0x1500) {
            o->velH = 0x1500;
            o->timer = 0x5a;
            *(signed char *)&o->ba5 = -1;
            o->state++;
        }
        ROT(o);
        break;
    case 4:
        FUN_8001fec0(o);
        MOVE(o);
        ROT(o);
        if (--o->timer == -1) o->state++;
        break;
    case 5:
        {
            short w = o->ba5 - 1;
            if (w <= 0) o->ba5 = 0;
            else o->ba5 = w;
        }
        FUN_8001fec0(o);
        MOVE(o);
        ROT(o);
        o->velH -= 0x40;
        if (o->velH < 0x400) {
            o->state++;
            if (D_1F80016A > D_1F8000EE) v = -0x10;
            else v = 0x10;
            o->velH = (D_1F800168 + v - o->h->raw) >> 15;
            v = D_1F80016C[0];
            o->timer = 0x80;
            o->wb0 = 0x40;
            *(signed char *)&o->b0f = -9;
            {
                int w = o->y.raw + 8;
                v -= w;
                o->velV = v >> 15;
            }
            FUN_8001d63c(D_80125684[D_8009C962[0]]);
        }
        break;
    case 6:
        {
            short w = o->ba5 - 2;
            if (w <= 0) o->ba5 = 0;
            else o->ba5 = w;
        }
        turn(o, *(unsigned char *)&o->wb0);
        FUN_8001fec0(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        if (--o->timer == 0) {
            o->timer = 0x3c;
            o->ba5 = 0;
            o->ba6 = 0;
            o->ba7 = 1;
            o->state++;
            func_801178CC(o, 0, 0, 0);
        }
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        break;
    case 7:
        u = o->ba5 + 2;
        if (u >= 0x100) *(signed char *)&o->ba5 = -1;
        else o->ba5 = u;
        o->ba6 = o->ba5;
        turn(o, *(unsigned char *)&o->wb0);
        FUN_8001fec0(o);
        if (--o->timer == 0) {
            FUN_8001e4f0(D_80125C50[D_8009C962[0]]);
            o->timer = 0x3c;
            o->state++;
            setAnim(o, D_80129F80[0]);
        }
        break;
    case 8:
        u = o->ba5 + 2;
        if (u >= 0x100) {
            o->anim = 0;
            *(signed char *)&o->ba5 = -1;
        } else o->ba5 = u;
        o->ba6 = o->ba5;
        if (--o->timer == -1) {
            unsigned char *p;
            o->state++;
            o->anim = 0;
            FUN_8001f110(1);
            {
                int n = D_80125C68[D_8009C962[0]];
                p = &D_8009D2C3;
                D_800A4555 = 4;
                *p |= 1 << n;
            }
            FUN_80026e0c(D_80125C60[D_8009C962[0]], 1);
        }
        break;
    case 9:
        o->anim = 0;
        break;
    }
}
