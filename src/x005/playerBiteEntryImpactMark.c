// FUNC 80102a5c 100 X005
// MATCHING 80102a5c 100
typedef struct O { char p0[0x12]; short x; char p1[2]; short y; char p2[2]; short z; char p3[4]; short t; } O;
extern char *BITTEN_OBJECT;
extern void FUN_800eeb5c(O *, int);
extern void spawnImpactEffect(int, int, int, int);
void playerBiteEntryImpactMark(O *o)
{
    o->t = 0;
    FUN_800eeb5c(o, 0xd);
    spawnImpactEffect(2, o->x, o->y, o->z);
    if (BITTEN_OBJECT[2] == 2)
        BITTEN_OBJECT[6] = 2;
}
