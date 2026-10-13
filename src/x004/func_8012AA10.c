// FUNC 8012aa10 2048 X004
// MATCHING 8012aa10 2048
/* Debt: empty do {} while (0) in cases 1 and 2 ends the sched block so `la D_80011DD8` lands before the lb. */
#include "TOBJ.H"
typedef struct { signed char a; unsigned char b; unsigned char c; signed char d; } E4;
extern TObj PLAYER;
extern unsigned char D_800A60DA;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern E4 D_80011DD8[];
extern signed char D_80011EB4[];
extern void *D_80134CE0;
extern unsigned char D_8009C938, D_8009C93E, D_8009C93F, D_8009C942;
extern short FUN_8001fddc(int, int);
extern short FUN_8001fdac(int, int);
extern int FUN_8001f9e0(void);
extern int showMessageBox(int, int, int, int);

#define ANG(e) \
    if (o->animFrame & 1) { short c = (signed char)e->c; int k = 0x80; ang = (PLAYER.d8c + k - c) & 0xff; } else { short c = (signed char)e->c; ang = (c + PLAYER.d8c) & 0xff; }

#define TRIG() \
    { D_8009C942 = 1; D_800A603C = 5; D_800A603D = 0; D_800A603E = 0; o->state = 1; }

void func_8012AA10(TObj *o)
{
    E4 *e;
    short ang;
    short dx, dy;
    __typeof__(PLAYER.h) p;
    int i; char *t;
    short x;
    unsigned short u;

    o->visible = PLAYER.visible;
    switch (o->state) {
    case 0:
        o->active = 2;
        o->animFrame = PLAYER.animFrame & 1;
        o->category |= 0x80;
        e = &D_80011DD8[D_80011EB4[*(unsigned short *)PLAYER.anim]];
        o->anim = D_80134CE0;
        ANG(e)
        dx = FUN_8001fddc(ang, e->d);
        dy = FUN_8001fdac(ang, e->d);
        PLAYER.w7a = dy;
        o->h->p.whole = PLAYER.h->p.whole + dx;
        o->y.p.whole = PLAYER.y.p.whole + dy;
        o->d->p.whole = PLAYER.d->p.whole;
        o->d8c = PLAYER.d8c;
        o->b0f = PLAYER.b0f + e->b;
        if (D_8009C938) break;
        if (PLAYER.d->p.whole == 0x168) {
            p = PLAYER.h;
            x = p->p.whole;
            if (x < 0x2ac) {
                p->p.whole = 0x2ac;
                if (D_800A603C == 1) TRIG();
                break;
            }
            u = x;
            if (D_800A60DA == 0) break;
            if (x >= 0x5dc) break;
            if ((unsigned short)(u - 0x2b0) < 0x41) break;
            if (D_800A603C == 1) TRIG();
        } else if (PLAYER.d->p.whole == 0x1c2) {
            if (PLAYER.h->p.whole < 0x2ac) {
                PLAYER.h->p.whole = 0x2ac;
                if (D_800A603C == 1) TRIG();
            }
            if (PLAYER.h->p.whole > 0x2f0) {
                PLAYER.h->p.whole = 0x2f0;
                if (D_800A603C == 1) TRIG();
            }
        } else if (PLAYER.d->p.whole == 0x21c) {
            if (D_800A60DA == 1 && D_800A603D != 0x16) {
                if (D_800A603C == 1) TRIG();
                break;
            }
            if ((unsigned char)(D_800A603D - 0xc) < 2 || D_800A603D == 0x1d) {
                PLAYER.h->p.whole = 0x2f0;
                if (D_800A603C == 1) TRIG();
                break;
            }
            if (PLAYER.h->p.whole > 0x2f0) {
                PLAYER.h->p.whole = 0x2f0;
                if (D_800A603C == 1) TRIG();
            }
        } else if (PLAYER.d->p.whole == 0x4ec) {
            if (PLAYER.h->p.whole > 0x448) PLAYER.h->p.whole = 0x448;
            if (D_800A60DA == 1 && D_800A603D != 0x16 && D_800A603C == 1) TRIG();
        } else if (PLAYER.d->p.whole == 0x546) {
            if (D_800A60DA == 1 && (unsigned short)(PLAYER.h->p.whole - 0x230) < 0xc8 && D_800A603C == 1) TRIG();
        }
        break;
    case 1:
        {
            int k = *(unsigned short *)PLAYER.anim;
            t = (char *)D_80011DD8;
            do {} while (0);
            i = D_80011EB4[k];
            i <<= 2;
            e = (E4 *)(t + i);
        }
        if (e->a < 2 && !(FUN_8001f9e0() & 3)) e->a = FUN_8001f9e0() & 1;
        o->anim = D_80134CE0;
        ANG(e)
        dx = FUN_8001fddc(ang, e->d);
        dy = FUN_8001fdac(ang, e->d);
        o->h->p.whole = PLAYER.h->p.whole + dx;
        o->y.p.whole = PLAYER.y.p.whole + dy;
        o->d->p.whole = PLAYER.d->p.whole;
        o->d8c = PLAYER.d8c;
        o->b0f = PLAYER.b0f + e->b;
        if (PLAYER.b69) {
            PLAYER.active = 3;
            o->d90 = showMessageBox(2, 10, 0x80, 0x6c);
            o->state++;
        }
        break;
    case 2:
        {
            int k = *(unsigned short *)PLAYER.anim;
            t = (char *)D_80011DD8;
            do {} while (0);
            i = D_80011EB4[k];
            i <<= 2;
            e = (E4 *)(t + i);
        }
        if (e->a < 2 && !(FUN_8001f9e0() & 3)) e->a = FUN_8001f9e0() & 1;
        o->anim = D_80134CE0;
        ANG(e)
        dx = FUN_8001fddc(ang, e->d);
        dy = FUN_8001fdac(ang, e->d);
        o->h->p.whole = PLAYER.h->p.whole + dx;
        o->y.p.whole = PLAYER.y.p.whole + dy;
        o->d->p.whole = PLAYER.d->p.whole;
        o->d8c = PLAYER.d8c;
        o->b0f = PLAYER.b0f + e->b;
        {
            TObj *p = (TObj *)o->d90;
            if (p->b04 == 2) {
                p->b04 = 3;
                PLAYER.visible = 1;
                if (PLAYER.active == 3) PLAYER.active = 1;
                D_8009C942 = 0;
                D_8009C93E = 0;
                D_8009C93F = 0;
                D_800A603C = 1;
                D_800A603D = 0;
                D_800A603E = 0;
                o->state = 0;
            }
        }
        break;
    }
}
