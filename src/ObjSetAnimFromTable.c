// FUNC 800efc04 136 X000
// MATCHING 800efc04 136
// VERSION SLPM_80165 x00 800EDA04 120
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 800EFC04 (ObjSetAnimFromTable.c):  56 player animations (PAL 90).
typedef struct O { char p[0x24]; int anim; } O;
typedef struct P { char p[0x2c]; unsigned short f; } P;
extern P *PLAYER_EXT;
typedef struct { int a[56]; } AnimTable;
extern AnimTable D_800E8404; /* shared with FUN_800fe13c (one table in the retail .rodata) */
void ObjSetAnimFromTable(O *o)
{
    AnimTable t = D_800E8404;
    o->anim = t.a[PLAYER_EXT->f];
}
#else
typedef struct O { char p[0x24]; int anim; } O;
typedef struct P { char p[0x2c]; unsigned short f; } P;
extern P *PLAYER_EXT;
typedef struct { int a[90]; } AnimTable;
extern AnimTable D_800E8404; /* shared with playerStepHazardRespawn (one table in the retail .rodata) */
void ObjSetAnimFromTable(O *o)
{
    AnimTable t = D_800E8404;
    o->anim = t.a[PLAYER_EXT->f];
}
#endif
