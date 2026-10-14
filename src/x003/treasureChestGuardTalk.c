// FUNC 8012b66c 1612 X003
// MATCHING 8012b66c 1612
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80135DD8[];
extern unsigned char D_8009CE58[]; /* debt: second name for GCD.bce5d so chk() re-reads it */
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern short FUN_8004065c(TObj *, short, short, short);
extern short FUN_80040278(TObj *, short, short);

#define WC2(o) (*(unsigned short *)((char *)(o) + 0xc2))

static __inline__ void setanim(TObj *o, int n)
{
    o->anim = D_80135DD8[o->subtype].anims[n];
    FUN_8001fe94(o, 0);
}

static __inline__ int chk(void)
{
    return GCD.bd0c8 && D_8009CE58[5] != 0xff;
}

void treasureChestGuardTalk(TObj *o)
{
    V6 v;
    TObj *p;
    unsigned char k;

    FUN_8001fec0(o);
    switch (o->state) {
    case 0:
        if (GCD.bce5d == 0xff) {
            o->b68 = 0;
            goto reset;
        }
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->animFrame = o->b68 & 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        o->state++;
        break;
    case 1:
        ((unsigned char*)&GVARS.b3e)[0] = 1;
        o->state = 10;
        break;
    case 10:
        k = GCD.bce5d;
        if (k == 0xff) {
            o->state = 0x5a;
            WC2(o) = 0x7a;
            break;
        }
        if (chk()) {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 1, &v);
            o->state = 0x5a;
            WC2(o) = 0x46;
        } else if (k == 0) {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 0, &v);
            o->state = 0x5a;
            WC2(o) = 0x32;
        } else {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 0, &v);
            o->state = 0x5a;
            WC2(o) = 0x7a;
        }
        break;
    case 0x33:
        break;
    case 0x32:
        setEventStarted(0xb9, 0, 0);
        o->state = 0x78;
        break;
    case 0x46:
        setanim(o, 2);
        o->velX = 0x100;
        o->velY = -0x400;
        o->animFrame = 0;
        o->b69 = 0;
        o->state = 0x47;
        break;
    case 0x47:
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        FUN_8004065c(o, o->h->p.whole, o->y.p.whole, o->animFrame);
        if (o->velY > 0) o->state = 0x48;
        goto vis;
    case 0x48:
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        FUN_8004065c(o, o->h->p.whole, o->y.p.whole, o->animFrame);
        if (FUN_80040278(o, o->h->p.whole, o->y.p.whole)) o->state = 0x46;
    vis:
        if (!o->visible) o->state = 0x49;
        break;
    case 0x49:
        setEventComplete(0xb9, 0);
        o->state = 0x78;
        WC2(o) = 0xfa;
        break;
    case 0x5a:
        setanim(o, 1);
        o->state = 0x5b;
        break;
    case 0x5b:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setanim(o, 0);
        o->state = WC2(o);
        break;
    case 0x6e:
        o->timer = 200;
        o->state = 0x6f;
        break;
    case 0x6f:
        if (--o->timer == 0) o->state = WC2(o);
        break;
    case 0x78:
        o->timer = 200;
        o->state = 0x79;
        break;
    case 0x7b:
        if (o->timer <= 0) o->timer = 4;
        o->state = 0x79;
        break;
    case 0x79:
        if (--o->timer == 0) o->state = 0x7a;
        break;
    case 0x7a:
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        if (o->d88 == 0) {
            PLAYER.obj.wb2 = 0;
            PLAYER.obj.b04 = 1;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
        }
        o->b68 = 0;
        if (WC2(o) == 0xf0) {
            o->state = 0xf0;
        } else if (WC2(o) == 0xfa) {
            o->state = 0xfa;
        } else {
        reset:
            o->step = 0;
            o->state = 0;
        }
        break;
    case 0xf0:
        o->anim = 0;
        break;
    case 0xfa:
        o->b04 = 2;
        break;
    }
}
