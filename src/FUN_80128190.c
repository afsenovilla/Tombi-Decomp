// FUNC 80128190 1832 X000
// MATCHING 80128190 1832
#include "TOBJ.H"
typedef struct { short w0; unsigned short w2; short w4; char p6[4]; unsigned char ba, bb; } EX;
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001faf4(TObj *);
extern void FUN_8001fb20(TObj *);
extern void FUN_8001fab4(TObj *);
extern int FUN_80127214(TObj *);
extern int FUN_801274cc(TObj *);
extern int FUN_801275e4(TObj *);
extern unsigned char FUN_80127720(TObj *);
extern void FUN_800eae0c(short, short, short, unsigned char);
extern char DAT_80077cdc[];
extern char DAT_80077d30[];
extern unsigned char DAT_80138fd8[];
extern unsigned short *PTR_DAT_8013a184[];
extern unsigned short *PTR_DAT_8013a18c[];
extern unsigned short *PTR_DAT_8013a194[];
extern unsigned short *PTR_DAT_8013a198[];
extern unsigned short *PTR_DAT_8013a1a4[];

#define SETBOX(a) \
    o->anim = a; { unsigned char *p; \
    p = &DAT_80138fd8[a[1] * 4]; \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p++; \
    o->box3 = *p++; } \
    o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;

static __inline__ void fin(TObj *t)
{
    t->y.p.whole += 2;
    FUN_801274cc(t);
}

void FUN_80128190(TObj *o)
{
    EX *x = (EX *)((char *)o + 0xb4);
    unsigned short *a;
    short s;
    unsigned char r;

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        o->b9c = 0;
        o->substep++;
        FUN_8001f8e4(o);
        o->timer = 0x28;
        o->movetab = DAT_80077cdc;
        o->wac = 9;
        a = PTR_DAT_8013a184[0];
        SETBOX(a);
        fin(o);
        break;
    case 1:
        FUN_80127214(o);
        if (--o->timer != -1) goto move;
        o->timer = 0xc;
        o->b68 = 1;
        o->velH = 0x200;
        o->b69 = 0;
        o->wac = 0x11;
        o->substep++;
        a = PTR_DAT_8013a1a4[0];
        goto common;
    case 2:
        if (--o->timer == -1) o->substep++;
        FUN_80127214(o);
        FUN_8001fb20(o);
        if (o->animFrame != 0) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        if (!FUN_801274cc(o)) goto fall;
        if (FUN_801275e4(o)) {
            o->state = 4;
            o->substep = 0;
        }
        o->velH += 8;
        if (o->velH > 0x250) o->velH = 0x250;
        break;
    case 3:
        o->b9c = 1;
        o->movetab = DAT_80077d30;
        o->velV = -0x300;
        o->b69 = 0;
        o->wac = 0xd;
        o->substep++;
        a = PTR_DAT_8013a194[0];
        SETBOX(a);
    case 4:
        FUN_80127214(o);
        FUN_8001faf4(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->substep++;
        }
        if (!FUN_801275e4(o)) break;
        o->state = 4;
        o->substep = 0;
        break;
    case 5:
        FUN_80127214(o);
        FUN_8001faf4(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (FUN_801274cc(o)) {
            o->d8c = x->w2;
            if (o->visible) {
                if (o->animFrame) x->w4 = -0x10;
                else x->w4 = 0x10;
                FUN_800eae0c(o->a.p.whole + x->w4, o->y.p.whole, o->b.p.whole, 1);
            }
            o->timer = 0xf;
            o->substep++;
        }
        if (!FUN_801275e4(o)) break;
        o->state = 4;
        o->substep = 0;
        break;
    case 6:
        if (--o->timer == -1) o->substep++;
        FUN_80127214(o);
        FUN_8001fab4(o);
        if (!FUN_801274cc(o)) goto fall;
        if (o->wb2 != 0) {
            int g = (o->wb2 > 0);
            g ^= o->animFrame;
            if (g) o->timer = 0xf;
        }
        if (!FUN_801275e4(o)) break;
        o->state = 4;
        o->substep = 0;
        break;
    case 7:
        o->b68 = 0;
        o->velH = 0x250;
        o->substep++;
    case 8:
        FUN_80127214(o);
        FUN_8001fb20(o);
        if (o->animFrame != 0) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        FUN_801275e4(o);
        if (FUN_801274cc(o)) {
            s = o->wb2;
            if (s != 0) {
                int g = (s > 0);
                g ^= o->animFrame;
                if (!g) {
                    short t = s;
                    if (t < 0) t = -t;
                    o->velH -= t;
                }
            }
        } else {
        fall:
            x->ba = 1;
            x->bb = 2;
            o->state = 7;
            o->substep = 0;
            break;
        }
        if ((o->velH -= 0x10) < 0) o->substep++;
        break;
    case 9:
        o->timer = 0x78;
        o->wac = 0xe;
        o->substep++;
        a = PTR_DAT_8013a198[0];
        SETBOX(a);
    case 10:
        FUN_80127214(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0xb;
            o->substep++;
            a = PTR_DAT_8013a18c[0];
        common:
            SETBOX(a);
        }
        goto move;
    case 11:
        FUN_80127214(o);
        if (--o->timer == -1) {
            o->state = 0;
            o->substep = 0;
            r = FUN_80127720(o);
            if (r == 0xff) break;
            o->state = r;
        }
    move:
        fin(o);
        break;
    }
}
