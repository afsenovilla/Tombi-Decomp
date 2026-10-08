// FUNC 800317c0 6188 MAIN0
// MATCHING 800317c0 6188
#include "TOBJ.H"
typedef struct { short x, y; } XY;
typedef struct {
    char p0[4];
    unsigned char b04, step, state, substep;
    char p8[0x14 - 8];
    Fix16 y;
    char p18[0x2e - 0x18];
    unsigned short animFrame;
    char p30[0x40 - 0x30];
    Fix16 *h;
    Fix16 *d;
    char p48[0x8c - 0x48];
    int d8c;
    char p90[0x9d - 0x90];
    unsigned char b9d, b9e;
    char p9f[0xa4 - 0x9f];
    unsigned char ba4, ba5;
    char pa6[0xac - 0xa6];
    unsigned char bac;
    char pad[0xe4 - 0xad];
    int de4;
} P38;
typedef struct {
    unsigned char b0;
    char p1[5];
    unsigned char b6, b7, b8;
    char p9[0x22 - 9];
    unsigned short w22;
    char p24[4];
    unsigned short w28, w2a, w2c, w2e;
} G330;
typedef struct {
    char p0[0xc];
    unsigned char b0c, b0d;
    short w0e;
    char p10[0x30 - 0x10];
    short w30, w32, w34, w36;
} G338;
typedef struct { short a, b, c; } D600;
extern P38 D_800A6038;
extern Fix16 *D_800A6078;
extern G330 *D_8009C330;
extern G338 *D_8009C338;
extern D600 D_8009D600;
extern short D_8007A070[], D_8007A072[], D_8007A074[];
extern short D_8007A04C[], D_8007A04E[];
extern unsigned char *D_8009F0EC;
extern unsigned short D_8009D670;
extern unsigned short D_1F8003C8, D_1F8001F8, D_1F800282;
extern unsigned char D_8009C93F, D_8009D2B2;
extern int D_8009D2E8;
extern short MulCos(int, int);
extern short MulNegSinScaled(int, int);
extern void SfxPlay(int);
extern void SfxPlay2(int, int);
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern void ObjApplyVelocity(TObj *);
extern void FUN_800314a8(TObj *);
extern void FUN_800312e8(TObj *);
extern void FUN_800313c8(TObj *);
extern short FUN_800411cc(TObj *, int, int);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_800eaffc(int, int, int, int);
extern void FUN_8002c578(TObj *, int, int, int);
extern short FUN_8002078c(XY, XY);
extern void FUN_800ec710(int, TObj *);
extern void FUN_80025f40(int, int, int, int);
extern void FUN_80031588(D600 *, int, short *, short *);
extern void FUN_800efc04(TObj *);

#define IDX(o, i) \
    i = o->step * 6; \
    switch (o->animFrame) { \
    case 0: case 1: case 2: case 3: break; \
    case 4: case 5: i += 2; break; \
    case 6: case 7: i += 4; break; \
    }

#define TB(o) ((short)(o->animFrame + o->animFrame * 2))

#define PADOK() ((*(volatile unsigned short *)&D_8009D670 & D_1F8003C8) && D_800A6038.b04 == 1 && D_800A6038.ba4 == 0)

