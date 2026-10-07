// FUNC 80018980 56 MAIN0
// MATCHING 80018980 56
extern int **DAT_1f800210;
extern unsigned short DAT_1f80023e;

void PoolFree_1F800210(int *p)
{
    p[0] = 0;
    p[1] = 0;
    DAT_1f80023e++;
    *--DAT_1f800210 = p;
}
