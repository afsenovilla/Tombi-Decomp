// FUNC 80019084 448 MAIN0
// MATCHING 80019084 448
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern long OpenEvent(unsigned long, long, long, long (*)());
extern long EnableEvent(unsigned long);
extern void InitCARD(long);
extern long StartCARD(void);
extern void _bu_init(void);
extern long MEMCARD_SW_END_IO, MEMCARD_SW_END_ERROR, MEMCARD_SW_TIMEOUT, MEMCARD_SW_NEW_DEVICE;
extern long MEMCARD_HW_END_IO, MEMCARD_HW_END_ERROR, MEMCARD_HW_TIMEOUT, MEMCARD_HW_NEW_DEVICE;

void MemCardInit(void)
{
    EnterCriticalSection();
    MEMCARD_SW_END_IO = OpenEvent(0xf4000001, 4, 0x2000, 0);
    MEMCARD_SW_END_ERROR = OpenEvent(0xf4000001, 0x8000, 0x2000, 0);
    MEMCARD_SW_TIMEOUT = OpenEvent(0xf4000001, 0x100, 0x2000, 0);
    MEMCARD_SW_NEW_DEVICE = OpenEvent(0xf4000001, 0x2000, 0x2000, 0);
    MEMCARD_HW_END_IO = OpenEvent(0xf0000011, 4, 0x2000, 0);
    MEMCARD_HW_END_ERROR = OpenEvent(0xf0000011, 0x8000, 0x2000, 0);
    MEMCARD_HW_TIMEOUT = OpenEvent(0xf0000011, 0x100, 0x2000, 0);
    MEMCARD_HW_NEW_DEVICE = OpenEvent(0xf0000011, 0x2000, 0x2000, 0);
    InitCARD(0);
    ExitCriticalSection();
    StartCARD();
    _bu_init();
    EnableEvent(MEMCARD_SW_END_IO);
    EnableEvent(MEMCARD_SW_END_ERROR);
    EnableEvent(MEMCARD_SW_TIMEOUT);
    EnableEvent(MEMCARD_SW_NEW_DEVICE);
    EnableEvent(MEMCARD_HW_END_IO);
    EnableEvent(MEMCARD_HW_END_ERROR);
    EnableEvent(MEMCARD_HW_TIMEOUT);
    EnableEvent(MEMCARD_HW_NEW_DEVICE);
}
