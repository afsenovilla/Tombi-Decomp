// FUNC 800411cc 116 MAIN0
// MATCHING 800411cc 116
extern int getCollisionPlaneAt(int, int);
extern short testCollisionSegmentList(int, int, int);
extern int COLLISION_SEG_PTR;
int probeCollisionAtDepthB(int o, short a, int b)
{
    COLLISION_SEG_PTR = getCollisionPlaneAt(a, *(short *)(*(int *)(o + 0x44) + 2));
    return (short)testCollisionSegmentList(o, a, (short)b);
}
