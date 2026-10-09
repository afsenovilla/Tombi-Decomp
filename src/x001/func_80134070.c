// FUNC 80134070 1164 X001
// MATCHING 80134070 1164
#include "TOBJ.H"

extern void *D_8013F034[], *D_8013F074[];
extern short D_8007A5F0[], D_8007A3F0[];
extern unsigned short D_1F80017E[];
extern void func_80026BFC(int, int);
extern void playSFX(int);

void func_80134070(TObj *o)
{
    TObj *p, *e;
    short k;

    if (o->state == 0) {
        o->state++;
        o->d84 = 0;
        func_80026BFC(2, 6);
        playSFX(0x54);
    }
    o->d->p.whole = D_1F80017E[0];
    k = 0x10;
    if ((unsigned int)((*(TObj **)&o->wa8)->d88 - 0x40) < 0x80) o->animFrame = 1;
    else o->animFrame = 0;
    if (o->b0c == 0) {
        o->wac = ((o->d84 + 8) & 0xff) >> 4;
        o->anim = D_8013F034[o->wac];
    } else {
        o->wac = ((o->d84 + 8) & 0xff) >> 4;
        o->anim = D_8013F074[o->wac];
    }
    p = o;
    for (e = (TObj *)p->d90; e; e = (TObj *)e->d90) {
        e->h->raw = p->h->raw;
        e->y.raw = p->y.raw;
        e->d->p.whole = D_1F80017E[0];
        if ((e->animFrame = o->animFrame) == 0) {
            e->d84 = (p->d84 + k) & 0xff;
            e->h->raw += D_8007A5F0[e->d84] * 3 << 6;
            e->y.raw += D_8007A3F0[(unsigned char)e->d84] * 3 << 6;
            if (e->b0c == 0) {
                e->wac = ((e->d84 + 8) & 0xff) >> 4;
                e->anim = D_8013F034[e->wac];
            } else {
                e->wac = ((e->d84 + 8) & 0xff) >> 4;
                e->anim = D_8013F074[e->wac];
            }
        } else {
            e->d84 = (p->d84 - k) & 0xff;
            e->h->raw -= D_8007A5F0[e->d84] * 3 << 6;
            e->y.raw -= D_8007A3F0[(unsigned char)e->d84] * 3 << 6;
            if (e->b0c == 0) {
                e->wac = ((-e->d84 - 8) & 0xff) >> 4;
                e->anim = D_8013F034[e->wac];
            } else {
                e->wac = ((-e->d84 - 8) & 0xff) >> 4;
                e->anim = D_8013F074[e->wac];
            }
        }
        p = e;
    }
    p = o;
    for (e = (TObj *)p->d94; e; e = (TObj *)e->d94) {
        e->h->raw = p->h->raw;
        e->y.raw = p->y.raw;
        e->d->p.whole = D_1F80017E[0];
        if ((e->animFrame = o->animFrame) == 0) {
            e->d84 = (p->d84 - k) & 0xff;
            e->h->raw -= D_8007A5F0[e->d84] * 3 << 6;
            e->y.raw -= D_8007A3F0[(unsigned char)e->d84] * 3 << 6;
            e->wac = ((-e->d84 - 8) & 0xff) >> 4;
            e->anim = D_8013F074[e->wac];
        } else {
            e->d84 = (p->d84 + k) & 0xff;
            e->h->raw += D_8007A5F0[e->d84] * 3 << 6;
            e->y.raw += D_8007A3F0[(unsigned char)e->d84] * 3 << 6;
            e->wac = ((e->d84 + 8) & 0xff) >> 4;
            e->anim = D_8013F074[e->wac];
        }
        p = e;
    }
}
