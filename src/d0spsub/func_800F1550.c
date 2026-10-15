// FUNC 800f1550 24 D0SPSUB
// MATCHING 800f1550 24
extern int D_8009C984;

int func_800F1550(void)
{
    if (D_8009C984 & 0x100) return 1;
    return 0;
}
