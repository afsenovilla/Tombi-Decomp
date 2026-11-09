// FUNC 80134070 1164 X001
// MATCHING 80134070 1164
// VERSION SLPS_01144 x01 8013294C 1156
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

extern void *SEGMENTED_CRAWLER_ANIMS[], *SEGMENTED_CRAWLER_GRABBED_ANIMS[];
extern short COS_TABLE[], NEG_SIN_TABLE[];
extern void func_80026BFC(int, int);
extern void playSFX(int);

void func_8013294C(TObj *o)
{
    TObj *p, *e;
    short k;

    if (o->state == 0) {
        o->state++;
        o->d84 = 0;
        func_80026BFC(2, 6);
        playSFX(0x54);
    }
    o->d->p.whole = ((unsigned short*)&SPAD.s17e)[0];
    k = 0x10;
    if ((unsigned int)(o->d88 - 0x40) < 0x80) o->animFrame = 1;
    else o->animFrame = 0;
    if (o->b0c == 0) {
        o->wac = ((o->d84 + 8) & 0xff) >> 4;
        o->anim = SEGMENTED_CRAWLER_ANIMS[o->wac];
    } else {
        o->wac = ((o->d84 + 8) & 0xff) >> 4;
        o->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[o->wac];
    }
    p = o;
    for (e = (TObj *)p->d90; e; e = (TObj *)e->d90) {
        e->h->raw = p->h->raw;
        e->y.raw = p->y.raw;
        e->d->p.whole = ((unsigned short*)&SPAD.s17e)[0];
        if ((e->animFrame = o->animFrame) == 0) {
            e->d84 = (p->d84 + k) & 0xff;
            e->h->raw += COS_TABLE[e->d84] * 3 << 6;
            e->y.raw += NEG_SIN_TABLE[(unsigned char)e->d84] * 3 << 6;
            if (e->b0c == 0) {
                e->wac = ((e->d84 + 8) & 0xff) >> 4;
                e->anim = SEGMENTED_CRAWLER_ANIMS[e->wac];
            } else {
                e->wac = ((e->d84 + 8) & 0xff) >> 4;
                e->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[e->wac];
            }
        } else {
            e->d84 = (p->d84 - k) & 0xff;
            e->h->raw -= COS_TABLE[e->d84] * 3 << 6;
            e->y.raw -= NEG_SIN_TABLE[(unsigned char)e->d84] * 3 << 6;
            if (e->b0c == 0) {
                e->wac = ((-e->d84 - 8) & 0xff) >> 4;
                e->anim = SEGMENTED_CRAWLER_ANIMS[e->wac];
            } else {
                e->wac = ((-e->d84 - 8) & 0xff) >> 4;
                e->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[e->wac];
            }
        }
        p = e;
    }
    p = o;
    for (e = (TObj *)p->d94; e; e = (TObj *)e->d94) {
        e->h->raw = p->h->raw;
        e->y.raw = p->y.raw;
        e->d->p.whole = ((unsigned short*)&SPAD.s17e)[0];
        if ((e->animFrame = o->animFrame) == 0) {
            e->d84 = (p->d84 - k) & 0xff;
            e->h->raw -= COS_TABLE[e->d84] * 3 << 6;
            e->y.raw -= NEG_SIN_TABLE[(unsigned char)e->d84] * 3 << 6;
            e->wac = ((-e->d84 - 8) & 0xff) >> 4;
            e->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[e->wac];
        } else {
            e->d84 = (p->d84 + k) & 0xff;
            e->h->raw += COS_TABLE[e->d84] * 3 << 6;
            e->y.raw += NEG_SIN_TABLE[(unsigned char)e->d84] * 3 << 6;
            e->wac = ((e->d84 + 8) & 0xff) >> 4;
            e->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[e->wac];
        }
        p = e;
    }
}
#else
#include "spad.h"
#include "TOBJ.H"

extern void *SEGMENTED_CRAWLER_ANIMS[], *SEGMENTED_CRAWLER_GRABBED_ANIMS[];
extern short COS_TABLE[], NEG_SIN_TABLE[];
extern void func_80026BFC(int, int);
extern void playSFX(int);

void segmentedCrawlerGrabbed(TObj *o)
{
    TObj *p, *e;
    short k;

    if (o->state == 0) {
        o->state++;
        o->d84 = 0;
        func_80026BFC(2, 6);
        playSFX(0x54);
    }
    o->d->p.whole = ((unsigned short*)&SPAD.s17e)[0];
    k = 0x10;
    if ((unsigned int)((*(TObj **)&o->wa8)->d88 - 0x40) < 0x80) o->animFrame = 1;
    else o->animFrame = 0;
    if (o->b0c == 0) {
        o->wac = ((o->d84 + 8) & 0xff) >> 4;
        o->anim = SEGMENTED_CRAWLER_ANIMS[o->wac];
    } else {
        o->wac = ((o->d84 + 8) & 0xff) >> 4;
        o->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[o->wac];
    }
    p = o;
    for (e = (TObj *)p->d90; e; e = (TObj *)e->d90) {
        e->h->raw = p->h->raw;
        e->y.raw = p->y.raw;
        e->d->p.whole = ((unsigned short*)&SPAD.s17e)[0];
        if ((e->animFrame = o->animFrame) == 0) {
            e->d84 = (p->d84 + k) & 0xff;
            e->h->raw += COS_TABLE[e->d84] * 3 << 6;
            e->y.raw += NEG_SIN_TABLE[(unsigned char)e->d84] * 3 << 6;
            if (e->b0c == 0) {
                e->wac = ((e->d84 + 8) & 0xff) >> 4;
                e->anim = SEGMENTED_CRAWLER_ANIMS[e->wac];
            } else {
                e->wac = ((e->d84 + 8) & 0xff) >> 4;
                e->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[e->wac];
            }
        } else {
            e->d84 = (p->d84 - k) & 0xff;
            e->h->raw -= COS_TABLE[e->d84] * 3 << 6;
            e->y.raw -= NEG_SIN_TABLE[(unsigned char)e->d84] * 3 << 6;
            if (e->b0c == 0) {
                e->wac = ((-e->d84 - 8) & 0xff) >> 4;
                e->anim = SEGMENTED_CRAWLER_ANIMS[e->wac];
            } else {
                e->wac = ((-e->d84 - 8) & 0xff) >> 4;
                e->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[e->wac];
            }
        }
        p = e;
    }
    p = o;
    for (e = (TObj *)p->d94; e; e = (TObj *)e->d94) {
        e->h->raw = p->h->raw;
        e->y.raw = p->y.raw;
        e->d->p.whole = ((unsigned short*)&SPAD.s17e)[0];
        if ((e->animFrame = o->animFrame) == 0) {
            e->d84 = (p->d84 - k) & 0xff;
            e->h->raw -= COS_TABLE[e->d84] * 3 << 6;
            e->y.raw -= NEG_SIN_TABLE[(unsigned char)e->d84] * 3 << 6;
            e->wac = ((-e->d84 - 8) & 0xff) >> 4;
            e->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[e->wac];
        } else {
            e->d84 = (p->d84 + k) & 0xff;
            e->h->raw += COS_TABLE[e->d84] * 3 << 6;
            e->y.raw += NEG_SIN_TABLE[(unsigned char)e->d84] * 3 << 6;
            e->wac = ((e->d84 + 8) & 0xff) >> 4;
            e->anim = SEGMENTED_CRAWLER_GRABBED_ANIMS[e->wac];
        }
        p = e;
    }
}
#endif
