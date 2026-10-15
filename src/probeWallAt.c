// FUNC 8004065c 140 MAIN0
// MATCHING 8004065c 140
typedef struct {
    char pad[0x44];
    short *p44;
} O8004065C;

extern int getCollisionPlaneAt(int, int);
extern short pushOutOfWallSegments(O8004065C *, int, int, short);

short probeWallAt(O8004065C *o, short a, short b, int c)
{
    *(int *)0x1F800278 = getCollisionPlaneAt(a, o->p44[1]);
    return pushOutOfWallSegments(o, a, b, (c & 1) | 0xff00);
}
