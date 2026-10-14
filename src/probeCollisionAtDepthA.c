// FUNC 800408d8 116 MAIN0
// MATCHING 800408d8 116
typedef struct O { char p0[0x44]; short *q; } O;
extern int getCollisionPlaneAt(int, int);
extern int FUN_800406e8(O *, int, int);
short probeCollisionAtDepthA(O *o, short a, int b)
{
    *(int *)0x1f800278 = getCollisionPlaneAt(a, o->q[1]);
    return FUN_800406e8(o, a, (short)b);
}
