// FUNC 80113754 72 X005
// MATCHING 80113754 72
typedef struct O { char p0[3]; unsigned char st; char p1[0x20]; int *anim; char p2[0x74-0x28]; short a; short b; } O;
extern int *PTR_80115df0[][4];
extern void FUN_8001fe94(O*,short); void FUN_80113754(O *o){ o->anim = (int*)((int**)PTR_80115df0)[o->st * 4][o->a]; FUN_8001fe94(o, o->b); }
