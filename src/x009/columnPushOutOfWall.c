// FUNC 80121434 156 X009
// MATCHING 80121434 156
typedef struct { char pad[0x44]; short *p44; } O;
extern char *getCollisionColumnPtr(short, unsigned char);
extern short pushOutOfWallSegments(O *, int, int, short);

short columnPushOutOfWall(O *o, short a, short b, int c, unsigned char d)
{
    *(int *)0x1F800278 = (int)getCollisionColumnPtr(a, d);
    return pushOutOfWallSegments(o, a, b, (d << 8) | (c & 1));
}
