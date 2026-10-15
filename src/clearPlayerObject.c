// FUNC 80017d04 44 MAIN0
// MATCHING 80017d04 44
// VERSION SLPS_01144 slps01144 800172D8 44
// VERSION SLPM_80165 slpm80165 8001698C 44
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
#else
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x178);
}
#endif
