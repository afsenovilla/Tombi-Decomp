// FUNC 800ece68 132 D0SPSUB2
// MATCHING 800ece68 132
extern int MEMCARD_SW_END_IO, MEMCARD_SW_END_ERROR, MEMCARD_SW_TIMEOUT, MEMCARD_SW_NEW_DEVICE;
extern int TestEvent(int ev);

int memCardWaitHwEvent(void)
{
    while (1) {
        if (TestEvent(MEMCARD_SW_END_IO) == 1) return 0;
        if (TestEvent(MEMCARD_SW_END_ERROR) == 1) return 1;
        if (TestEvent(MEMCARD_SW_TIMEOUT) == 1) return 2;
        if (TestEvent(MEMCARD_SW_NEW_DEVICE) == 1) return 3;
    }
}
