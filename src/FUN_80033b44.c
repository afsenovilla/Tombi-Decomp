// FUNC 80033b44 5056 MAIN0
// MATCHING 80033b44 5056
/* Case 1 tail: x/y as block-local ints declared inside each branch (separate pseudos, compare cross-jumped). Debt: frame pad block at the end. */
#include "TOBJ.H"
typedef struct V2 { short x, y; } V2;
typedef struct {
    unsigned char b0; char p1[6]; unsigned char b7; char p8[0x22 - 8];
    unsigned short w22; char p24[4]; unsigned short w28, w2a; char p2c[2]; unsigned short w2e;
} P;
typedef struct { short w0, w2, w4; } D600;
extern P *DAT_8009c330;
extern TObj DAT_800a6038;
extern unsigned char DAT_800a60d5[];
extern unsigned char DAT_800a60d6[];
extern unsigned char *DAT_8009f0ec;
extern D600 DAT_8009d600;
extern short DAT_8009d600s, DAT_8009d602s, DAT_8009d604a[];
extern unsigned short DAT_8007a04c[];
extern unsigned short DAT_8007a070[];
extern short DAT_8007a072[];
extern unsigned char DAT_8009d2b2[];
extern volatile unsigned short DAT_8009d670[];
extern unsigned short DAT_1f8003c8;
extern unsigned short DAT_1f8001f8;
extern unsigned char DAT_800a603c, DAT_800a60dc, DAT_800a60e4, DAT_800a60fe, DAT_800a60ff;
extern unsigned char DAT_8009c93f, DAT_8009c990;
extern short MulCos(int, int);
extern short MulNegSinScaled(int, int);
extern void SfxPlay(int);
extern void SfxPlay2(int, int);
extern void FUN_800314a8(TObj *);
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern void FUN_800ec710(int, TObj *);
extern void FUN_80033834(TObj *);
extern short FUN_800331c4(TObj *);
extern short FUN_80033638(TObj *);
extern void ObjApplyVelocity(TObj *);
extern short FUN_8002078c(V2, V2);
extern void FUN_800339a4(TObj *);
extern void FUN_80025f40(int, int, int, int);
extern void FUN_80031588(D600 *, int, short *, short *);

static __inline__ short dir(TObj *o)
{
    V2 b, a;
    a.x = o->h->p.whole;
    a.y = o->y.p.whole;
    b.x = DAT_800a6038.h->p.whole;
    b.y = DAT_800a6038.y.p.whole;
    return FUN_8002078c(a, b);
}
#define DIR dir

static __inline__ short near(TObj *o, int d, int w)
{
    if ((unsigned short)(DAT_800a6038.h->p.whole - o->h->p.whole + d) > w) return 0;
    if ((unsigned short)(DAT_800a6038.y.p.whole - o->y.p.whole + d) > w) return 0;
    return 1;
}

#define SPIN(o) \
    { if ((o)->animFrame & 1) (o)->d88 = ((o)->d88 + 0x300) & 0xfff; else (o)->d88 = ((o)->d88 - 0x300) & 0xfff; }

#define IDX(o, idx) \
    idx = (o)->step * 6; \
    switch ((o)->animFrame) { \
    case 0: case 1: case 2: case 3: break; \
    case 4: case 5: idx += 2; break; \
    case 6: case 7: idx += 4; break; \
    }

