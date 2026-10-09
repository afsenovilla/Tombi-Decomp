// FUNC 8012aa10 2048 X004
/* score 24: case 0 matches; cases 1 and 2 differ only in the table-entry setup: game loads la D_80011DD8 into a0
   first and the lb index into v1 (sll in place); ours lb v0, sll v1, la v0. The function-scope `int i` (global
   pseudo) fixed the size; tried in-place shifts, char/int bases, table pointer locals. Same attach code as wip X001
   func_80135164: (signed char)e->c through a short local + `int k = 0x80`, ang as short masked with 0xff. */
#include "TOBJ.H"
typedef struct { signed char a; unsigned char b; unsigned char c; signed char d; } E4;
extern TObj D_800A6038;
extern unsigned char D_800A60DA;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern E4 D_80011DD8[];
extern signed char D_80011EB4[];
extern void *D_80134CE0;
extern unsigned char D_8009C938, D_8009C93E, D_8009C93F, D_8009C942;
extern short FUN_8001fddc(int, int);
extern short FUN_8001fdac(int, int);
extern int FUN_8001f9e0(void);
extern int FUN_8002dc50(int, int, int, int);

#define ANG(e) \
    if (o->animFrame & 1) { short c = (signed char)e->c; int k = 0x80; ang = (D_800A6038.d8c + k - c) & 0xff; } else { short c = (signed char)e->c; ang = (c + D_800A6038.d8c) & 0xff; }

#define TRIG() \
    { D_8009C942 = 1; D_800A603C = 5; D_800A603D = 0; D_800A603E = 0; o->state = 1; }

void func_8012AA10(TObj *o)
{
    E4 *e;
    short ang;
    short dx, dy;
    __typeof__(D_800A6038.h) p;
    int i;
    short x;
    unsigned short u;

    o->visible = D_800A6038.visible;
    switch (o->state) {
    case 0:
        o->active = 2;
        o->animFrame = D_800A6038.animFrame & 1;
        o->category |= 0x80;
        e = &D_80011DD8[D_80011EB4[*(unsigned short *)D_800A6038.anim]];
        o->anim = D_80134CE0;
        ANG(e)
        dx = FUN_8001fddc(ang, e->d);
        dy = FUN_8001fdac(ang, e->d);
        D_800A6038.w7a = dy;
        o->h->p.whole = D_800A6038.h->p.whole + dx;
        o->y.p.whole = D_800A6038.y.p.whole + dy;
        o->d->p.whole = D_800A6038.d->p.whole;
        o->d8c = D_800A6038.d8c;
        o->b0f = D_800A6038.b0f + e->b;
        if (D_8009C938) break;
        if (D_800A6038.d->p.whole == 0x168) {
            p = D_800A6038.h;
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
        } else if (D_800A6038.d->p.whole == 0x1c2) {
            if (D_800A6038.h->p.whole < 0x2ac) {
                D_800A6038.h->p.whole = 0x2ac;
                if (D_800A603C == 1) TRIG();
            }
            if (D_800A6038.h->p.whole > 0x2f0) {
                D_800A6038.h->p.whole = 0x2f0;
                if (D_800A603C == 1) TRIG();
            }
        } else if (D_800A6038.d->p.whole == 0x21c) {
            if (D_800A60DA == 1 && D_800A603D != 0x16) {
                if (D_800A603C == 1) TRIG();
                break;
            }
            if ((unsigned char)(D_800A603D - 0xc) < 2 || D_800A603D == 0x1d) {
                D_800A6038.h->p.whole = 0x2f0;
                if (D_800A603C == 1) TRIG();
                break;
            }
            if (D_800A6038.h->p.whole > 0x2f0) {
                D_800A6038.h->p.whole = 0x2f0;
                if (D_800A603C == 1) TRIG();
            }
        } else if (D_800A6038.d->p.whole == 0x4ec) {
            if (D_800A6038.h->p.whole > 0x448) D_800A6038.h->p.whole = 0x448;
            if (D_800A60DA == 1 && D_800A603D != 0x16 && D_800A603C == 1) TRIG();
        } else if (D_800A6038.d->p.whole == 0x546) {
            if (D_800A60DA == 1 && (unsigned short)(D_800A6038.h->p.whole - 0x230) < 0xc8 && D_800A603C == 1) TRIG();
        }
        break;
    case 1:
        i = D_80011EB4[*(unsigned short *)D_800A6038.anim] * 4;
        e = (E4 *)((char *)D_80011DD8 + i);
        if (e->a < 2 && !(FUN_8001f9e0() & 3)) e->a = FUN_8001f9e0() & 1;
        o->anim = D_80134CE0;
        ANG(e)
        dx = FUN_8001fddc(ang, e->d);
        dy = FUN_8001fdac(ang, e->d);
        o->h->p.whole = D_800A6038.h->p.whole + dx;
        o->y.p.whole = D_800A6038.y.p.whole + dy;
        o->d->p.whole = D_800A6038.d->p.whole;
        o->d8c = D_800A6038.d8c;
        o->b0f = D_800A6038.b0f + e->b;
        if (D_800A6038.b69) {
            D_800A6038.active = 3;
            o->d90 = FUN_8002dc50(2, 10, 0x80, 0x6c);
            o->state++;
        }
        break;
    case 2:
        i = D_80011EB4[*(unsigned short *)D_800A6038.anim] * 4;
        e = (E4 *)((char *)D_80011DD8 + i);
        if (e->a < 2 && !(FUN_8001f9e0() & 3)) e->a = FUN_8001f9e0() & 1;
        o->anim = D_80134CE0;
        ANG(e)
        dx = FUN_8001fddc(ang, e->d);
        dy = FUN_8001fdac(ang, e->d);
        o->h->p.whole = D_800A6038.h->p.whole + dx;
        o->y.p.whole = D_800A6038.y.p.whole + dy;
        o->d->p.whole = D_800A6038.d->p.whole;
        o->d8c = D_800A6038.d8c;
        o->b0f = D_800A6038.b0f + e->b;
        {
            TObj *p = (TObj *)o->d90;
            if (p->b04 == 2) {
                p->b04 = 3;
                D_800A6038.visible = 1;
                if (D_800A6038.active == 3) D_800A6038.active = 1;
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
