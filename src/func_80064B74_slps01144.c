// near-twin of PAL CdMix (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 80064B74 40
#if defined(VERSION_SLPS_01144)
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"
extern int D_8009DDB0;
int CD_vol(CdlATV *vol);

/* older libcd: CdMix returns the complement of the CD_debug/flag word (PAL returns 1) */
int func_80064B74(CdlATV *vol) {
    CD_vol(vol);
    return ~D_8009DDB0;
}
#endif
