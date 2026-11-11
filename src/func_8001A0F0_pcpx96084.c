// VERSION PCPX_96084 tomba 8001A0F0 44
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80017D04 (clearPlayerObject.c): PAL C of src/clearPlayerObject.c (0.909 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x80);
}
#endif
