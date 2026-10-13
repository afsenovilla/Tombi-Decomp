// FUNC 8002078c 96 MAIN0
// MATCHING 8002078c 96
typedef struct { short x, y; } P;
extern int angleFromDelta(int, int);

int angleBetweenPoints(P a, P b)
{
    if (a.x == b.x && a.y == b.y)
        return 0;
    return angleFromDelta(b.x - a.x, b.y - a.y);
}
