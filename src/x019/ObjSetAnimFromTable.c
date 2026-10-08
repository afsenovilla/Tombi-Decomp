// FUNC 800efc04 136 X019
// MATCHING 800efc04 136
typedef struct O { char p[0x24]; int anim; } O;
typedef struct P { char p[0x2c]; unsigned short f; } P;
extern P *g330;
typedef struct { int a[90]; } AnimTable;
extern AnimTable D_800E8404; /* shared with FUN_800fe13c (one table in the retail .rodata) */
void ObjSetAnimFromTable(O *o)
{
    AnimTable t = D_800E8404;
    o->anim = t.a[g330->f];
}
