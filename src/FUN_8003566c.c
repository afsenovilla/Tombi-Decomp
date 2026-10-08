// FUNC 8003566c 5708 MAIN0
// MATCHING 8003566c 5708
#include "TOBJ.H"
#include "raw7.h"

typedef struct { short x, y; } P;

extern TObj D_800A6038;
extern Fix16 *D_800A6078, *D_800A607C;
extern short D_800A604E;
extern TObj *D_800A611C;
extern TObj *D_8009D2E8;
typedef struct {
    unsigned char b0, p1[5], b6, b7, b8, p9[0x22 - 9];
    unsigned short w22;
    unsigned char p24[4];
    unsigned short w28, w2a, w2c, w2e;
} S330;
typedef struct {
    unsigned char p0[0xc];
    unsigned char bc, bd;
    short we;
    unsigned char p10[0x20];
    short w30, w32, w34, w36;
} S338;
extern S330 *D_8009C330;
extern S338 *D_8009C338;
extern unsigned char *D_8009F0EC;
extern short D_8009D600[], D_8009D602[], D_8009D604[];
extern short D_8007A072[];
extern short D_8007A04C[];
extern unsigned short D_8009D670;
extern unsigned char D_8009C93F;
extern unsigned char D_8009D2B2;
extern int D_8009C934;
extern unsigned short D_1F8001F8;
extern short MulCos(int, short);
extern short MulNegSinScaled(int, short);
extern void SfxPlay2(int, int);
extern void playSFX(int);
extern void FUN_800314a8(TObj *);
extern void FUN_800312e8(TObj *);
extern void FUN_800313c8(TObj *);
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern void ObjApplyVelocity(TObj *);
extern short FUN_80035428(TObj *);
extern short FUN_8002078c(P, P);
extern void FUN_8002c578(TObj *, int, int, int);
extern void FUN_800ec710(int, TObj *);
extern void FUN_80025f40(int, int, int, int);
extern void FUN_80031588(short *, int, short *, short *);
extern void FUN_800efc04(TObj *);

#define G D_800A6038
#define TB(n) D_8007A072[(short)(o->animFrame + o->animFrame * 2) + (n) - 1]
#define TK(n) D_8007A072[k + (n) - 1]

