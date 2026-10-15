// FUNC 800411cc 116 MAIN0
// MATCHING 800411cc 116
extern int getCollisionPlaneAt(int, int);
extern short testCollisionSegmentList(int, int, int);
extern int DAT_1f800278;
int probeCollisionAtDepthB(int o, short a, int b)
{
    DAT_1f800278 = getCollisionPlaneAt(a, *(short *)(*(int *)(o + 0x44) + 2));
    return (short)testCollisionSegmentList(o, a, (short)b);
}