void func_800317C0(TObj *o)
{
    XY t, c;
    short dx, dy;
    short i;
    short v;
    short n;
    int k;
    short v2;

    D_8009C330->w2e = 0xffff;
    switch (o->state) {
    case 0:
        o->animFrame = D_800A6038.animFrame;
        D_800A6038.b9d = 1;
        D_8009C330->b7 = o->animFrame;
        v = D_8007A074[TB(o)];
        o->b0a = 0;
        o->active = 2;
        o->waa = v;
        D_8009C338->b0c = 0;
        D_8009C330->w22 = 0;
        D_8009D600.b = 0;
        D_8009C338->w30 = 1;
        D_8009C338->w32 = 0;
        D_8009C338->w34 = 0;
        D_8009C338->w36 = 0;
        o->wa8 = 0;
        if (D_800A6038.b9e == 4 || D_800A6038.b9e == 7) {
            o->d94 = (int)D_8009F0EC;
            *D_8009F0EC = 2;
        }
        IDX(o, i);
        if (o->animFrame & 1) dx = 160 - D_8007A04C[i];
        else dx = D_8007A04C[i] - 32;
        dy = D_8007A04E[i];
        if (o->animFrame >= 6) {
            k = D_800A6078->p.whole + MulCos((unsigned char)dx, dy);
            if (o->animFrame & 1) v2 = k - 4;
            else v2 = k + 4;
            o->velX = v2;
            o->velY = D_800A6038.y.p.whole + MulNegSinScaled((unsigned char)dx, dy);
        } else {
            o->velX = D_800A6078->p.whole + MulCos((unsigned char)dx, dy);
            o->velY = D_800A6038.y.p.whole + MulNegSinScaled((unsigned char)dx, dy);
        }
        o->h->p.whole = o->velX + MulCos(((unsigned short)o->waa + 0x80) & 0xff, 16);
        o->y.p.whole = o->velY + MulNegSinScaled(((unsigned short)o->waa + 0x80) & 0xff, 16);
        o->d->p.whole = D_800A6038.d->p.whole;
        o->ba5 = 1;
        o->box0 = 6;
        o->box2 = 6;
        o->box1 = 12;
        o->box3 = 12;
        D_8009C338->b0d = 0;
        switch (D_8009C330->b0) {
        case 2:
            o->wa8 = 0x1400;
            SfxPlay2(3, 2);
            o->w98 = 1;
            break;
        case 1:
            o->wa8 = 0x1900;
            SfxPlay2(3, 8);
            o->w98 = 2;
            break;
        }
        o->velH = MulCos(o->waa, o->wa8);
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        D_8009C330->w28 = 0xffff;
        D_8009C330->w2a = 0xffff;
        FUN_800314a8(o);
        AnimJump((TObj *)&D_800A6038, 0);
        o->waa += 0x80;
        o->state++;
        break;
    case 1:
        AnimAdvance((TObj *)&D_800A6038);
        IDX(o, i);
        if (o->animFrame & 1) dx = 160 - D_8007A04C[i];
        else dx = D_8007A04C[i] - 32;
        dy = D_8007A04E[i];
        if (o->animFrame >= 6) {
            k = D_800A6078->p.whole + MulCos((unsigned char)dx, dy);
            if (o->animFrame & 1) v2 = k - 4;
            else v2 = k + 4;
            o->velX = v2;
            o->velY = D_800A6038.y.p.whole + MulNegSinScaled((unsigned char)dx, dy);
        } else {
            o->velX = D_800A6078->p.whole + MulCos((unsigned char)dx, dy);
            o->velY = D_800A6038.y.p.whole + MulNegSinScaled((unsigned char)dx, dy);
        }
        o->h->p.whole = o->velX + MulCos((unsigned char)o->waa, 16);
        o->y.p.whole = o->velY + MulNegSinScaled((unsigned char)o->waa, 16);
        {
            n = TB(o);
            o->waa += D_8007A072[n];
            if (o->animFrame & 1) {
                if (o->waa < D_8007A072[n + 1] + 0x40) {
                    FUN_800312e8(o);
                    o->waa = D_8007A072[n + 1];
                    D_8009C338->b0c = 1;
                    D_800A6038.d8c = 0;
                    o->timer = 1;
                    o->state++;
                }
            } else {
                if (o->waa > D_8007A072[n + 1] + 0xc0) {
                    FUN_800312e8(o);
                    o->waa = D_8007A072[n + 1];
                    D_8009C338->b0c = 1;
                    D_800A6038.d8c = 0;
                    o->timer = 1;
                    o->state++;
                }
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
        D_8009C338->w30 += (short)o->wa8 >> 8;
        if (D_8009C338->w30 > 0x60 - D_8009C330->b0 * 16)
            D_8009C338->w30 = 0x60 - (D_8009C330->b0 << 4);
        D_8009C338->w32 -= ((short)o->wa8 >> 8) * 2;
        if (D_8009C338->w32 < 0) D_8009C338->w32 = 0;
        {
            short s = o->wa8 - 0x200;
            o->wa8 = s;
            o->velH = MulCos(o->waa, s);
        }
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        ObjApplyVelocity(o);
        if (o->active == 1 && FUN_800411cc(o, o->h->p.whole, o->y.p.whole) != 0) {
            unsigned short u;
            int m;
            FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            u = D_1F800282;
            m = u >> 5 & 0xf;
            if ((u & 0x3000) == 0 && m < 4 && m != 0)
                FUN_800eaffc(o->h->p.whole, o->y.p.whole, o->d->p.whole, (unsigned char)o->animFrame);
            SfxPlay(5);
            o->wa8 = 0x4ff;
        }
        D_800A6038.d8c = 0;
        if (o->wa8 >= 0x500) break;
        {
            unsigned short w = D_8007A074[TB(o)];
            o->waa = w;
            o->velX = o->h->p.whole + MulCos((w + 0x80) & 0xff, 24);
        }
        o->velY = o->y.p.whole + MulNegSinScaled(((unsigned short)o->waa + 0x80) & 0xff, 24);
        {
            G338 *q = D_8009C338;
            q->b0c = 2;
            q->w32 = 0;
            D_8009C338->w0e = 0;
        }
        if (o->visible) {
            int x = o->a.p.whole;
            FUN_8002c578(o, (short)((o->animFrame & 1) ? x - 16 : x + 16), o->y.p.whole, o->b.p.whole);
        }
        o->state++;
        break;
    case 4:
        D_8009C338->w30 -= ((short)o->wa8 >> 8) * 2;
        if (D_8009C338->w30 < 0) D_8009C338->w30 = 0;
        o->h->p.whole = o->velX + MulCos(((unsigned short)o->waa + (unsigned short)D_8009C338->w0e) & 0xff, 24);
        o->y.p.whole = o->velY + MulNegSinScaled(((unsigned short)o->waa + (unsigned short)D_8009C338->w0e) & 0xff, 24);
        D_8009C338->w0e += D_8007A072[TB(o)] >> 1;
        D_800A6038.d8c = 0;
        if (o->animFrame & 1) {
            {
                unsigned char b = 1;
                if (D_8009C338->w0e < -32) b = 2;
                D_8009C338->b0d = b;
            }
            if (D_8009C338->w0e >= -64) break;
            o->waa = ((unsigned short)o->waa + (unsigned short)D_8009C338->w0e) & 0xff;
            D_8009C338->b0c = 3;
            FUN_800313c8(o);
            AnimJump((TObj *)&D_800A6038, 0);
            D_8009C338->w0e = (D_8007A072[TB(o) + 1] - 0x40) & 0xff;
            o->ba5 = 0;
            o->active = 2;
            o->state++;
        } else {
            {
                unsigned char b = 1;
                if (D_8009C338->w0e > 32) b = 2;
                D_8009C338->b0d = b;
            }
            if (D_8009C338->w0e <= 64) break;
            o->waa = ((unsigned short)o->waa + (unsigned short)D_8009C338->w0e) & 0xff;
            D_8009C338->b0c = 3;
            FUN_800313c8(o);
            AnimJump((TObj *)&D_800A6038, 0);
            D_8009C338->w0e = (D_8007A072[TB(o) + 1] + 0x40) & 0xff;
            o->ba5 = 0;
            o->active = 2;
            o->state++;
        }
        break;
    case 5:
        D_8009C338->b0d = 0;
        o->wa8 = 0x1400;
        D_8009C338->w30 += 20;
        if (D_8009C338->w30 > 0x60 - D_8009C330->b0 * 16)
            D_8009C338->w30 = 0x60 - (D_8009C330->b0 << 4);
        D_8009C338->w32 -= (short)o->wa8 >> 8;
        if (D_8009C338->w32 < 0) D_8009C338->w32 = 0;
        if (o->animFrame & 1) o->velX = D_800A6078->p.whole + 16;
        else o->velX = D_800A6078->p.whole - 16;
        o->velY = D_800A6038.y.p.whole - 16;
        t.x = o->velX + MulCos(D_8009C338->w0e, 12);
        t.y = o->velY + MulNegSinScaled(D_8009C338->w0e, 12);
        c.x = o->h->p.whole;
        c.y = o->y.p.whole;
        o->waa = FUN_8002078c(c, t);
        o->velH = MulCos(o->waa, o->wa8);
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        ObjApplyVelocity(o);
        dx = t.x - c.x + 16;
        dy = t.y - c.y + 16;
        if ((unsigned short)dx >= 32 || (unsigned short)dy >= 32) break;
        o->waa = D_8007A074[TB(o)] + 0x80;
        if (o->animFrame & 1) {
            {
                short pv = D_800A6078->p.whole;
                o->waa = (*(volatile unsigned short *)&o->waa + 0x40) & 0xff;
                o->velX = pv + 16;
            }
        } else {
            {
                short pv = D_800A6078->p.whole;
                o->waa = (*(volatile unsigned short *)&o->waa - 0x40) & 0xff;
                o->velX = pv - 16;
            }
        }
        o->velY = D_800A6038.y.p.whole - 16;
        o->h->p.whole = o->velX + MulCos(o->waa, 12);
        o->y.p.whole = o->velY + MulNegSinScaled(o->waa, 12);
        D_8009C338->w32 = 0;
        D_8009C338->w0e = 0;
        D_8009C338->b0c = 0;
        D_800A6038.b9d = 0;
        o->ba5 = 0;
        o->state++;
        break;
    case 6:
        if (o->animFrame & 1) o->velX = D_800A6078->p.whole + 16;
        else o->velX = D_800A6078->p.whole - 16;
        o->velY = D_800A6038.y.p.whole - 16;
        AnimAdvance((TObj *)&D_800A6038);
        D_8009C338->w0e += D_8007A072[TB(o)];
        if (o->animFrame & 1) {
            if (D_8009C338->w0e < -64) {
                if (D_800A6038.b9e == 4 || D_800A6038.b9e == 7)
                    *(unsigned char *)o->d94 = D_8009C330->b6;
                if (PADOK() && D_800A6038.bac < 2 && D_8009C93F == 0) {
                    FUN_800ec710(D_8009D2B2, o);
                    D_800A6038.b9d = 0;
                    D_8009C338->w0e = 0;
                    o->waa -= 0x40;
                    o->state++;
                } else {
                    D_800A6038.b9d = 0;
                    D_8009C338->b0c = 0;
                    D_8009C330->b0 = 0;
                    o->b04 = 2;
                    o->step = 0;
                    o->state = 0;
                }
            }
        } else if (D_8009C338->w0e > 64) {
            if (D_800A6038.b9e == 4 || D_800A6038.b9e == 7)
                *(unsigned char *)o->d94 = D_8009C330->b6;
            if (!PADOK() || D_800A6038.bac >= 2 || D_8009C93F != 0) {
                D_800A6038.b9d = 0;
                D_8009C338->b0c = 0;
                D_8009C330->b0 = 0;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            } else {
                FUN_800ec710(D_8009D2B2, o);
                D_800A6038.b9d = 0;
                D_8009C338->w0e = 0;
                o->waa += 0x40;
                o->state++;
            }
        }
        o->h->p.whole = o->velX + MulCos(((unsigned short)o->waa + (unsigned short)D_8009C338->w0e) & 0xff, 12);
        o->y.p.whole = o->velY + MulNegSinScaled(((unsigned short)o->waa + (unsigned short)D_8009C338->w0e) & 0xff, 12);
        break;
    case 7:
        o->animFrame = D_800A6038.animFrame;
        FUN_800314a8(o);
        AnimJump((TObj *)&D_800A6038, 0);
        if ((D_1F8001F8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xc0, 2);
            SfxPlay(3);
        }
        AnimJump((TObj *)&D_800A6038, D_8009C330->w22 >> 2 & 3);
        n = TB(o);
        D_8009D600.a = 10;
        D_8009D600.b = 2;
        D_8009D600.c = D_8007A070[n];
        o->waa += D_8007A072[n];
        if (o->waa >= 0x100) o->waa -= 0x100;
        if (o->waa < 0) o->waa += 0x100;
        IDX(o, i);
        if (o->animFrame & 1) dx = 0x80 - D_8007A04C[i];
        else dx = D_8007A04C[i];
        dy = D_8007A04E[i];
        o->velX = D_800A6078->p.whole + MulCos((unsigned char)dx, dy);
        o->velY = D_800A6038.y.p.whole + MulNegSinScaled((unsigned char)dx, dy);
        FUN_80031588(&D_8009D600, o->waa, &dx, &dy);
        o->h->p.whole = o->velX + dx;
        o->y.p.whole = o->velY + dy;
        if (!PADOK() || D_8009C93F != 0) {
            D_800A6038.b9d = 1;
            D_8009C330->b0 = 2;
            D_8009C330->w22 = 0;
            o->state = 0;
        } else {
            D_8009C330->w22++;
        }
        if (D_8009C330->w22 < 61) break;
        D_8009C330->w22 = 0;
        o->state++;
        break;
    case 8:
        o->animFrame = D_800A6038.animFrame;
        FUN_800314a8(o);
        AnimJump((TObj *)&D_800A6038, 0);
        if ((D_1F8001F8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xff, 2);
            SfxPlay2(3, 6);
        }
        AnimJump((TObj *)&D_800A6038, D_8009C330->w22 / 3 & 3);
        o->animFrame = D_800A6038.animFrame;
        n = TB(o);
        D_8009D600.a = 16;
        D_8009D600.b = 4;
        D_8009D600.c = D_8007A070[n];
        o->waa += D_8007A072[n];
        if (o->waa >= 0x100) o->waa -= 0x100;
        if (o->waa < 0) o->waa += 0x100;
        IDX(o, i);
        if (o->animFrame & 1) dx = 0x80 - D_8007A04C[i];
        else dx = D_8007A04C[i];
        dy = D_8007A04E[i];
        o->velX = D_800A6078->p.whole + MulCos((unsigned char)dx, dy);
        o->velY = D_800A6038.y.p.whole + MulNegSinScaled((unsigned char)dx, dy);
        FUN_80031588(&D_8009D600, o->waa, &dx, &dy);
        o->h->p.whole = o->velX + dx;
        o->y.p.whole = o->velY + dy;
        if (!PADOK() || D_8009C93F != 0) {
            D_8009C330->b0 = 1;
            D_8009C330->w22 = 0;
            D_800A6038.b9d = 1;
            o->state = 0;
        }
        D_8009C330->w22++;
        break;
    }
    if (D_800A6038.bac >= 2) {
        char pad[16];
        D_8009D2E8 = D_800A6038.de4;
        D_8009C330->b8 = 0;
        D_800A6038.step = 14;
        D_800A6038.ba5 = 0;
        D_800A6038.state = 0;
        D_800A6038.substep = 0;
        D_8009C330->w2c = 13;
        if (D_8009C330->w2e != 13) {
            FUN_800efc04(o);
            AnimJump(o, 0);
            D_8009C330->w2e = D_8009C330->w2c;
        }
        D_8009C330->w2e = 0xffff;
        D_800A6038.b9d = 0;
        D_8009C338->b0c = 0;
        D_8009C330->b0 = 0;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
    }
}
