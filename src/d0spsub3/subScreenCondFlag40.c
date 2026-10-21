// FUNC 800ee9c8 24 D0SPSUB3
// MATCHING 800ee9c8 24
extern int D_8009C984;

int subScreenCondFlag40(void)
{
    if (D_8009C984 & 0x40) return 1;
    return 0;
}
