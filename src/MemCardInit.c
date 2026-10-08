// FUNC 80019084 448 MAIN0
// MATCHING 80019084 448
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern long OpenEvent(unsigned long, long, long, long (*)());
extern long EnableEvent(unsigned long);
extern void InitCARD(long);
extern long StartCARD(void);
extern void _bu_init(void);
extern long DAT_8009f10c, DAT_8009f110, DAT_8009f114, DAT_8009f118;
extern long DAT_8009f0f4, DAT_8009f0f8, DAT_8009f0fc, DAT_8009f100;

void MemCardInit(void)
{
    EnterCriticalSection();
    DAT_8009f10c = OpenEvent(0xf4000001, 4, 0x2000, 0);
    DAT_8009f110 = OpenEvent(0xf4000001, 0x8000, 0x2000, 0);
    DAT_8009f114 = OpenEvent(0xf4000001, 0x100, 0x2000, 0);
    DAT_8009f118 = OpenEvent(0xf4000001, 0x2000, 0x2000, 0);
    DAT_8009f0f4 = OpenEvent(0xf0000011, 4, 0x2000, 0);
    DAT_8009f0f8 = OpenEvent(0xf0000011, 0x8000, 0x2000, 0);
    DAT_8009f0fc = OpenEvent(0xf0000011, 0x100, 0x2000, 0);
    DAT_8009f100 = OpenEvent(0xf0000011, 0x2000, 0x2000, 0);
    InitCARD(0);
    ExitCriticalSection();
    StartCARD();
    _bu_init();
    EnableEvent(DAT_8009f10c);
    EnableEvent(DAT_8009f110);
    EnableEvent(DAT_8009f114);
    EnableEvent(DAT_8009f118);
    EnableEvent(DAT_8009f0f4);
    EnableEvent(DAT_8009f0f8);
    EnableEvent(DAT_8009f0fc);
    EnableEvent(DAT_8009f100);
}
