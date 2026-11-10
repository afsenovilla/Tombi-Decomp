// VERSION SLPM_80165 x00 80112EE8 520
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of a00KomaPigSpawnerStep (src/a00KomaPigSpawnerStep.c): no live-pig count (the spawn does not
// wait for the previous pig), the remaining count stops at 0.
#include "TOBJ.H"
extern void FUN_8001fec0(TObj *);
extern void FUN_8001fe6c(TObj *);
extern TObj *allocObjectLayer2(void);
extern unsigned short TICK_COUNT;
extern unsigned short PLAYER_D_WHOLE;
extern unsigned short PLAYER_H_WHOLE;
extern char D_8013BA48[];
extern char D_8013BA80[];
extern char D_8013BA88[];
extern char D_8013BA90[];
static void *DAT_8013b214[4] = {
    D_8013BA48, D_8013BA80, D_8013BA88, D_8013BA90,
};

void a00KomaPigSpawnerStep(TObj *o)
{
    TObj *n;
    int ok;

    switch (o->step) {
    case 0:
        if ((TICK_COUNT & 3) == 0) {
            if ((unsigned short)(o->d->p.whole - PLAYER_D_WHOLE + 0x2d) >= 0x5b) {
                ok = 0;
            } else {
                ok = (unsigned short)(o->h->p.whole - PLAYER_H_WHOLE + 0x60) < 0xc1;
            }
            if (ok) {
                o->timer = 0x29;
                o->step++;
            }
        }
        break;
    case 1:
        FUN_8001fec0(o);
        if (--o->timer == -1) {
            n = allocObjectLayer2();
            if (n != 0) {
                n->active = 2;
                n->type = 2;
                n->h->raw = o->h->p.whole << 16;
                n->y.raw = o->y.p.whole << 16;
                n->d->raw = 0x5a0000;
                n->subtype = 5;
                n->b1d = o->b1d;
                n->animFrame = 0;
            }
            if (--o->w98 < 0)
                o->w98 = 0;
            o->timer = 2000;
            o->step++;
        }
        break;
    case 2:
        if (--o->timer == -1) {
            if (o->w98 == 0) {
                o->step++;
            } else {
                o->step = 0;
            }
            o->anim = DAT_8013b214[o->subtype];
            FUN_8001fe6c(o);
        }
        break;
    }
}
#endif
