// FUNC 8006dd14 168 MAIN0
// MATCHING 8006dd14 168
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_de_11.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libsnd, member de_11.o: _SsSetNrpnVabAttr11. The first split
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

/* libsnd _SsSetNrpnVabAttr11(vabId, prog, tone, vagatr, fn_idx, data) (de_11.o): the NRPN
   data-entry handler for tone attribute 11, the release rate with an exponential release.
   The by-value VagAtr is scratch: SsUtGetVagAtr reads the tone's
   attributes into it, the release mode (short 7 of the nine ADSR shorts) is set to 1 and
   the release rate (short 4) to data, _SsUtBuildADSR packs the nine into
   adsr1 / adsr2 (+0x10 / +0x12) and SsUtSetVagAtr writes the tone back.
   Unlike attributes 4 to 10 this object has no _SsUtResolveADSR call, so the other seven
   shorts are whatever the stack held (as in _SsSetNrpnVabAttr13 / 14).  fn_idx is not
   used. */
/* func_800971D8 */
void _SsSetNrpnVabAttr11(s16 vabId, s16 prog, s16 tone, VagAtr vagatr, s16 fn_idx, u8 data)
{
    SsAdsr adsr;

    SsUtGetVagAtr(vabId, prog, tone, &vagatr);
    adsr.r_mode = 1;
    adsr.rr = data;
    _SsUtBuildADSR(&adsr, &vagatr.adsr1, &vagatr.adsr2);
    SsUtSetVagAtr(vabId, prog, tone, &vagatr);
}
