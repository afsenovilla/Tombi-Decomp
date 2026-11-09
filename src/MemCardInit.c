// FUNC 80019084 448 MAIN0
// MATCHING 80019084 448
// VERSION SLPM_80165 slpm80165 80017CAC 448
// VERSION SLPS_01144 slps01144 80018658 448
// VERSION PCPX_96084 tomba 80021E74 448
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80019084 (MemCardInit.c): InitCARD(1) (PAL 0).
#include "globals.h"
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern long OpenEvent(unsigned long, long, long, long (*)());
extern long EnableEvent(unsigned long);
extern void InitCARD(long);
extern long StartCARD(void);
extern void _bu_init(void);

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
    InitCARD(1);
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
#elif defined(VERSION_SLPS_01144)
#include "globals.h"
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern long OpenEvent(unsigned long, long, long, long (*)());
extern long EnableEvent(unsigned long);
extern void InitCARD(long);
extern long StartCARD(void);
extern void _bu_init(void);

void func_80018658(void)
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
    InitCARD(1);
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
#elif defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80019084 (MemCardInit.c): SLPS_01144 C of src/MemCardInit.c (func_80018658, twin score 0.629) [-O2 -G0] (tools/demo_sweep.py)
#include "globals.h"
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern long OpenEvent(unsigned long, long, long, long (*)());
extern long EnableEvent(unsigned long);
extern void InitCARD(long);
extern long StartCARD(void);
extern void _bu_init(void);

void func_80018658(void)
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
    InitCARD(1);
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
#else
#include "globals.h"
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern long OpenEvent(unsigned long, long, long, long (*)());
extern long EnableEvent(unsigned long);
extern void InitCARD(long);
extern long StartCARD(void);
extern void _bu_init(void);

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
#endif
