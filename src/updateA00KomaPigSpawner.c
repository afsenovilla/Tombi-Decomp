// FUNC 8011a63c 264 X000
// MATCHING 8011a63c 264
// VERSION SLPM_80165 x00 801130F0 260
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 8011A63C (FUN_8011a63c.c): +0x98 starts at 5 (PAL 10), +0x9a not cleared.
#include "TOBJ.H"
extern int FUN_800202b4(TObj *);
extern void a00KomaPigSpawnerStep(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_800187e4(TObj *);
extern void *A00_KOMA_PIG_SPAWNER_ANIMS[];
extern int DAT_1f8002d4[];
void FUN_8011a63c(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w98 = 5;
        o->w1e = 0xc;
        *(signed char *)&o->b0f = -5;
        o->b0d = 0;
        o->b0a = 0;
        o->animFrame = 0;
        o->b04 = o->b04 + 1;
        o->anim = A00_KOMA_PIG_SPAWNER_ANIMS[o->subtype];
        o->d3c = DAT_1f8002d4[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) != 0 && o->subtype == 0)
            a00KomaPigSpawnerStep(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
#else
#include "TOBJ.H"
extern int FUN_800202b4(TObj *);
extern void a00KomaPigSpawnerStep(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_800187e4(TObj *);
extern void *A00_KOMA_PIG_SPAWNER_ANIMS[];
extern int DAT_1f8002d4[];
void updateA00KomaPigSpawner(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w98 = 10;
        o->w1e = 0xc;
        *(signed char *)&o->b0f = -5;
        o->w9a = 0;
        o->b0d = 0;
        o->b0a = 0;
        o->animFrame = 0;
        o->b04 = o->b04 + 1;
        o->anim = A00_KOMA_PIG_SPAWNER_ANIMS[o->subtype];
        o->d3c = DAT_1f8002d4[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) != 0 && o->subtype == 0)
            a00KomaPigSpawnerStep(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
#endif
