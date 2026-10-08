// FUNC 8011c3d8 1060 X003
// MATCHING 8011c3d8 1060
#include "TOBJ.H"
extern TObj D_800A6038;
extern unsigned char D_8009C93F, D_8009C942, D_8009D2C2;
extern short D_8007A1F0[], D_8007A5F0[];
extern short D_80078DB4[];
extern void *D_8013987C[];
extern void AnimLoadDuration(TObj *);
extern int func_800205D8(int, int);

void func_8011C3D8(TObj *o)
{
    TObj *pl = &D_800A6038;
    TObj *q = (TObj *)o->d90;
    unsigned char s = o->state;
    short a;
    int dx, dy, d;

    switch (s) {
    case 0:
        o->state = s + 1;
        D_8009C93F = 1;
        D_8009C942 = 1;
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
        a = func_800205D8(q->h->p.whole - D_800A6038.h->p.whole, q->y.p.whole - D_800A6038.y.p.whole) & 0xff;
        o->wb0 = a;
        dy = (o->velH * D_8007A1F0[a]) >> 12;
        dx = (o->velH * D_8007A5F0[a]) >> 12;
        {
            Fix16 *pd = D_800A6038.d;
            short v = pd->p.whole;
            d = q->d->p.whole - v;
            if (d != 0) {
                if (d > 0) {
                    pd->p.whole = v + 4;
                    if (q->d->p.whole < (short)(v + 4))
                        D_800A6038.d->p.whole = q->d->p.whole;
                } else {
                    pd->p.whole = v - 4;
                    if ((short)(v - 4) < q->d->p.whole)
                        D_800A6038.d->p.whole = q->d->p.whole;
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
        D_8009C93F = 0;
        D_8009C942 = 0;
        o->step = 0;
        o->state = 0;
        pl->h->p.whole = q->h->p.whole;
        pl->y.p.whole = q->y.p.whole;
        pl->d->p.whole = q->d->p.whole;
        o->b0a = 2;
        o->y.p.whole -= 0x30;
        break;
    case 2:
        a = func_800205D8(D_80078DB4[0] - D_800A6038.h->p.whole, D_80078DB4[1] - D_800A6038.y.p.whole) & 0xff;
        o->wb0 = a;
        dy = (o->velH * D_8007A1F0[a]) >> 12;
        dx = (o->velH * D_8007A5F0[a]) >> 12;
        D_800A6038.h->raw += dx << 8;
        D_800A6038.y.raw += dy << 8;
        dx = D_800A6038.h->p.whole - D_80078DB4[0] + 0x10;
        if ((unsigned short)dx > 0x20)
            break;
        o->velH = 0x400;
        dx = D_800A6038.y.p.whole - D_80078DB4[1] + 0x10;
        if ((unsigned short)dx > 0x20)
            break;
        D_8009C93F = 0;
        D_8009C942 = 0;
        o->step = 0;
        o->state = 0;
        D_8009D2C2 = 1;
        D_800A6038.h->p.whole = 0xb93;
        D_800A6038.y.p.whole = -0x484;
        o->b0a = 2;
        o->y.p.whole -= 0x30;
        break;
    }
}
