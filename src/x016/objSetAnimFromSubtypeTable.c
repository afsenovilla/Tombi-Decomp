// FUNC 80113754 72 X016
// MATCHING 80113754 72
typedef struct O { char p0[3]; unsigned char st; char p1[0x20]; int *anim; char p2[0x74-0x28]; short a; short b; } O;
extern int *SCRIPT_ACTOR_DEFS_ANIMS[][4];
extern void FUN_8001fe94(O*,short); void objSetAnimFromSubtypeTable(O *o){ o->anim = (int*)((int**)SCRIPT_ACTOR_DEFS_ANIMS)[o->st * 4][o->a]; FUN_8001fe94(o, o->b); }
