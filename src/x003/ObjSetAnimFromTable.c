// FUNC 800efc04 136 X003
// MATCHING 800efc04 136
typedef struct O { char p[0x24]; int anim; } O;
typedef struct P { char p[0x2c]; unsigned short f; } P;
extern P *g330;
typedef struct { int a[90]; } AnimTable;
extern AnimTable PLAYER_ANIM_TABLE; /* shared with playerStepHazardRespawn (one table in the retail .rodata) */
void ObjSetAnimFromTable(O *o)
{
    AnimTable t = PLAYER_ANIM_TABLE;
    o->anim = t.a[g330->f];
}
