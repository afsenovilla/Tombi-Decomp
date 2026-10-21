// FUNC 800ece68 132 D0SPSUB2
// MATCHING 800ece68 132
extern int D_8009F10C, D_8009F110, D_8009F114, D_8009F118;
extern int TestEvent(int ev);

int memCardWaitHwEvent(void)
{
    while (1) {
        if (TestEvent(D_8009F10C) == 1) return 0;
        if (TestEvent(D_8009F110) == 1) return 1;
        if (TestEvent(D_8009F114) == 1) return 2;
        if (TestEvent(D_8009F118) == 1) return 3;
    }
}
