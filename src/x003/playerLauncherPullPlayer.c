// FUNC 8011c3d8 1060 X003
// MATCHING 8011c3d8 1060
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
extern TObj PLAYER;
extern short D_8007A1F0[], D_8007A5F0[];
extern short D_80078DB4[];
extern void *D_8013987C[];
extern void AnimLoadDuration(TObj *);
extern int func_800205D8(int, int);

void playerLauncherPullPlayer(TObj *o)
{
    TObj *pl = &PLAYER;
    TObj *q = (TObj *)o->d90;
    unsigned char s = o->state;
    short a;
    int dx, dy, d;

    switch (s) {
    case 0:
        o->state = s + 1;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        o->velH = 0x800;
        if (o->b0c & 4) {
            o->state = 2;
            break;
        }
        q->step = 7;
        q->d38 = 0x40;
        q->state = 0;
        q->b0a = 0;
        q->wac = 6;
        q->y.p.whole += 0x30;
        q->anim = D_8013987C[0];
        AnimLoadDuration(q);
        break;
    case 1:
        a = func_800205D8(q->h->p.whole - PLAYER.h->p.whole, q->y.p.whole - PLAYER.y.p.whole) & 0xff;
        o->wb0 = a;
        dy = (o->velH * D_8007A1F0[a]) >> 12;
        dx = (o->velH * D_8007A5F0[a]) >> 12;
        {
            Fix16 *pd = PLAYER.d;
            short v = pd->p.whole;
            d = q->d->p.whole - v;
            if (d != 0) {
                if (d > 0) {
                    pd->p.whole = v + 4;
                    if (q->d->p.whole < (short)(v + 4))
                        PLAYER.d->p.whole = q->d->p.whole;
                } else {
                    pd->p.whole = v - 4;
                    if ((short)(v - 4) < q->d->p.whole)
                        PLAYER.d->p.whole = q->d->p.whole;
                }
            }
        }
        pl->h->raw += dx << 8;
        pl->y.raw += dy << 8;
        dx = pl->h->p.whole - q->h->p.whole + 8;
        if ((unsigned short)dx > 0x20)
            break;
        o->velH = 0x400;
        dx = pl->y.p.whole - q->y.p.whole + 8;
        if ((unsigned short)dx > 0x20)
            break;
        q->state = 1;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        o->step = 0;
        o->state = 0;
        pl->h->p.whole = q->h->p.whole;
        pl->y.p.whole = q->y.p.whole;
        pl->d->p.whole = q->d->p.whole;
        o->b0a = 2;
        o->y.p.whole -= 0x30;
        break;
    case 2:
        a = func_800205D8(D_80078DB4[0] - PLAYER.h->p.whole, D_80078DB4[1] - PLAYER.y.p.whole) & 0xff;
        o->wb0 = a;
        dy = (o->velH * D_8007A1F0[a]) >> 12;
        dx = (o->velH * D_8007A5F0[a]) >> 12;
        PLAYER.h->raw += dx << 8;
        PLAYER.y.raw += dy << 8;
        dx = PLAYER.h->p.whole - D_80078DB4[0] + 0x10;
        if ((unsigned short)dx > 0x20)
            break;
        o->velH = 0x400;
        dx = PLAYER.y.p.whole - D_80078DB4[1] + 0x10;
        if ((unsigned short)dx > 0x20)
            break;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        o->step = 0;
        o->state = 0;
        GV2.bc2 = 1;
        PLAYER.h->p.whole = 0xb93;
        PLAYER.y.p.whole = -0x484;
        o->b0a = 2;
        o->y.p.whole -= 0x30;
        break;
    }
}
