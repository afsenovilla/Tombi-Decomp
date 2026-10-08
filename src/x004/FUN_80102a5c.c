// FUNC 80102a5c 100 X004
// MATCHING 80102a5c 100
typedef struct O { char p0[0x12]; short x; char p1[2]; short y; char p2[2]; short z; char p3[4]; short t; } O;
extern char *DAT_8009d2e8;
extern void FUN_800eeb5c(O *, int);
extern void FUN_8001f96c(int, int, int, int);
void FUN_80102a5c(O *o)
{
    o->t = 0;
    FUN_800eeb5c(o, 0xd);
    FUN_8001f96c(2, o->x, o->y, o->z);
    if (DAT_8009d2e8[2] == 2)
        DAT_8009d2e8[6] = 2;
}
