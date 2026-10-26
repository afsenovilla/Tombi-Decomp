// FUNC 800ecf70 88 D0SPSUB2
// MATCHING 800ecf70 88
extern int MEMCARD_SW_END_IO, MEMCARD_SW_END_ERROR, MEMCARD_SW_TIMEOUT, MEMCARD_SW_NEW_DEVICE;
extern int TestEvent(int ev);

void memCardClearHwEvents(void)
{
    TestEvent(MEMCARD_SW_END_IO);
    TestEvent(MEMCARD_SW_END_ERROR);
    TestEvent(MEMCARD_SW_TIMEOUT);
    TestEvent(MEMCARD_SW_NEW_DEVICE);
}
