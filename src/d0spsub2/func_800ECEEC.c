// FUNC 800eceec 132 D0SPSUB2
// MATCHING 800eceec 132
extern int D_8009F0F4, D_8009F0F8, D_8009F0FC, D_8009F100;
extern int TestEvent(int ev);

int func_800ECEEC(void)
{
    while (1) {
        if (TestEvent(D_8009F0F4) == 1) return 0;
        if (TestEvent(D_8009F0F8) == 1) return 1;
        if (TestEvent(D_8009F0FC) == 1) return 2;
        if (TestEvent(D_8009F100) == 1) return 3;
    }
}
