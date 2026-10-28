// FUNC 8006d6b0 144 MAIN0
// MATCHING 8006d6b0 144
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_de_3.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libsnd, member de_3.o: _SsSetNrpnVabAttr3. The first split had
   it in psyq_math.c.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libspu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"

/* The nine shorts a tone's two ADSR words are split into (ccadsr.o's record:
   _SsUtResolveADSR fills it, _SsUtBuildADSR packs it back).  _SsUtBuildADSR reads the
   five rates unsigned and the four mode flags signed. */

s16 SsUtGetVagAtr(s16 vabId, s16 progNum, s16 toneNum, VagAtr *vagatrptr);
s16 SsUtSetVagAtr(s16 vabId, s16 progNum, s16 toneNum, VagAtr *vagatrptr);

/* libsnd _SsSetNrpnVabAttr3(vabId, prog, tone, vagatr, fn_idx, data) (de_3.o): the NRPN
   data-entry handler for tone attribute 3, the highest note of the tone's key range.  The
   by-value VagAtr is scratch: SsUtGetVagAtr reads the tone's attributes
   into it, max (+0x07) = data, SsUtSetVagAtr writes them back.  fn_idx
   is not used. */
/* func_80096B58 */
void _SsSetNrpnVabAttr3(s16 vabId, s16 prog, s16 tone, VagAtr vagatr, s16 fn_idx, u8 data)
{
    SsUtGetVagAtr(vabId, prog, tone, &vagatr);
    vagatr.max = data;
    SsUtSetVagAtr(vabId, prog, tone, &vagatr);
}
