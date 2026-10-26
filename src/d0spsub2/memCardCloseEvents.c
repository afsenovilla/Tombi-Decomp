// FUNC 800ed020 168 D0SPSUB2
// MATCHING 800ed020 168
extern int MEMCARD_HW_END_IO, MEMCARD_HW_END_ERROR, MEMCARD_HW_TIMEOUT, MEMCARD_HW_NEW_DEVICE;
extern int MEMCARD_SW_END_IO, MEMCARD_SW_END_ERROR, MEMCARD_SW_TIMEOUT, MEMCARD_SW_NEW_DEVICE;
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int CloseEvent(int ev);

void memCardCloseEvents(void)
{
    EnterCriticalSection();
    CloseEvent(MEMCARD_SW_END_IO);
    CloseEvent(MEMCARD_SW_END_ERROR);
    CloseEvent(MEMCARD_SW_TIMEOUT);
    CloseEvent(MEMCARD_SW_NEW_DEVICE);
    CloseEvent(MEMCARD_HW_END_IO);
    CloseEvent(MEMCARD_HW_END_ERROR);
    CloseEvent(MEMCARD_HW_TIMEOUT);
    CloseEvent(MEMCARD_HW_NEW_DEVICE);
    ExitCriticalSection();
}
