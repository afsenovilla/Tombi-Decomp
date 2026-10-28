// FUNC 8006ddbc 220 MAIN0
// MATCHING 8006ddbc 220
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_de_12.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libsnd, member de_12.o: _SsSetNrpnVabAttr12. The first split
   had it in psyq_math.c.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libspu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"
/* the nine shorts a tone's two ADSR words are split into (ccadsr.o) */
typedef struct { u16 ar, dr, sl, sr, rr; s16 a_mode, s_mode, r_mode, s_dir; } SsAdsr;

/* The nine shorts a tone's two ADSR words are split into (ccadsr.o's record:
   _SsUtResolveADSR fills it, _SsUtBuildADSR packs it back).  _SsUtBuildADSR reads the
   five rates unsigned and the four mode flags signed. */

s16 SsUtGetVagAtr(s16 vabId, s16 progNum, s16 toneNum, VagAtr *vagatrptr);
s16 SsUtSetVagAtr(s16 vabId, s16 progNum, s16 toneNum, VagAtr *vagatrptr);
void _SsUtBuildADSR(SsAdsr *adsr, u16 *adsr1, u16 *adsr2);

/* libsnd _SsSetNrpnVabAttr12(vabId, prog, tone, vagatr, fn_idx, data) (de_12.o): the NRPN
   data-entry handler for tone attribute 12, the sustain direction.  The by-value VagAtr
   is scratch: SsUtGetVagAtr reads the tone's attributes into it; data 1
   to 0x3F clears short 8 of the nine ADSR shorts (the sustain direction, bit 0x4000 of
   adsr2), data 0x40 to 0x7F sets it to 1, any other data leaves it alone; _SsUtBuildADSR
    packs the nine into adsr1 / adsr2 (+0x10 / +0x12) and SsUtSetVagAtr
    writes the tone back.  Unlike attributes 4 to 10 this object has no
   _SsUtResolveADSR call, so the other eight shorts (all nine for data 0 or above 0x7F)
   are whatever the stack held (as in _SsSetNrpnVabAttr13 / 14).  fn_idx is not used. */
/* func_80097288 */
void _SsSetNrpnVabAttr12(s16 vabId, s16 prog, s16 tone, VagAtr vagatr, s16 fn_idx, u8 data)
{
    SsAdsr adsr;

    SsUtGetVagAtr(vabId, prog, tone, &vagatr);
    if (data > 0 && data < 0x40) {
        adsr.s_dir = 0;
    } else if (data >= 0x40 && data < 0x80) {
        adsr.s_dir = 1;
    }
    _SsUtBuildADSR(&adsr, &vagatr.adsr1, &vagatr.adsr2);
    SsUtSetVagAtr(vabId, prog, tone, &vagatr);
}
