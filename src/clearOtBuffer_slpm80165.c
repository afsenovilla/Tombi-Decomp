// VERSION SLPM_80165 slpm80165 80016DDC 44
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80017D04 (clearPlayerObject.c): memset size 0x178 -> 0x6c.
extern char PLAYER[];
extern void memset();

void clearOtBuffer(void)
{
    memset(PLAYER, 0, 0x6c);
}
#endif
