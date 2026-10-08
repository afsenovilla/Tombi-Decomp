// FUNC 800ebf68 1040 X016
// MATCHING 800ebf68 1040
#include "TOBJ.H"
typedef struct T12 { void **p; int a, b; } T12;
extern T12 DAT_80114c24[];
extern unsigned char DAT_800a6039;
extern unsigned char DAT_800a6047a[];
#define DAT_800a6047 DAT_800a6047a[0]
extern void FUN_8001fe6c(TObj *);
extern int func_8001fec0(TObj *o);
extern int FUN_8001fe3c(short, short);
extern int FUN_8001fe0c(short, short);
extern int FUN_8001fdac(short, short);
extern int FUN_8001fddc(short, short);
extern unsigned FUN_8001f9e0(void);
extern void FUN_800ec5f4(TObj *o, int a, int b, int c);

void FUN_800ebf68(TObj *o)
{
    TObj *q;
    short d;
    int r;
    short c;
    int b;
    int v;
    int x;
    short a;

    switch (o->state) {
    case 0:
        o->anim = *DAT_80114c24[o->subtype].p;
        FUN_8001fe6c(o);
        o->visible = DAT_800a6039;
        o->b0f = DAT_800a6047 - 1;
        {
            short t = 8;
            if (o->b0c & 2) t = 4;
            q = (TObj *)o->d90;
            o->velX = t;
        }
        o->velY = 0;
        switch (q->animFrame) {
        case 0:
        case 2:
            o->w74 = 0;
            break;
        case 1:
        case 3:
            o->w74 = 0x7f;
            break;
        case 4:
            o->w74 = 0x20;
            break;
        case 5:
            o->w74 = 0x60;
            break;
        case 6:
        case 7:
            o->w74 = 0x40;
            break;
        }
        o->state++;
    case 1:
        func_8001fec0(o);
        q = (TObj *)o->d90;
        switch (q->state) {
        case 0:
            o->b04 = 2;
        case 1:
            o->visible = 0;
            break;
        case 2:
        case 3:
        case 4:
            o->visible = 0;
            o->b0f = DAT_800a6047 - 1;
            a = q->waa - o->w74;
            if (a != 0) {
                if ((unsigned short)a < 0x80)
                    o->w74 = (o->w74 + 0x10) & 0xff;
                else
                    o->w74 = (o->w74 - 0x10) & 0xff;
            }
            r = 8;
            a = (o->w74 + 0x80) & 0xff;
            if (o->b0c & 2) r = 4;
            o->velH = FUN_8001fe3c(a, r);
            o->velV = FUN_8001fe0c(a, r);
            a = (o->b0c & 1 ? o->w74 + 0x40 : o->w74 + 0xc0) & 0xff;
            r = FUN_8001fdac(o->velY, o->velX);
            o->velY = (o->velY + 0x10) & 0xff;
            x = FUN_8001fe3c(a, r);
            v = FUN_8001fe0c(a, r);
            o->h->p.whole = x + (q->h->p.whole + o->velH);
            o->y.p.whole = v + (q->y.p.whole + o->velV);
            o->d->p.whole = q->d->p.whole;
            o->animTimer = 6;
            FUN_800ec5f4(o, 0, 0, 0);
            if (q->wa8 < 0x600) {
                o->animTimer = 6;
                if (FUN_8001f9e0() & 1)
                    FUN_800ec5f4(o, 4, 4, 0);
                else
                    FUN_800ec5f4(o, -4, -4, 0);
            }
            break;
        case 7:
        case 8:
            c = (o->w76 + 0x10) & 0xff;
            o->visible = DAT_800a6039;
            o->b0f = DAT_800a6047 - 1;
            o->w76 = c;
            if (o->b0c & 1) {
                v = FUN_8001fddc(c, 8);
                o->h->p.whole = q->h->p.whole + v;
                v = FUN_8001fdac(c, 0x10);
                o->y.p.whole = q->y.p.whole + v;
                v = FUN_8001fdac(c, 8);
                o->d->p.whole = q->d->p.whole + v;
            } else {
                v = FUN_8001fddc(c, 8);
                o->h->p.whole = q->h->p.whole + v;
                v = FUN_8001fdac((c + 0x80) & 0xff, 0x10);
                o->y.p.whole = q->y.p.whole + v;
                v = FUN_8001fdac(c, 8);
                o->d->p.whole = q->d->p.whole + v;
            }
            break;
        }
        switch (q->step) {
        case 0 ... 6:
            break;
        default:
            o->b04 = 2;
        }
        if (q->b04 != 1)
            o->b04 = 2;
        break;
    }
}
