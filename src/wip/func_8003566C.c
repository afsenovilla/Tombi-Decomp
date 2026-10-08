// FUNC 8003566c 5708 MAIN0
/* score 96 (same size, ~20 instrs differ): case 5 after "o->waa = TB(2) + 0x80" the game reloads waa (lhu) in both odd/even arms and schedules the D_800A6078 load before the waa store; ours reuses the register (tried volatile/raw/unsigned reads and stores, switch, goto, split ifs, -fno-cse-* flags). Case 5 end: waa+-0x40 result lands in a0 instead of v0 (li 6 scheduled first). D_800A6078 must stay a scalar extern (not PL.h). */
#include "TOBJ.H"
#include "raw7.h"

typedef struct { short x, y; } P;
typedef struct {
    unsigned char b0;
    char p1[5];
    unsigned char b6, b7, b8;
    char p9[0x19];
    unsigned short w22;
    char p24[4];
    unsigned short w28, w2a, w2c, w2e;
} G330;
typedef struct {
    char p0[0xc];
    unsigned char bc, bd;
    short we;
    char p10[0x20];
    short w30, w32, w34, w36;
} G338;

extern G330 *D_8009C330;
extern G338 *D_8009C338;
extern TObj D_800A6038;
extern Fix16 *D_800A6078;
extern short D_8007A070[];
extern short D_8007A04C[];
extern short D_8009D600[];
extern unsigned char *D_8009F0EC;
extern unsigned short D_1F8001F8, D_1F8003C8;
extern volatile unsigned short D_8009D670[];
extern unsigned char D_8009C93F, D_8009D2B2;
extern int D_8009C934, D_8009D2E8;

extern int MulCos(int a, short b);
extern int MulNegSinScaled(int a, short b);
extern void SfxPlay2(int, int);
extern void FUN_8001e4f0(int);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern void ObjApplyVelocity(TObj *);
extern short FUN_80035428(TObj *);
extern void FUN_800314a8(TObj *);
extern void FUN_800312e8(TObj *);
extern void FUN_800313c8(TObj *);
extern void FUN_8002c578(TObj *, int, int, int);
extern short FUN_8002078c(P, P);
extern void FUN_80031588(short *, int, unsigned short *, unsigned short *);
extern void FUN_800ec710(short, TObj *);
extern void FUN_800efc04(TObj *);
extern void FUN_80025f40(int, int, int, int);

#define PL D_800A6038
#define TB(k) D_8007A070[(short)(o->animFrame + o->animFrame * 2) + (k)]

#define CALCN() \
    n = o->step * 6; \
    switch (o->animFrame) { \
    case 0: case 1: case 2: case 3: break; \
    case 4: case 5: n += 2; break; \
    case 6: case 7: n += 4; break; \
    }

