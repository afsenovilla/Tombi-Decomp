// FUNC 800ecfc8 88 D0SPSUB2
// MATCHING 800ecfc8 88
extern int MEMCARD_HW_END_IO, MEMCARD_HW_END_ERROR, MEMCARD_HW_TIMEOUT, MEMCARD_HW_NEW_DEVICE;
extern int TestEvent(int ev);

void memCardClearSwEvents(void)
{
    TestEvent(MEMCARD_HW_END_IO);
    TestEvent(MEMCARD_HW_END_ERROR);
    TestEvent(MEMCARD_HW_TIMEOUT);
    TestEvent(MEMCARD_HW_NEW_DEVICE);
}
