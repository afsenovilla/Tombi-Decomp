// FUNC 80135164 1372 X001
// MATCHING 80135164 1372
#include "gvars.h"
/* Match keys: the angle as in matched X009 func_8011B01C (`u = ...; t = u; t &= 0xff;`) with t pinned to $2 and
   ang an int pinned to $16 behind an empty asm (game `andi v0; move s0,v0`), dx pinned to $19; two inline copies
   because only case 0 stores PLAYER.w7a (D_800A60B2) with dy in $4; case 1 computes the D_80011DD8 base first
   (pinned $4 behind an asm barrier) and adds the index*4 in $3. */
#include "TOBJ.H"
typedef struct { signed char a; unsigned char b; unsigned char c; signed char d; } E4;
extern TObj PLAYER;
extern E4 D_80011DD8[];
extern signed char D_80011EB4[];
extern char D_8013E054[];
extern char D_8013E05C[];
extern char D_8013E064[];
extern char D_8013E06C[];
static void *D_8013DE4C[4] = {
    D_8013E054, D_8013E05C, D_8013E064, D_8013E06C,
};
extern void *D_8013DDF8[];
extern unsigned short GAME, D_8009C962;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern int FUN_8001f9e0(void);
extern short FUN_8001fddc(int, int);
extern short FUN_8001fdac(int, int);
extern int showMessageBox(int, int, int, int);

static __inline__ void follow1(TObj *o, E4 *e)
{
    register int ang asm("$16");
    register short dx asm("$19");
    register short dy asm("$4");
    unsigned short u;
    register unsigned int t asm("$2");

    if (GAME == 1 && D_8009C962 < 2)
        o->anim = D_8013DE4C[e->a];
    else
        o->anim = D_8013DDF8[e->a];
    if (o->animFrame & 1) {
        u = PLAYER.d8c + 0x80 - (signed char)e->c;
        t = u; t &= 0xff;
    } else {
        u = (signed char)e->c + PLAYER.d8c;
        t = u; t &= 0xff;
    }
    ang = t; asm("");
    dx = FUN_8001fddc(ang, e->d);
    dy = FUN_8001fdac(ang, e->d);
    PLAYER.w7a = dy;
    o->h->p.whole = PLAYER.h->p.whole + dx;
    o->y.p.whole = PLAYER.y.p.whole + dy;
    o->d->p.whole = PLAYER.d->p.whole;
    o->d8c = PLAYER.d8c;
    o->b0f = PLAYER.b0f + e->b;
}

static __inline__ void follow0(TObj *o, E4 *e)
{
    register int ang asm("$16");
    register short dx asm("$19");
    short dy;
    unsigned short u;
    register unsigned int t asm("$2");

    if (GAME == 1 && D_8009C962 < 2)
        o->anim = D_8013DE4C[e->a];
    else
        o->anim = D_8013DDF8[e->a];
    if (o->animFrame & 1) {
        u = PLAYER.d8c + 0x80 - (signed char)e->c;
        t = u; t &= 0xff;
    } else {
        u = (signed char)e->c + PLAYER.d8c;
        t = u; t &= 0xff;
    }
    ang = t; asm("");
    dx = FUN_8001fddc(ang, e->d);
    dy = FUN_8001fdac(ang, e->d);
    o->h->p.whole = PLAYER.h->p.whole + dx;
    o->y.p.whole = PLAYER.y.p.whole + dy;
    o->d->p.whole = PLAYER.d->p.whole;
    o->d8c = PLAYER.d8c;
    o->b0f = PLAYER.b0f + e->b;
}

void wrongWayHintTalk(TObj *o)
{
    E4 *e;
    TObj *p;

    o->visible = PLAYER.visible;
    switch (o->state) {
    case 0:
        o->active = 2;
        o->animFrame = PLAYER.animFrame & 1;
        o->category |= 0x80;
        e = &D_80011DD8[D_80011EB4[*(unsigned short *)PLAYER.anim]];
        follow1(o, e);
        if (GVARS.b38) break;
        switch (D_8009C962) {
        case 1:
            if (PLAYER.h->p.whole >= 0x1000) break;
            PLAYER.h->p.whole = 0x1000;
            goto set;
        case 3:
            if ((unsigned short)(PLAYER.y.p.whole + 0x2d6) < 0x48) {
                if ((unsigned short)(PLAYER.h->p.whole - 0x7f0) >= 0x20) break;
                PLAYER.h->p.whole = 0x7ee;
            } else {
                if (PLAYER.h->p.whole < 0xad3) break;
                PLAYER.h->p.whole = 0xad2;
            }
        set:
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            o->state = 1;
            break;
        }
        break;
    case 1: {
        register E4 *tb asm("$4") = D_80011DD8;
        asm("" : "=r"(tb) : "0"(tb));
        { register int ix asm("$3") = D_80011EB4[*(unsigned short *)PLAYER.anim] * 4; e = (E4 *)(ix + (int)tb); } }
        if (e->a < 2 && (FUN_8001f9e0() & 3) == 0)
            e->a = FUN_8001f9e0() & 1;
        follow0(o, e);
        if (PLAYER.b69) {
            GVARS.b42 = 1;
            o->d90 = showMessageBox(2, 0xc, 0x80, 0x6c);
            o->state = 2;
        }
        break;
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
            GVARS.b3f = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            o->state = 0;
        }
        break;
    }
}
