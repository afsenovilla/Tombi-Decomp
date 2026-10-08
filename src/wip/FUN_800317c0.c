// FUNC 800317c0 6188 MAIN0
/* wip by b0: ncheck score ~1400, 1556 vs 1547 insns. Structure per case is right; left: register/scheduling diffs (velX store vs next-call arg loads from pt on the stack, clamp recompute "0x60 - b00*16" in case 3/5, case 5 waa reload, case 6 branch layout). Key fixes so far: X = DAT_800a6038 struct (game derives &X from field addresses), separate m index var in IDX macro, D600..D604 as arrays, PL->b00 goto layout. */
#include "TOBJ.H"
typedef struct {
    unsigned char b00, b01, p02[4], b06, b07, b08;
    unsigned char p09[0x22 - 9];
    unsigned short w22;
    unsigned char p24[4];
    unsigned short w28, w2a, w2c, w2e;
} PL;
typedef struct P { short x, y; } P;
typedef struct { short a, b, c; } E6;
extern PL *DAT_8009c330;
extern short *DAT_8009c338;
extern TObj DAT_800a6038;
#define X DAT_800a6038
#define U8X(n) (((unsigned char *)&DAT_800a6038)[n])
extern TObj *DAT_8009f0ec;
extern TObj *DAT_800a611c;
extern TObj *DAT_8009d2e8;
extern short DAT_8009d600[], DAT_8009d602[], DAT_8009d604[];
extern unsigned char DAT_8009d2b2, DAT_8009c93f;
extern unsigned short DAT_8009d670;
extern unsigned short DAT_1f8003c8, DAT_1f800282, DAT_1f8001f8;
extern short DAT_8007a070[], DAT_8007a072[], DAT_8007a074[];
extern short DAT_8007a04c[], DAT_8007a04e[];
extern short MulCos(int, int);
extern short MulNegSinScaled(int, int);
extern void ObjApplyVelocity(TObj *);
extern short FUN_800411cc(TObj *, int, int);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_800eaffc(int, int, int, int);
extern void SfxPlay(int);
extern void SfxPlay2(int, int);
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern void FUN_800314a8(TObj *);
extern void FUN_800312e8(TObj *);
extern void FUN_800313c8(TObj *);
extern void FUN_80031588(short *, int, short *, short *);
extern void FUN_8002c578(TObj *, int, int, int);
extern short FUN_8002078c(P, P);
extern void FUN_800ec710(int, TObj *);
extern void FUN_80025f40(int, int, int, int);
extern void FUN_800efc04(TObj *);

#define B338(n) (((unsigned char *)DAT_8009c338)[n])
#define AF3(o) ((short)((o)->animFrame * 3))

#define IDX(o, k) \
    k = (o)->step * 6; \
    switch ((o)->animFrame) { \
    case 0: case 1: case 2: case 3: break; \
    case 4: case 5: k += 2; break; \
    case 6: case 7: k += 4; break; \
    }

#define BTN() ((*(volatile unsigned short *)&DAT_8009d670 & DAT_1f8003c8) && X.b04 == 1 && X.ba4 == 0)

