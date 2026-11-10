// VERSION SLPM_80165 slpm80165 8003D21C 156
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of probeWallAt (src/probeWallAt.c): returns 0 for a negative position.
typedef struct {
    char pad[0x44];
    short *p44;
} O8004065C;

extern int getCollisionPlaneAt(int, int);
extern short pushOutOfWallSegments(O8004065C *, int, int, short);

short probeWallAt(O8004065C *o, short a, short b, int c)
{
    if (a < 0)
        return 0;
    *(int *)0x1F800278 = getCollisionPlaneAt(a, o->p44[1]);
    return pushOutOfWallSegments(o, a, b, (c & 1) | 0xff00);
}
#endif
