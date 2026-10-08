// FUNC 800695a8 104 MAIN0
// MATCHING 800695a8 104
extern int *D_8009820C;
extern void (*D_800981D4)(void);
int func_800695A8(void)
{
    if (!(D_8009820C[1] & 1)) return 0;
    if (!(D_8009820C[0] & 1)) return 0;
    if (D_800981D4 != 0) D_800981D4();
    return 1;
}
