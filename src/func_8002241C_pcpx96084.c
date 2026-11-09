// VERSION PCPX_96084 tomba 8002241C 132
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL d0spsub2 800ECEEC (memCardWaitSwEvent.c): PAL C of src/d0spsub2/memCardWaitSwEvent.c (memCardWaitSwEvent, twin score 0.703) [-O2 -G0] (tools/demo_sweep.py)
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
#endif
