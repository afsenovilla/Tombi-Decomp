// FUNC 800eceec 132 D0SPSUB2
// MATCHING 800eceec 132
extern int MEMCARD_HW_END_IO, MEMCARD_HW_END_ERROR, MEMCARD_HW_TIMEOUT, MEMCARD_HW_NEW_DEVICE;
extern int TestEvent(int ev);

int memCardWaitSwEvent(void)
{
    while (1) {
        if (TestEvent(MEMCARD_HW_END_IO) == 1) return 0;
        if (TestEvent(MEMCARD_HW_END_ERROR) == 1) return 1;
        if (TestEvent(MEMCARD_HW_TIMEOUT) == 1) return 2;
        if (TestEvent(MEMCARD_HW_NEW_DEVICE) == 1) return 3;
    }
}
