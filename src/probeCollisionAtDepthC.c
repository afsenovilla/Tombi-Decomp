// FUNC 80041ca8 120 MAIN0
// MATCHING 80041ca8 120
typedef struct O { char p[0x44]; short *q; } O;
extern int getCollisionPlaneAt(int, int);
extern int FUN_800416b8(O *, int, int, int);
extern int COLLISION_SEG_PTR;
short probeCollisionAtDepthC(O *o, short b, int c)
{
short *q = o->q; COLLISION_SEG_PTR = getCollisionPlaneAt(b, q[1]); return FUN_800416b8(o, b, (short)c, -1);
}
