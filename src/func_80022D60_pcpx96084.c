// VERSION PCPX_96084 tomba 80022D60 180
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 startMovieStream (src/startMovieStream_scus94236.c): movie 1 is read without the real-time flag.
#include "common.h"
#include "psyq/libcd.h"
#include "psyq/libpress.h"
extern short D_800DE084;
extern unsigned char D_800A2200[];
extern unsigned long D_80124DC4[];
extern void func_80022BFC_pcpx96084(void);

void func_80022D60_pcpx96084(CdlLOC *loc)
{
    int mode;

    DecDCTReset(0);
    DecDCToutCallback(func_80022BFC_pcpx96084);
    CdMix((CdlATV *)D_800A2200);
    StSetRing(D_80124DC4, 0x20);
    StSetStream(0, 1, -1, 0, 0);
    do {
    } while (CdControl(CdlSetloc, (u_char *)loc, 0) == 0);
    if (D_800DE084 == 1) mode = CdlModeStream | CdlModeSpeed;
    else mode = CdlModeStream | CdlModeSpeed | CdlModeRT;
    do {
    } while (CdRead2(mode) == 0);
}
#endif