void FUN_800317c0(TObj *o)
{
    P b, a;
    P pt;
    char pad[12];
    int k;
    int m;
    short *e;
    short *q;
    short t;
    unsigned short u, w;

    DAT_8009c330->w2e = 0xffff;
    switch (o->state) {
    case 0:
        o->animFrame = X.animFrame;
        X.b9d = 1;
        DAT_8009c330->b07 = o->animFrame;
        t = DAT_8007a074[AF3(o)];
        o->b0a = 0;
        o->active = 2;
        o->waa = t;
        B338(0xc) = 0;
        DAT_8009c330->w22 = 0;
        DAT_8009d602[0] = 0;
        DAT_8009c338[0x18] = 1;
        DAT_8009c338[0x19] = 0;
        DAT_8009c338[0x1a] = 0;
        DAT_8009c338[0x1b] = 0;
        o->wa8 = 0;
        if (X.b9e == 4 || X.b9e == 7) {
            o->d94 = (int)DAT_8009f0ec;
            DAT_8009f0ec->active = 2;
        }
        IDX(o, m);
        if (o->animFrame & 1)
            pt.x = 0xa0 - DAT_8007a04c[(short)m];
        else
            pt.x = DAT_8007a04c[(short)m] - 0x20;
        pt.y = DAT_8007a04e[(short)m];
        if (o->animFrame >= 6) {
            k = X.h->p.whole + MulCos((unsigned char)pt.x, pt.y);
            o->velX = (o->animFrame & 1) ? k - 4 : k + 4;
        } else {
            o->velX = X.h->p.whole + MulCos((unsigned char)pt.x, pt.y);
        }
        o->velY = X.y.p.whole + MulNegSinScaled((unsigned char)pt.x, pt.y);
        o->h->p.whole = o->velX + MulCos(((unsigned short)o->waa + 0x80) & 0xff, 0x10);
        o->y.p.whole = o->velY + MulNegSinScaled(((unsigned short)o->waa + 0x80) & 0xff, 0x10);
        o->d->p.whole = X.d->p.whole;
        o->ba5 = 1;
        o->box0 = 6;
        o->box2 = 6;
        o->box1 = 0xc;
        o->box3 = 0xc;
        B338(0xd) = 0;
        if (DAT_8009c330->b00 == 1)
            goto one;
        if (DAT_8009c330->b00 == 2) {
            o->wa8 = 0x1400;
            SfxPlay2(3, 2);
            o->w98 = 1;
        }
        goto skip98;
    one:
        o->wa8 = 0x1900;
        SfxPlay2(3, 8);
        o->w98 = 2;
    skip98:
        o->velH = MulCos(o->waa, o->wa8);
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        DAT_8009c330->w28 = 0xffff;
        DAT_8009c330->w2a = 0xffff;
        FUN_800314a8(o);
        AnimJump(&X, 0);
        o->waa += 0x80;
        o->state++;
        break;
    case 1:
        AnimAdvance(&X);
        IDX(o, m);
        if (o->animFrame & 1)
            pt.x = 0xa0 - DAT_8007a04c[(short)m];
        else
            pt.x = DAT_8007a04c[(short)m] - 0x20;
        pt.y = DAT_8007a04e[(short)m];
        if (o->animFrame >= 6) {
            k = X.h->p.whole + MulCos((unsigned char)pt.x, pt.y);
            o->velX = (o->animFrame & 1) ? k - 4 : k + 4;
        } else {
            o->velX = X.h->p.whole + MulCos((unsigned char)pt.x, pt.y);
        }
        o->velY = X.y.p.whole + MulNegSinScaled((unsigned char)pt.x, pt.y);
        o->h->p.whole = o->velX + MulCos((unsigned char)o->waa, 0x10);
        o->y.p.whole = o->velY + MulNegSinScaled((unsigned char)o->waa, 0x10);
        k = AF3(o);
        o->waa += DAT_8007a072[k];
        if (o->animFrame & 1) {
            e = &DAT_8007a072[k + 1];
            if (o->waa < *e + 0x40) {
                FUN_800312e8(o);
                o->waa = *e;
                B338(0xc) = 1;
                X.d8c = 0;
                o->timer = 1;
                o->state++;
            }
        } else {
            e = &DAT_8007a072[k + 1];
            if (o->waa > *e + 0xc0) {
                FUN_800312e8(o);
                o->waa = *e;
                B338(0xc) = 1;
                X.d8c = 0;
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
        DAT_8009c338[0x18] += o->wa8 >> 8;
        if (DAT_8009c338[0x18] > 0x60 - DAT_8009c330->b00 * 16)
            DAT_8009c338[0x18] = 0x60 - DAT_8009c330->b00 * 16;
        DAT_8009c338[0x19] -= (o->wa8 >> 8) * 2;
        if (DAT_8009c338[0x19] < 0)
            DAT_8009c338[0x19] = 0;
        o->wa8 -= 0x200;
        o->velH = MulCos(o->waa, o->wa8);
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        ObjApplyVelocity(o);
        if (o->active == 1 && FUN_800411cc(o, o->h->p.whole, o->y.p.whole) != 0) {
            FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            u = DAT_1f800282;
            k = u >> 5 & 0xf;
            if ((u & 0x3000) == 0 && k < 4 && k != 0)
                FUN_800eaffc(o->h->p.whole, o->y.p.whole, o->d->p.whole, (unsigned char)o->animFrame);
            SfxPlay(5);
            o->wa8 = 0x4ff;
        }
        X.d8c = 0;
        if (o->wa8 >= 0x500)
            break;
        o->waa = DAT_8007a074[AF3(o)];
        o->velX = o->h->p.whole + MulCos(((unsigned short)o->waa + 0x80) & 0xff, 0x18);
        o->velY = o->y.p.whole + MulNegSinScaled(((unsigned short)o->waa + 0x80) & 0xff, 0x18);
        q = DAT_8009c338;
        ((unsigned char *)q)[0xc] = 2;
        q[0x19] = 0;
        DAT_8009c338[7] = 0;
        if (o->visible != 0) {
            k = o->a.p.whole;
            if (o->animFrame & 1)
                k -= 0x10;
            else
                k += 0x10;
            FUN_8002c578(o, (short)k, o->y.p.whole, o->b.p.whole);
        }
        o->state++;
        break;
    case 4:
        DAT_8009c338[0x18] -= (o->wa8 >> 8) * 2;
        if (DAT_8009c338[0x18] < 0)
            DAT_8009c338[0x18] = 0;
        o->h->p.whole = o->velX + MulCos((o->waa + DAT_8009c338[7]) & 0xff, 0x18);
        o->y.p.whole = o->velY + MulNegSinScaled((o->waa + DAT_8009c338[7]) & 0xff, 0x18);
        DAT_8009c338[7] += DAT_8007a072[AF3(o)] >> 1;
        X.d8c = 0;
        if (o->animFrame & 1) {
            B338(0xd) = DAT_8009c338[7] < -0x20 ? 2 : 1;
            if (DAT_8009c338[7] < -0x40) {
                o->waa = (o->waa + DAT_8009c338[7]) & 0xff;
                B338(0xc) = 3;
                FUN_800313c8(o);
                AnimJump(&X, 0);
                DAT_8009c338[7] = (DAT_8007a072[AF3(o) + 1] - 0x40) & 0xff;
                o->ba5 = 0;
                o->active = 2;
                o->state++;
            }
        } else {
            B338(0xd) = DAT_8009c338[7] > 0x20 ? 2 : 1;
            if (DAT_8009c338[7] > 0x40) {
                o->waa = (o->waa + DAT_8009c338[7]) & 0xff;
                B338(0xc) = 3;
                FUN_800313c8(o);
                AnimJump(&X, 0);
                DAT_8009c338[7] = (DAT_8007a072[AF3(o) + 1] + 0x40) & 0xff;
                o->ba5 = 0;
                o->active = 2;
                o->state++;
            }
        }
        break;
    case 5:
        B338(0xd) = 0;
        o->wa8 = 0x1400;
        DAT_8009c338[0x18] += 0x14;
        if (DAT_8009c338[0x18] > 0x60 - DAT_8009c330->b00 * 16)
            DAT_8009c338[0x18] = 0x60 - DAT_8009c330->b00 * 16;
        DAT_8009c338[0x19] -= o->wa8 >> 8;
        if (DAT_8009c338[0x19] < 0)
            DAT_8009c338[0x19] = 0;
        if (o->animFrame & 1)
            o->velX = X.h->p.whole + 0x10;
        else
            o->velX = X.h->p.whole - 0x10;
        o->velY = X.y.p.whole - 0x10;
        b.x = o->velX + MulCos(DAT_8009c338[7], 0xc);
        b.y = o->velY + MulNegSinScaled(DAT_8009c338[7], 0xc);
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        o->waa = FUN_8002078c(a, b);
        o->velH = MulCos(o->waa, o->wa8);
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        ObjApplyVelocity(o);
        pt.x = b.x - a.x + 0x10;
        pt.y = b.y - a.y + 0x10;
        if ((unsigned short)pt.x >= 0x20 || (unsigned short)pt.y >= 0x20)
            break;
        o->waa = DAT_8007a074[AF3(o)] + 0x80;
        if (o->animFrame & 1) {
            t = X.h->p.whole;
            o->waa = (o->waa + 0x40U) & 0xff;
            t += 0x10;
        } else {
            t = X.h->p.whole;
            o->waa = (o->waa - 0x40U) & 0xff;
            t -= 0x10;
        }
        o->velX = t;
        o->velY = X.y.p.whole - 0x10;
        o->h->p.whole = o->velX + MulCos(o->waa, 0xc);
        o->y.p.whole = o->velY + MulNegSinScaled(o->waa, 0xc);
        DAT_8009c338[0x19] = 0;
        DAT_8009c338[7] = 0;
        B338(0xc) = 0;
        X.b9d = 0;
        o->ba5 = 0;
        o->state++;
        break;
    case 6:
        if (o->animFrame & 1)
            o->velX = X.h->p.whole + 0x10;
        else
            o->velX = X.h->p.whole - 0x10;
        o->velY = X.y.p.whole - 0x10;
        AnimAdvance(&X);
        DAT_8009c338[7] += DAT_8007a072[AF3(o)];
        if (o->animFrame & 1) {
            if (DAT_8009c338[7] < -0x40) {
                if (X.b9e == 4 || X.b9e == 7)
                    ((TObj *)o->d94)->active = DAT_8009c330->b06;
                if (BTN() && U8X(0xac) < 2 && DAT_8009c93f == 0) {
                    FUN_800ec710(DAT_8009d2b2, o);
                    X.b9d = 0;
                    DAT_8009c338[7] = 0;
                    o->waa -= 0x40;
                    o->state++;
                } else {
                    goto fail;
                }
            }
        } else {
            if (DAT_8009c338[7] > 0x40) {
                if (X.b9e == 4 || X.b9e == 7)
                    ((TObj *)o->d94)->active = DAT_8009c330->b06;
                if (BTN() && U8X(0xac) < 2 && DAT_8009c93f == 0) {
                    FUN_800ec710(DAT_8009d2b2, o);
                    X.b9d = 0;
                    DAT_8009c338[7] = 0;
                    o->waa += 0x40;
                    o->state++;
                } else {
                fail:
                    X.b9d = 0;
                    B338(0xc) = 0;
                    DAT_8009c330->b00 = 0;
                    o->b04 = 2;
                    o->step = 0;
                    o->state = 0;
                }
            }
        }
        o->h->p.whole = o->velX + MulCos((o->waa + DAT_8009c338[7]) & 0xff, 0xc);
        o->y.p.whole = o->velY + MulNegSinScaled((o->waa + DAT_8009c338[7]) & 0xff, 0xc);
        break;
    case 7:
        o->animFrame = X.animFrame;
        FUN_800314a8(o);
        AnimJump(&X, 0);
        if ((DAT_1f8001f8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xc0, 2);
            SfxPlay(3);
        }
        AnimJump(&X, DAT_8009c330->w22 >> 2 & 3);
        k = AF3(o);
        DAT_8009d600[0] = 10;
        DAT_8009d602[0] = 2;
        DAT_8009d604[0] = DAT_8007a070[k];
        o->waa += DAT_8007a072[k];
        if (o->waa >= 0x100)
            o->waa -= 0x100;
        if (o->waa < 0)
            o->waa += 0x100;
        IDX(o, m);
        if (o->animFrame & 1)
            pt.x = 0x80 - DAT_8007a04c[(short)m];
        else
            pt.x = DAT_8007a04c[(short)m];
        pt.y = DAT_8007a04e[(short)m];
        o->velX = X.h->p.whole + MulCos((unsigned char)pt.x, pt.y);
        o->velY = X.y.p.whole + MulNegSinScaled((unsigned char)pt.x, pt.y);
        FUN_80031588(DAT_8009d600, o->waa, &pt.x, &pt.y);
        o->h->p.whole = o->velX + pt.x;
        o->y.p.whole = o->velY + pt.y;
        if (!(BTN() && DAT_8009c93f == 0)) {
            X.b9d = 1;
            DAT_8009c330->b00 = 2;
            DAT_8009c330->w22 = 0;
            o->state = 0;
        } else {
            DAT_8009c330->w22++;
        }
        if (DAT_8009c330->w22 < 0x3d)
            break;
        DAT_8009c330->w22 = 0;
        o->state++;
        break;
    case 8:
        o->animFrame = X.animFrame;
        FUN_800314a8(o);
        AnimJump(&X, 0);
        if ((DAT_1f8001f8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xff, 2);
            SfxPlay2(3, 6);
        }
        AnimJump(&X, DAT_8009c330->w22 / 3 & 3);
        u = X.animFrame;
        o->animFrame = u;
        k = (short)(u * 3);
        DAT_8009d600[0] = 0x10;
        DAT_8009d602[0] = 4;
        DAT_8009d604[0] = DAT_8007a070[k];
        o->waa += DAT_8007a072[k];
        if (o->waa >= 0x100)
            o->waa -= 0x100;
        if (o->waa < 0)
            o->waa += 0x100;
        IDX(o, m);
        if (o->animFrame & 1)
            pt.x = 0x80 - DAT_8007a04c[(short)m];
        else
            pt.x = DAT_8007a04c[(short)m];
        pt.y = DAT_8007a04e[(short)m];
        o->velX = X.h->p.whole + MulCos((unsigned char)pt.x, pt.y);
        o->velY = X.y.p.whole + MulNegSinScaled((unsigned char)pt.x, pt.y);
        FUN_80031588(DAT_8009d600, o->waa, &pt.x, &pt.y);
        o->h->p.whole = o->velX + pt.x;
        o->y.p.whole = o->velY + pt.y;
        if (!(BTN() && DAT_8009c93f == 0)) {
            DAT_8009c330->b00 = 1;
            DAT_8009c330->w22 = 0;
            X.b9d = 1;
            o->state = 0;
        }
        DAT_8009c330->w22++;
        break;
    }
    if (U8X(0xac) >= 2) {
        DAT_8009d2e8 = DAT_800a611c;
        DAT_8009c330->b08 = 0;
        X.step = 0xe;
        X.ba5 = 0;
        X.state = 0;
        X.substep = 0;
        DAT_8009c330->w2c = 0xd;
        if (DAT_8009c330->w2e != 0xd) {
            FUN_800efc04(o);
            AnimJump(o, 0);
            DAT_8009c330->w2e = DAT_8009c330->w2c;
        }
        DAT_8009c330->w2e = 0xffff;
        X.b9d = 0;
        B338(0xc) = 0;
        DAT_8009c330->b00 = 0;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
    }
}
