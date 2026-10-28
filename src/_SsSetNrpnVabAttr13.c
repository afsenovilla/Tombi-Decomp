// FUNC 8006de98 160 MAIN0
// MATCHING 8006de98 160
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_de_13.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libsnd, member de_13.o: _SsSetNrpnVabAttr13. The first split
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

/* libsnd _SsSetNrpnVabAttr13(vabId, prog, tone, vagatr, fn_idx, data) (de_13.o): the NRPN
   data-entry handler for tone attribute 13, the vibrato time.  The by-value VagAtr is
   scratch: SsUtGetVagAtr reads the tone's attributes into it, vibT (+0x09)
   = data, _SsUtBuildADSR packs the nine ADSR shorts into adsr1 / adsr2
   (+0x10 / +0x12) and SsUtSetVagAtr writes the tone back.  The nine
   shorts are never filled in here: this object has no _SsUtResolveADSR call, so the
   tone's ADSR words are rebuilt from whatever the stack held.  fn_idx is not used. */
/* func_80097368 */
void _SsSetNrpnVabAttr13(s16 vabId, s16 prog, s16 tone, VagAtr vagatr, s16 fn_idx, u8 data)
{
    SsAdsr adsr;

    SsUtGetVagAtr(vabId, prog, tone, &vagatr);
    vagatr.vibT = data;
    _SsUtBuildADSR(&adsr, &vagatr.adsr1, &vagatr.adsr2);
    SsUtSetVagAtr(vabId, prog, tone, &vagatr);
}
