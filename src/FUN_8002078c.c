// FUNC 8002078c 96 MAIN0
// MATCHING 8002078c 96
typedef struct { short x, y; } P;
extern int FUN_800205d8(int, int);

int FUN_8002078c(P a, P b)
{
    if (a.x == b.x && a.y == b.y)
        return 0;
    return FUN_800205d8(b.x - a.x, b.y - a.y);
}