void FUN_8003566c(TObj *o)
{
    P t;
    P s;
    short va;
    short vb;
    short i;
    short k;
    int x;
    unsigned short u;

    D_8009C330->w2e = 0xffff;
    switch (o->state) {
    case 0:
        o->animFrame = G.animFrame;
        G.b9d = 1;
        D_8009C330->b7 = o->animFrame;
        u = TB(2);
        o->active = 2;
        o->b0a = 0;
        o->waa = u;
        D_8009C338->bc = 0;
        D_8009C330->w22 = 0;
        D_8009D602[0] = 0;
        D_8009C338->w30 = 1;
        D_8009C338->w32 = 0;
        D_8009C338->w34 = 0;
        D_8009C338->w36 = 0;
        o->wa8 = 0;
        if (G.b9e == 4 || G.b9e == 7) {
            o->d94 = (int)D_8009F0EC;
            *D_8009F0EC = 2;
        }
        i = o->step * 6;
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3:
            break;
        case 4: case 5:
            i += 2;
            break;
        case 6: case 7:
            i += 4;
            break;
        }
        if (o->animFrame & 1) va = 0xa0 - D_8007A04C[i];
        else va = D_8007A04C[i] - 0x20;
        vb = D_8007A04C[i + 1];
        if (o->animFrame >= 6) {
            x = D_800A6078->p.whole + MulCos((unsigned char)va, vb);
            o->velX = (o->animFrame & 1) ? x - 4 : x + 4;
            o->velY = D_800A604E + MulNegSinScaled((unsigned char)va, vb);
        } else {
            o->velX = D_800A6078->p.whole + MulCos((unsigned char)va, vb);
            o->velY = D_800A604E + MulNegSinScaled((unsigned char)va, vb);
        }
        o->h->p.whole = o->velX + MulCos(((unsigned short)o->waa + 0x80) & 0xff, 0x10);
        o->y.p.whole = o->velY + MulNegSinScaled(((unsigned short)o->waa + 0x80) & 0xff, 0x10);
        o->d->p.whole = G.d->p.whole;
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
        AnimJump(&G, 0);
        o->waa += 0x80;
        if (o->animFrame & 1) o->waa -= 0x40;
        else o->waa += 0x40;
        o->state++;
        break;
    case 1:
        AnimAdvance(&G);
        i = o->step * 6;
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3:
            break;
        case 4: case 5:
            i += 2;
            break;
        case 6: case 7:
            i += 4;
            break;
        }
        if (o->animFrame & 1) va = 0xa0 - D_8007A04C[i];
        else va = D_8007A04C[i] - 0x20;
        vb = D_8007A04C[i + 1];
        if (o->animFrame >= 6) {
            x = D_800A6078->p.whole + MulCos((unsigned char)va, vb);
            o->velX = (o->animFrame & 1) ? x - 4 : x + 4;
            o->velY = D_800A604E + MulNegSinScaled((unsigned char)va, vb);
        } else {
            o->velX = D_800A6078->p.whole + MulCos((unsigned char)va, vb);
            o->velY = D_800A604E + MulNegSinScaled((unsigned char)va, vb);
        }
        o->h->p.whole = o->velX + MulCos((unsigned char)o->waa, 0x10);
        o->y.p.whole = o->velY + MulNegSinScaled((unsigned char)o->waa, 0x10);
        k = o->animFrame + o->animFrame * 2;
        o->waa += TK(1);
        if (o->animFrame & 1) {
            if (o->waa < TK(2) + 0x40) {
                FUN_800312e8(o);
                AnimJump(&G, 0);
                o->waa = TK(2);
                D_8009C338->bc = 1;
                G.d8c = 0;
                o->b69 = 0;
                o->timer = 1;
                o->state++;
            }
        } else {
            if (o->waa > TK(2) + 0xc0) {
                FUN_800312e8(o);
                AnimJump(&G, 0);
                o->waa = TK(2);
                D_8009C338->bc = 1;
                G.d8c = 0;
                o->b69 = 0;
                o->timer = 1;
                o->state++;
            }
        }
        break;
    case 2:
        if (o->timer != 0) {
            if (--o->timer <= 0) {
                o->timer = 0;
                o->active = 1;
                o->state++;
            }
        }
    case 3:
        D_8009C338->w30 += o->wa8 >> 8;
        if (D_8009C338->w30 > 0x60 - D_8009C330->b0 * 16) D_8009C338->w30 = 0x60 - (D_8009C330->b0 << 4);
        D_8009C338->w32 -= (o->wa8 >> 8) * 2;
        if (D_8009C338->w32 < 0) D_8009C338->w32 = 0;
        o->wa8 -= 0x280;
        o->velH = MulCos(o->waa, o->wa8);
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        ObjApplyVelocity(o);
        if (FUN_80035428(o) == 0) {
            G.d8c = 0;
            if (o->wa8 < 0x500) {
                o->waa = TB(2);
                o->velX = o->h->p.whole + MulCos(((unsigned short)o->waa + 0x80) & 0xff, 0x18);
                o->velY = o->y.p.whole + MulNegSinScaled(((unsigned short)o->waa + 0x80) & 0xff, 0x18);
                D_8009C338->w32 = 0;
                U8(D_8009C338, 0xc) = 2;
                D_8009C338->we = 0;
                if (o->visible) {
                    FUN_8002c578(o, (short)(o->a.p.whole + ((o->animFrame & 1) ? -0x10 : 0x10)), o->y.p.whole, o->b.p.whole);
                }
                o->state = 4;
            }
        }
        break;
    case 4:
        D_8009C338->w30 -= (o->wa8 >> 8) * 2;
        if (D_8009C338->w30 < 0) D_8009C338->w30 = 0;
        o->h->p.whole = o->velX + MulCos(((unsigned short)o->waa + (unsigned short)D_8009C338->we) & 0xff, 0x18);
        o->y.p.whole = o->velY + MulNegSinScaled(((unsigned short)o->waa + (unsigned short)D_8009C338->we) & 0xff, 0x18);
        D_8009C338->we += TB(1) >> 1;
        G.d8c = 0;
        if (o->animFrame & 1) {
            D_8009C338->bd = (D_8009C338->we < -0x20) ? 2 : 1;
            if (D_8009C338->we < -0x40) {
                o->waa = (o->waa + D_8009C338->we) & 0xff;
                D_8009C338->bc = 3;
                FUN_800313c8(o);
                AnimJump(&G, 0);
                D_8009C338->we = (TB(2) - 0x40) & 0xff;
                o->state++;
            }
        } else {
            D_8009C338->bd = (D_8009C338->we > 0x20) ? 2 : 1;
            if (D_8009C338->we > 0x40) {
                o->waa = (o->waa + D_8009C338->we) & 0xff;
                D_8009C338->bc = 3;
                FUN_800313c8(o);
                AnimJump(&G, 0);
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
            D_8009C338->w30 += 0x20;
            if (D_8009C338->w30 > 0x60 - D_8009C330->b0 * 16) D_8009C338->w30 = 0x60 - (D_8009C330->b0 << 4);
            D_8009C338->w32 -= o->wa8 >> 8;
            if (D_8009C338->w32 < 0) D_8009C338->w32 = 0;
            if (o->animFrame & 1) o->velX = D_800A6078->p.whole + 0x10;
            else o->velX = D_800A6078->p.whole - 0x10;
            o->velY = D_800A604E - 0x10;
            t.x = o->velX + MulCos(D_8009C338->we, 0xc);
            t.y = o->velY + MulNegSinScaled(D_8009C338->we, 0xc);
            s.x = o->h->p.whole;
            s.y = o->y.p.whole;
            o->waa = FUN_8002078c(s, t);
            o->velH = MulCos(o->waa, o->wa8);
            o->velV = MulNegSinScaled(o->waa, o->wa8);
            ObjApplyVelocity(o);
            va = t.x - s.x + 0x10;
            vb = t.y - s.y + 0x10;
            if ((unsigned short)va < 0x20 && (unsigned short)vb < 0x20) {
                if (G.b9e == 4 || G.b9e == 7) *(unsigned char *)o->d94 = D_8009C330->b6;
                o->waa = TB(2) + 0x80;
                if (o->animFrame & 1) {
                    o->velX = D_800A6078->p.whole + 0x10;
                    o->waa = (o->waa + 0x40) & 0xff;
                } else {
                    o->velX = D_800A6078->p.whole - 0x10;
                    o->waa = (o->waa - 0x40) & 0xff;
                }
                o->velY = D_800A604E - 0x10;
                o->h->p.whole = o->velX + MulCos(o->waa, 0xc);
                o->y.p.whole = o->velY + MulNegSinScaled(o->waa, 0xc);
                D_8009C338->w32 = 0;
                D_8009C338->we = 0;
                D_8009C338->bc = 0;
                G.b9d = 0;
                o->ba5 = 0;
                if (!(*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C8) || G.b04 != 1 || G.ba4 || *(unsigned char *)&G.wac >= 2 || D_8009C93F) {
                    D_8009C330->b0 = 0;
                    o->b04 = 2;
                    o->step = 0;
                    o->state = 0;
                    D_8009C934 = 0;
                } else {
                    FUN_800ec710(D_8009D2B2, o);
                    o->waa += (o->animFrame & 1) ? -0x40 : 0x40;
                    o->state = 6;
                }
            }
        }
        break;
    case 6:
        o->animFrame = G.animFrame;
        FUN_800314a8(o);
        AnimJump(&G, 0);
        if ((D_1F8001F8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xc0, 2);
            playSFX(3);
        }
        AnimJump(&G, (D_8009C330->w22 >> 2) & 3);
        k = o->animFrame + o->animFrame * 2;
        D_8009D600[0] = 10;
        D_8009D602[0] = 2;
        D_8009D604[0] = TK(0);
        o->waa += TK(1);
        if (o->waa > 0xff) o->waa -= 0x100;
        if (o->waa < 0) o->waa += 0x100;
        if (o->animFrame & 1) o->d8c = (o->waa + 0x80) & 0xff;
        else o->d8c = (unsigned char)o->waa;
        i = o->step * 6;
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3:
            break;
        case 4: case 5:
            i += 2;
            break;
        case 6: case 7:
            i += 4;
            break;
        }
        if (o->animFrame & 1) va = 0x80 - D_8007A04C[i];
        else va = D_8007A04C[i];
        vb = D_8007A04C[i + 1];
        o->velX = D_800A6078->p.whole + MulCos((unsigned char)va, vb);
        o->velY = D_800A604E + MulNegSinScaled((unsigned char)va, vb);
        FUN_80031588(D_8009D600, o->waa, &va, &vb);
        o->h->p.whole = o->velX + va;
        o->y.p.whole = o->velY + vb;
        if (!(*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C8) || G.b04 != 1 || G.ba4 || D_8009C93F) {
            G.b9d = 1;
            D_8009C330->b0 = 2;
            D_8009C330->w22 = 0;
            o->state = 0;
        } else {
            D_8009C330->w22++;
        }
        if (D_8009C330->w22 >= 0x3d) {
            D_8009C330->w22 = 0;
            o->state++;
        }
        break;
    case 7:
        o->animFrame = G.animFrame;
        FUN_800314a8(o);
        AnimJump(&G, 0);
        if ((D_1F8001F8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xff, 2);
            SfxPlay2(3, 6);
        }
        AnimJump(&G, (D_8009C330->w22 / 3) & 3);
        o->animFrame = G.animFrame;
        k = o->animFrame + o->animFrame * 2;
        D_8009D600[0] = 0x10;
        D_8009D602[0] = 4;
        D_8009D604[0] = TK(0);
        o->waa += TK(1);
        if (o->waa > 0xff) o->waa -= 0x100;
        if (o->waa < 0) o->waa += 0x100;
        if (o->animFrame & 1) o->d8c = (o->waa + 0x80) & 0xff;
        else o->d8c = (unsigned char)o->waa;
        i = o->step * 6;
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3:
            break;
        case 4: case 5:
            i += 2;
            break;
        case 6: case 7:
            i += 4;
            break;
        }
        if (o->animFrame & 1) va = 0x80 - D_8007A04C[i];
        else va = D_8007A04C[i];
        vb = D_8007A04C[i + 1];
        o->velX = D_800A6078->p.whole + MulCos((unsigned char)va, vb);
        o->velY = D_800A604E + MulNegSinScaled((unsigned char)va, vb);
        FUN_80031588(D_8009D600, o->waa, &va, &vb);
        o->h->p.whole = o->velX + va;
        o->y.p.whole = o->velY + vb;
        if (!(*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C8) || G.b04 != 1 || G.ba4 || D_8009C93F) {
            D_8009C330->b0 = 1;
            D_8009C330->w22 = 0;
            G.b9d = 1;
            o->state = 0;
        }
        D_8009C330->w22++;
        break;
    }
    if (*(unsigned char *)&G.wac >= 2) {
        D_8009D2E8 = D_800A611C;
        D_8009C330->b8 = 0;
        G.step = 0xe;
        D_8009C934 = 0;
        G.ba5 = 0;
        G.state = 0;
        G.substep = 0;
        D_8009C330->w2c = 0xd;
        if (D_8009C330->w2e != 0xd) {
            FUN_800efc04(o);
            AnimJump(o, 0);
            D_8009C330->w2e = D_8009C330->w2c;
        }
        D_8009C330->w2e = 0xffff;
        G.b9d = 0;
        D_8009C338->bc = 0;
        D_8009C330->b0 = 0;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
    }
}
