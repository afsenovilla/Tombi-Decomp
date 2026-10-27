// FUNC 80018980 56 MAIN0
// MATCHING 80018980 56
extern int **DAT_1f800210;
extern unsigned short FREE_OBJECT_COUNT_UNLAYERED;

void PoolFree_1F800210(int *p)
{
    p[0] = 0;
    p[1] = 0;
    FREE_OBJECT_COUNT_UNLAYERED++;
    *--DAT_1f800210 = p;
}
