// FUNC 80123e50 212 X001
// MATCHING 80123e50 212
// VERSION SLPM_80165 slpm80165 8003BAB8 196
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x001 80123E50 (func_80123E50.c): subtype 0x11 (PAL 0x13), animation set from the 20-byte item
// table.
extern unsigned char D_8006DDEF[];
#include "TOBJ.H"

extern TObj *allocObjectLayer5(void);
extern unsigned char DAT_8007b084[];
extern unsigned char *DAT_8007b14c[];

void func_80123E50(Fix16 *pos, unsigned char c, short vx)
{
    TObj *o = allocObjectLayer5();

    if (o == 0) return;
    o->active = 1;
    o->type = 6;
    o->subtype = 0x11;
    o->b0c = 0x80;
    {
        unsigned char c2 = D_8006DDEF[o->subtype * 20];
        o->animFrame = 0;
        o->b6b = c;
        o->b0f = c2;
    }
    o->h->raw = pos[0].p.whole << 16;
    o->y.raw = pos[1].p.whole << 16;
    o->d->raw = pos[2].p.whole << 16;
    o->velH = vx;
}
#else
#include "TOBJ.H"

extern TObj *allocObjectLayer5(void);
extern unsigned char DAT_8007b084[];
extern unsigned char *DAT_8007b14c[];

void spawnItemEffectFlyingItem(Fix16 *pos, unsigned char c, short vx)
{
    TObj *o = allocObjectLayer5();

    if (o == 0) return;
    o->active = 1;
    o->type = 6;
    o->subtype = 0x13;
    o->b0c = 0x80;
    {
        unsigned char c2 = DAT_8007b14c[DAT_8007b084[o->subtype]][3];
        o->animFrame = 0;
        o->b6b = c;
        o->b0f = c2;
    }
    o->h->raw = pos[0].p.whole << 16;
    o->y.raw = pos[1].p.whole << 16;
    o->d->raw = pos[2].p.whole << 16;
    o->velH = vx;
}
#endif