void FUN_80033b44(TObj *o)
{
    V2 vb, va;
    short a, r;
    short idx;
    short k;
    short s;
    int f;
    unsigned short w;
    unsigned char c;
    unsigned short cc;
    unsigned char *q;
    P *p;

    switch (o->state) {
    case 0:
        p = DAT_8009c330;
        o->animFrame = DAT_800a6038.animFrame;
        DAT_800a60d5[0] = 1;
        p->b7 = o->animFrame;
        k = DAT_8007a072[(short)(o->animFrame + o->animFrame * 2) + 1];
        o->active = 2;
        o->waa = k;
        *(unsigned short *)((char *)DAT_8009c330 + 0x22) = 0;
        DAT_8009d602s = 0;
        o->wa8 = 0;
        o->w74 = 0;
        o->b6b = 0;
        c = DAT_800a60d6[0];
        o->b6b = c;
        switch (c) {
        case 4:
            q = DAT_8009f0ec;
            k = *q;
            o->w74 = k;
            *q = 2;
            goto set94;
        case 7:
            p = (P *)DAT_8009f0ec;
            o->w74 = p->b0;
            p->b0 = 5;
        set94:
            o->d94 = (int)DAT_8009f0ec;
            break;
        }
        IDX(o, idx);
        if (o->animFrame & 1) a = 0xa0 - DAT_8007a04c[idx]; else a = DAT_8007a04c[idx] - 0x20;
        r = DAT_8007a04c[idx + 1];
        o->velX = DAT_800a6038.h->p.whole + MulCos((unsigned char)a, r);
        o->velY = DAT_800a6038.y.p.whole + MulNegSinScaled((unsigned char)a, r);
        o->h->p.whole = o->velX + MulCos(((unsigned short)o->waa + 0x80) & 0xff, 0x10);
        o->y.p.whole = o->velY + MulNegSinScaled(((unsigned short)o->waa + 0x80) & 0xff, 0x10);
        o->d->p.whole = DAT_800a6038.d->p.whole;
        o->ba5 = 1;
        p = DAT_8009c330;
        o->box0 = 6;
        o->box2 = 6;
        o->box1 = 0xc;
        o->box3 = 0xc;
        f = p->b0;
        switch (f) {
        case 2:
            o->wa8 = 0x1600;
            SfxPlay2(3, 2);
            o->w98 = 1;
            if (DAT_8009d2b2[0] - 5 == f) o->w98 = 2;
            break;
        case 1:
            o->wa8 = 0x1a00;
            SfxPlay2(3, 8);
            o->w98 = 2;
            break;
        }
        o->velH = MulCos((unsigned char)o->waa, o->wa8);
        o->velV = MulNegSinScaled((unsigned char)o->waa, o->wa8);
        DAT_8009c330->w28 = 0xffff;
        DAT_8009c330->w2a = 0xffff;
        FUN_800314a8(o);
        AnimJump(&DAT_800a6038, 0);
        o->state++;
        o->waa += 0x80;
        DAT_8009c330->w2e = 0xffff;
        FUN_800ec710(DAT_8009d2b2[0], o);
        break;
    case 1:
        SPIN(o);
        o->d84 = 0x100;
        o->d8c = 0;
        AnimAdvance(&DAT_800a6038);
        IDX(o, idx);
        if (o->animFrame & 1) a = 0xa0 - DAT_8007a04c[idx]; else a = DAT_8007a04c[idx] - 0x20;
        r = DAT_8007a04c[idx + 1];
        o->velX = DAT_800a6038.h->p.whole + MulCos((unsigned char)a, r);
        o->velY = DAT_800a6038.y.p.whole + MulNegSinScaled((unsigned char)a, r);
        o->h->p.whole = o->velX + MulCos((unsigned char)o->waa, 0x10);
        o->y.p.whole = o->velY + MulNegSinScaled((unsigned char)o->waa, 0x10);
        k = o->animFrame + o->animFrame * 2;
        o->waa += DAT_8007a072[k];
        if (o->animFrame & 1) {
            int x = (short)o->waa, y = DAT_8007a072[k + 1] + 0x40;
            if (x < y) FUN_80033834(o);
        } else {
            int y = (short)o->waa, x = DAT_8007a072[k + 1] + 0xc0;
            if (x < y) FUN_80033834(o);
        }
        break;
    case 2:
        if (o->w7a > 0 && --o->w7a <= 0) o->active = 1;
        SPIN(o);
        o->d84 = (o->d84 - 0x80) & 0xfff;
        {
            int w, cc, g;
            if (o->animFrame & 1) {
                cc = (unsigned short)o->wac;
                w = ((unsigned short)o->waa - 2) & 0xff;
                o->waa = w;
                w = w < cc;
            } else {
                cc = (unsigned short)o->wac;
                w = ((unsigned short)o->waa + 2) & 0xff;
                o->waa = w;
                w = cc < w;
            }
            if (w) o->waa = cc & 0xff;
        }
        if (--o->timer <= 0) {
            short g;
            o->timer = 5;
            g = 0;
            switch (FUN_800331c4(o)) {
            case 0:
            case 1:
                o->timer = 5;
                break;
            case 2:
                o->timer = 5;
                g = 1;
                break;
            }
            o->state = g ? 3 : 2;
        }
        if (FUN_80033638(o) == 0) break;
        if (o->animFrame & 1)
            o->waa = (o->waa - 0x40) & 0xff;
        else
            o->waa = (o->waa + 0x40) & 0xff;
        o->state = 4;
        o->timer = 0;
        DAT_8009c330->w2e = 0xff;
        break;
    case 3:
        if (o->w7a > 0 && --o->w7a <= 0) o->active = 1;
        SPIN(o);
        o->d84 = (o->d84 - 0x80) & 0xfff;
        {
            int w, cc, g;
            if (o->animFrame & 1) {
                cc = (unsigned short)o->wac;
                w = ((unsigned short)o->waa + 2) & 0xff;
                o->waa = w;
                w = cc < w;
            } else {
                cc = (unsigned short)o->wac;
                w = ((unsigned short)o->waa - 2) & 0xff;
                o->waa = w;
                w = w < cc;
            }
            if (w) o->waa = cc & 0xff;
        }
        if (--o->timer <= 0) {
            short g;
            o->timer = 5;
            g = 0;
            switch (FUN_800331c4(o)) {
            case 0:
            case 1:
                o->timer = 5;
                break;
            case 2:
                o->timer = 5;
                g = 1;
                break;
            }
            o->state = g ? 3 : 2;
        }
        if (FUN_80033638(o) == 0) break;
        if (o->animFrame & 1)
            o->waa = (o->waa + 0x40) & 0xff;
        else
            o->waa = (o->waa - 0x40) & 0xff;
        o->state = 4;
        o->timer = 0;
        DAT_8009c330->w2e = 0xff;
        break;
    case 4:
        SPIN(o);
        o->d84 = (o->d84 - 0x80) & 0xfff;
        if (--o->timer <= 0) {
            s = DIR(o);
            o->timer = 2;
            o->wac = (s - o->waa) & 0xff;
        }
        if ((unsigned short)o->wac < 0x80)
            o->waa = (o->waa + 10) & 0xff;
        else
            o->waa = (o->waa - 10) & 0xff;
        o->wa8 += 0x80;
        if (o->wa8 > 0x1a00) o->wa8 = 0x1a00;
        o->velH = MulCos(o->waa, o->wa8);
        o->velV = MulNegSinScaled(o->waa, o->wa8);
        ObjApplyVelocity(o);
        if (near(o, 0x20, 0x40)) {
            o->wa8 = 0x1a00;
            o->state = 6;
        }
        break;
    case 6:
        o->waa = DIR(o);
        SPIN(o);
        o->d84 = (o->d84 - 0x80) & 0xfff;
        o->velH = MulCos((unsigned char)o->waa, o->wa8);
        o->velV = MulNegSinScaled((unsigned char)o->waa, o->wa8);
        ObjApplyVelocity(o);
        if (near(o, 0x10, 0x20)) {
            unsigned int m;
            o->ba5 = 0;
            DAT_800a60d5[0] = 0;
            o->waa = (o->waa - 0x40) & 0xff;
            o->state++;
            m = DAT_8009d2b2[0] - 5;
            if (o->b6b == 7) {
                *(unsigned char *)o->d94 = o->w74;
                o->b6b = 0;
            }
            if (o->b6b == 4 && ((unsigned char *)o->d94)[2] == 0x1d) {
                *(unsigned char *)o->d94 = o->w74;
                o->b6b = 0;
            }
            if ((DAT_8009d670[0] & DAT_1f8003c8) == 0 || DAT_800a603c != 1 || DAT_800a60dc != 0 ||
                DAT_800a60e4 >= 2 || DAT_800a60fe != 0 || DAT_8009c93f != 0 || m >= 4 ||
                (unsigned char)(DAT_8009c990 - 1) < 2 || DAT_800a60ff != 0) {
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            } else {
                FUN_800339a4(o);
            }
        }
        break;
    case 7:
        SPIN(o);
        o->d8c = 0;
        o->d84 = 0x200;
        o->animFrame = DAT_800a6038.animFrame;
        FUN_800314a8(o);
        AnimJump(&DAT_800a6038, 0);
        if ((DAT_1f8001f8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xc0, 2);
            SfxPlay(3);
        }
        AnimJump(&DAT_800a6038, (DAT_8009c330->w22 >> 2) & 3);
        k = o->animFrame + o->animFrame * 2;
        DAT_8009d600.w0 = 10;
        DAT_8009d600.w2 = 2;
        DAT_8009d600.w4 = DAT_8007a070[k];
        o->waa += DAT_8007a072[k];
        if (o->waa > 0xff) o->waa -= 0x100;
        if (o->waa < 0) o->waa += 0x100;
        IDX(o, idx);
        if (o->animFrame & 1) a = 0x80 - DAT_8007a04c[idx]; else a = DAT_8007a04c[idx];
        r = DAT_8007a04c[idx + 1];
        o->velX = DAT_800a6038.h->p.whole + MulCos((unsigned char)a, r);
        o->velY = DAT_800a6038.y.p.whole + MulNegSinScaled((unsigned char)a, r);
        FUN_80031588(&DAT_8009d600, o->waa, &a, &r);
        o->h->p.whole = o->velX + a;
        o->y.p.whole = o->velY + r;
        if ((DAT_8009d670[0] & DAT_1f8003c8) == 0 || DAT_800a603c != 1 || DAT_800a60dc != 0 || DAT_8009c93f != 0) {
            DAT_800a60fe = 0;
            DAT_8009c330->b0 = 2;
            DAT_8009c330->w22 = 0;
            o->state = 0;
        } else {
            DAT_8009c330->w22++;
        }
        if (DAT_8009c330->w22 > 0x3c) {
            DAT_8009c330->w22 = 0;
            o->state++;
        }
        break;
    case 8:
        SPIN(o);
        o->d8c = 0;
        o->d84 = 0x200;
        o->animFrame = DAT_800a6038.animFrame;
        FUN_800314a8(o);
        AnimJump(&DAT_800a6038, 0);
        if ((DAT_1f8001f8 & 0xf) == 0) {
            FUN_80025f40(0, 0, 0xff, 2);
            SfxPlay2(3, 6);
        }
        AnimJump(&DAT_800a6038, (DAT_8009c330->w22 / 3) & 3);
        o->animFrame = DAT_800a6038.animFrame;
        k = o->animFrame + o->animFrame * 2;
        DAT_8009d600.w0 = 0x10;
        DAT_8009d600.w2 = 4;
        DAT_8009d600.w4 = DAT_8007a070[k];
        o->waa += DAT_8007a072[k];
        if (o->waa > 0xff) o->waa -= 0x100;
        if (o->waa < 0) o->waa += 0x100;
        IDX(o, idx);
        if (o->animFrame & 1) a = 0x80 - DAT_8007a04c[idx]; else a = DAT_8007a04c[idx];
        r = DAT_8007a04c[idx + 1];
        o->velX = DAT_800a6038.h->p.whole + MulCos((unsigned char)a, r);
        o->velY = DAT_800a6038.y.p.whole + MulNegSinScaled((unsigned char)a, r);
        FUN_80031588(&DAT_8009d600, o->waa, &a, &r);
        o->h->p.whole = o->velX + a;
        o->y.p.whole = o->velY + r;
        if ((DAT_8009d670[0] & DAT_1f8003c8) == 0 || DAT_800a603c != 1 || DAT_800a60dc != 0 || DAT_8009c93f != 0) {
            DAT_800a60fe = 0;
            DAT_8009c330->b0 = 1;
            DAT_8009c330->w22 = 0;
            o->state = 0;
        }
        DAT_8009c330->w22++;
        break;
    }
    { char pady[16]; { char padz[16]; } }
    if (o->w22 != 0 && --o->w22 <= 0) o->active = 1;
    if (o->b6a != 0) {
        o->b6a = 0;
        o->w22 = 5;
    }
}