void func_8003566C(TObj *o)
{
    short n;
    short k;
    short s;
    int t;
    int w;
    P tgt;
    P cur;
    unsigned short xa, xb;

    D_8009C330->w2e = 0xffff;
    switch (o->state) {
    case 0:
        o->animFrame = PL.animFrame;
        PL.b9d = 1;
        D_8009C330->b7 = o->animFrame;
        o->waa = TB(2);
        o->active = 2;
        o->b0a = 0;
        D_8009C338->bc = 0;
        D_8009C330->w22 = 0;
        D_8009D600[1] = 0;
        D_8009C338->w30 = 1;
        D_8009C338->w32 = 0;
        D_8009C338->w34 = 0;
        D_8009C338->w36 = 0;
        o->wa8 = 0;
        if (PL.b9e == 4 || PL.b9e == 7) {
            o->d94 = (int)D_8009F0EC;
            *D_8009F0EC = 2;
        }
        CALCN();
        if (o->animFrame & 1)
            xa = 0xa0 - D_8007A04C[n];
        else
            xa = D_8007A04C[n] - 0x20;
        xb = D_8007A04C[n + 1];
        if (o->animFrame >= 6) {
            s = MulCos((unsigned char)xa, xb);
            t = D_800A6078->p.whole + s;
            o->velX = (o->animFrame & 1) ? t - 4 : t + 4;
            o->velY = PL.y.p.whole + MulNegSinScaled((unsigned char)xa, xb);
        } else {
            o->velX = D_800A6078->p.whole + MulCos((unsigned char)xa, xb);
            o->velY = PL.y.p.whole + MulNegSinScaled((unsigned char)xa, xb);
        }
        o->h->p.whole = o->velX + MulCos((unsigned short)o->waa + 0x80 & 0xff, 0x10);
        o->y.p.whole = o->velY + MulNegSinScaled((unsigned short)o->waa + 0x80 & 0xff, 0x10);
        o->d->p.whole = PL.d->p.whole;
        o->ba5 = 1;
        o->box0 = 6;
        o->box2 = 6;
        o->box1 = 0xc;
        o->box3 = 0xc;
        D_8009C338->bd = 0;
        switch (D_8009C330->b0) {
        case 2:
            o->wa8 = 0x1c00;
            SfxPlay2(3, 2);
            o->w98 = 0;
            if (o->type == 9) {
                o->wa8 = 0x1e00;
                o->w98 = 2;
            }
            break;
        case 1:
            o->wa8 = 0x2000;
            SfxPlay2(3, 8);
            o->w98 = 0;
            if (o->type == 9) {
                o->wa8 = 0x2200;
                o->w98 = 2;
            }
            break;
        }
        o->velH = MulCos(o->waa, o->wa8);
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        D_8009C330->w28 = 0xffff;
        D_8009C330->w2a = 0xffff;
        FUN_800314a8(o);
        AnimJump(&PL, 0);
        s = o->waa;
        o->waa = s + 0x80;
        o->waa = (o->animFrame & 1) ? s + 0x40 : s + 0xc0;
        goto next;
    case 1:
        AnimAdvance(&PL);
        CALCN();
        if (o->animFrame & 1)
            xa = 0xa0 - D_8007A04C[n];
        else
            xa = D_8007A04C[n] - 0x20;
        xb = D_8007A04C[n + 1];
        if (o->animFrame >= 6) {
            s = MulCos((unsigned char)xa, xb);
            t = D_800A6078->p.whole + s;
            o->velX = (o->animFrame & 1) ? t - 4 : t + 4;
            o->velY = PL.y.p.whole + MulNegSinScaled((unsigned char)xa, xb);
        } else {
            o->velX = D_800A6078->p.whole + MulCos((unsigned char)xa, xb);
            o->velY = PL.y.p.whole + MulNegSinScaled((unsigned char)xa, xb);
        }
        o->h->p.whole = o->velX + MulCos((unsigned char)o->waa, 0x10);
        o->y.p.whole = o->velY + MulNegSinScaled((unsigned char)o->waa, 0x10);
        k = o->animFrame + o->animFrame * 2;
        o->waa += D_8007A070[k + 1];
        if (o->animFrame & 1) {
            if (o->waa < D_8007A070[k + 2] + 0x40) {
                FUN_800312e8(o);
                AnimJump(&PL, 0);
                o->waa = D_8007A070[k + 2];
                D_8009C338->bc = 1;
                PL.d8c = 0;
                o->b69 = 0;
                o->timer = 1;
                o->state++;
            }
        } else {
            if (o->waa > D_8007A070[k + 2] + 0xc0) {
                FUN_800312e8(o);
                AnimJump(&PL, 0);
                o->waa = D_8007A070[k + 2];
                D_8009C338->bc = 1;
                PL.d8c = 0;
                o->b69 = 0;
                o->timer = 1;
                o->state++;
            }
        }
        break;
    case 2:
        if (o->timer != 0 && --o->timer <= 0) {
            o->timer = 0;
            o->active = 1;
            o->state++;
        }
    case 3:
        {
            G338 *g = D_8009C338;
            g->w30 += o->wa8 >> 8;
            if (g->w30 > 0x60 - (D_8009C330->b0 << 4))
                g->w30 = 0x60 - (D_8009C330->b0 << 4);
        }
        {
            G338 *g = D_8009C338;
            g->w32 -= (o->wa8 >> 8) * 2;
            if (g->w32 < 0)
                g->w32 = 0;
        }
        o->wa8 -= 0x280;
        o->velH = MulCos(o->waa, o->wa8);
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        ObjApplyVelocity(o);
        if (FUN_80035428(o) == 0) {
            PL.d8c = 0;
            if (o->wa8 < 0x500) {
                o->waa = TB(2);
                o->velX = o->h->p.whole + MulCos((unsigned short)o->waa + 0x80 & 0xff, 0x18);
                o->velY = o->y.p.whole + MulNegSinScaled((unsigned short)o->waa + 0x80 & 0xff, 0x18);
                {
                    G338 *g = D_8009C338;
                    g->bc = 2;
                    g->w32 = 0;
                    D_8009C338->we = 0;
                }
                if (o->visible) {
                    w = o->a.p.whole;
                    FUN_8002c578(o, (short)((o->animFrame & 1) ? w - 0x10 : w + 0x10), o->y.p.whole, o->b.p.whole);
                }
                o->state = 4;
            }
        }
        break;
    case 4:
        {
            G338 *g = D_8009C338;
            g->w30 -= (o->wa8 >> 8) * 2;
            if (g->w30 < 0)
                g->w30 = 0;
        }
        o->h->p.whole = o->velX + MulCos(((unsigned short)o->waa + (unsigned short)D_8009C338->we) & 0xff, 0x18);
        o->y.p.whole = o->velY + MulNegSinScaled(((unsigned short)o->waa + (unsigned short)D_8009C338->we) & 0xff, 0x18);
        D_8009C338->we += TB(1) >> 1;
        PL.d8c = 0;
        if (o->animFrame & 1) {
            unsigned char v = 1;
            if (D_8009C338->we < -0x20)
                v = 2;
            D_8009C338->bd = v;
            if (D_8009C338->we < -0x40) {
                o->waa = (o->waa + D_8009C338->we) & 0xff;
                D_8009C338->bc = 3;
                FUN_800313c8(o);
                AnimJump(&PL, 0);
                D_8009C338->we = (TB(2) - 0x40) & 0xff;
                o->state++;
            }
        } else {
            unsigned char v = 1;
            if (D_8009C338->we > 0x20)
                v = 2;
            D_8009C338->bd = v;
            if (D_8009C338->we > 0x40) {
                o->waa = (o->waa + D_8009C338->we) & 0xff;
                D_8009C338->bc = 3;
                FUN_800313c8(o);
                AnimJump(&PL, 0);
                D_8009C338->we = (TB(2) + 0x40) & 0xff;
                o->state++;
            }
        }
        FUN_80035428(o);
        break;
    case 5:
        if (FUN_80035428(o) == 0) {
            D_8009C338->bd = 0;
            o->wa8 = 0x2000;
            {
                G338 *g = D_8009C338;
                g->w30 += 0x20;
                if (g->w30 > 0x60 - (D_8009C330->b0 << 4))
                    g->w30 = 0x60 - (D_8009C330->b0 << 4);
            }
            {
                G338 *g = D_8009C338;
                g->w32 -= o->wa8 >> 8;
                if (g->w32 < 0)
                    g->w32 = 0;
            }
            if (o->animFrame & 1)
                o->velX = D_800A6078->p.whole + 0x10;
            else
                o->velX = D_800A6078->p.whole - 0x10;
            o->velY = PL.y.p.whole - 0x10;
            tgt.x = o->velX + MulCos(D_8009C338->we, 0xc);
            tgt.y = o->velY + MulNegSinScaled(D_8009C338->we, 0xc);
            cur.x = o->h->p.whole;
            cur.y = o->y.p.whole;
            o->waa = FUN_8002078c(cur, tgt);
            o->velH = MulCos(o->waa, o->wa8);
            o->velV = MulNegSinScaled(o->waa, o->wa8);
            ObjApplyVelocity(o);
            xa = tgt.x - cur.x + 0x10;
            xb = tgt.y - cur.y + 0x10;
            if (xa < 0x20 && xb < 0x20) {
                if (PL.b9e == 4 || PL.b9e == 7)
                    *(unsigned char *)o->d94 = D_8009C330->b6;
                o->waa = TB(2) + 0x80;
                if (o->animFrame & 1) {
                    o->waa = (o->waa + 0x40) & 0xff;
                    o->velX = D_800A6078->p.whole + 0x10;
                } else {
                    o->waa = (o->waa - 0x40) & 0xff;
                    o->velX = D_800A6078->p.whole - 0x10;
                }
                o->velY = PL.y.p.whole - 0x10;
                o->h->p.whole = o->velX + MulCos(o->waa, 0xc);
                o->y.p.whole = o->velY + MulNegSinScaled(o->waa, 0xc);
                D_8009C338->w32 = 0;
                D_8009C338->we = 0;
                D_8009C338->bc = 0;
                PL.b9d = 0;
                o->ba5 = 0;
                if (!(D_8009D670[0] & D_1F8003C8) || PL.b04 != 1 || PL.ba4 != 0 || U8(&PL, 0xac) >= 2 || D_8009C93F != 0) {
                    D_8009C330->b0 = 0;
                    o->b04 = 2;
                    o->step = 0;
                    o->state = 0;
                    D_8009C934 = 0;
                } else {
                    FUN_800ec710(D_8009D2B2, o);
                    w = o->waa;
                    t = w - 0x40;
                    if (!(o->animFrame & 1))
                        t = w + 0x40;
                    o->waa = t;
                    o->state = 6;
                }
            }
        }
        break;
    case 6:
        o->animFrame = PL.animFrame;
        FUN_800314a8(o);
        AnimJump(&PL, 0);
        if ((D_1F8001F8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xc0, 2);
            FUN_8001e4f0(3);
        }
        AnimJump(&PL, (D_8009C330->w22 >> 2) & 3);
        k = o->animFrame + o->animFrame * 2;
        D_8009D600[0] = 10;
        D_8009D600[1] = 2;
        D_8009D600[2] = D_8007A070[k];
        s = o->waa + D_8007A070[k + 1];
        o->waa = s;
        if (s > 0xff)
            o->waa = s - 0x100;
        if (o->waa < 0)
            o->waa += 0x100;
        if (o->animFrame & 1)
            o->d8c = (o->waa + 0x80) & 0xff;
        else
            o->d8c = (unsigned char)o->waa;
        CALCN();
        if (o->animFrame & 1)
            xa = 0x80 - D_8007A04C[n];
        else
            xa = D_8007A04C[n];
        xb = D_8007A04C[n + 1];
        o->velX = D_800A6078->p.whole + MulCos((unsigned char)xa, xb);
        o->velY = PL.y.p.whole + MulNegSinScaled((unsigned char)xa, xb);
        FUN_80031588(D_8009D600, o->waa, &xa, &xb);
        o->h->p.whole = o->velX + xa;
        o->y.p.whole = o->velY + xb;
        if (!(D_8009D670[0] & D_1F8003C8) || PL.b04 != 1 || PL.ba4 != 0 || D_8009C93F != 0) {
            PL.b9d = 1;
            D_8009C330->b0 = 2;
            D_8009C330->w22 = 0;
            o->state = 0;
        } else
            D_8009C330->w22++;
        if (D_8009C330->w22 < 0x3d)
            break;
        D_8009C330->w22 = 0;
    next:
        o->state++;
        break;
    case 7:
        o->animFrame = PL.animFrame;
        FUN_800314a8(o);
        AnimJump(&PL, 0);
        if ((D_1F8001F8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xff, 2);
            SfxPlay2(3, 6);
        }
        AnimJump(&PL, (D_8009C330->w22 / 3) & 3);
        o->animFrame = PL.animFrame;
        k = o->animFrame + o->animFrame * 2;
        D_8009D600[0] = 0x10;
        D_8009D600[1] = 4;
        D_8009D600[2] = D_8007A070[k];
        s = o->waa + D_8007A070[k + 1];
        o->waa = s;
        if (s > 0xff)
            o->waa = s - 0x100;
        if (o->waa < 0)
            o->waa += 0x100;
        if (o->animFrame & 1)
            o->d8c = (o->waa + 0x80) & 0xff;
        else
            o->d8c = (unsigned char)o->waa;
        CALCN();
        if (o->animFrame & 1)
            xa = 0x80 - D_8007A04C[n];
        else
            xa = D_8007A04C[n];
        xb = D_8007A04C[n + 1];
        o->velX = D_800A6078->p.whole + MulCos((unsigned char)xa, xb);
        o->velY = PL.y.p.whole + MulNegSinScaled((unsigned char)xa, xb);
        FUN_80031588(D_8009D600, o->waa, &xa, &xb);
        o->h->p.whole = o->velX + xa;
        o->y.p.whole = o->velY + xb;
        if (!(D_8009D670[0] & D_1F8003C8) || PL.b04 != 1 || PL.ba4 != 0 || D_8009C93F != 0) {
            D_8009C330->b0 = 1;
            D_8009C330->w22 = 0;
            PL.b9d = 1;
            o->state = 0;
        }
        D_8009C330->w22++;
        break;
    }
    if (U8(&PL, 0xac) >= 2) {
        D_8009D2E8 = S32(&PL, 0xe4);
        D_8009C330->b8 = 0;
        PL.step = 0xe;
        D_8009C934 = 0;
        PL.ba5 = 0;
        PL.state = 0;
        PL.substep = 0;
        D_8009C330->w2c = 0xd;
        if (D_8009C330->w2e != 0xd) {
            FUN_800efc04(o);
            AnimJump(o, 0);
            D_8009C330->w2e = D_8009C330->w2c;
        }
        D_8009C330->w2e = 0xffff;
        PL.b9d = 0;
        D_8009C338->bc = 0;
        D_8009C330->b0 = 0;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
    }
}
