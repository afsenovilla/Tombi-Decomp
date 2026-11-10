// FUNC 80017d04 44 MAIN0
// MATCHING 80017d04 44
// VERSION SLPS_01144 slps01144 800172D8 44
// VERSION SLPM_80165 slpm80165 8001698C 44
// VERSION PCPX_96084 tomba 80019A30 44
// VERSION BETA_980203 beta980203 800178FC 44
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL main0 80017D04 (clearPlayerObject.c): 376 -> 360.
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x168);
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80017D04 (clearPlayerObject.c): PLAYER is 0x164 bytes (PAL 0x178).
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x164);
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 80017D04 (clearPlayerObject.c): PLAYER is 0x16c bytes (PAL 0x178).
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x16c);
}
#elif defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80017D04 (clearPlayerObject.c): PAL C of src/clearPlayerObject.c (0.909 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x14c);
}
#else
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x178);
}
#endif
