// JPDEMO x00 800EDA04 120
// Demo version of PAL x000 800EFC04 (ObjSetAnimFromTable.c):  56 player animations (PAL 90).
typedef struct O { char p[0x24]; int anim; } O;
typedef struct P { char p[0x2c]; unsigned short f; } P;
extern P *g330;
typedef struct { int a[56]; } AnimTable;
extern AnimTable D_800E8404; /* shared with FUN_800fe13c (one table in the retail .rodata) */
void ObjSetAnimFromTable(O *o)
{
    AnimTable t = D_800E8404;
    o->anim = t.a[g330->f];
}
