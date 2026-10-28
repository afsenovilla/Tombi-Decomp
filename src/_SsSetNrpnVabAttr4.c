// FUNC 8006d740 180 MAIN0
// MATCHING 8006d740 180
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_de_4.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libsnd, member de_4.o: _SsSetNrpnVabAttr4. The first split had
   it in psyq_math.c.
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
void _SsUtResolveADSR(u16 adsr1, u16 adsr2, SsAdsr *adsr);      /* _SsUtResolveADSR; TU defines this below */
void _SsUtBuildADSR(SsAdsr *adsr, u16 *adsr1, u16 *adsr2);

/* libsnd _SsSetNrpnVabAttr4(vabId, prog, tone, vagatr, fn_idx, data) (de_4.o): the NRPN
   data-entry handler for tone attribute 4, the attack rate with a linear attack.  The
   by-value VagAtr is scratch: SsUtGetVagAtr reads the tone's attributes
   into it, _SsUtResolveADSR splits its adsr1 / adsr2 (+0x10 / +0x12) into
   nine shorts (0 attack rate, 1 decay rate, 2 sustain level, 3 sustain rate, 4 release
   rate, 5 attack mode, 6 sustain mode, 7 release mode, 8 sustain direction), the attack
   mode is cleared and the attack rate set to data, _SsUtBuildADSR packs
   the nine back into adsr1 / adsr2 and SsUtSetVagAtr writes the tone
   back.  fn_idx is not used. */
/* func_80096BE8 */
void _SsSetNrpnVabAttr4(s16 vabId, s16 prog, s16 tone, VagAtr vagatr, s16 fn_idx, u8 data)
{
    SsAdsr adsr;

    SsUtGetVagAtr(vabId, prog, tone, &vagatr);
    _SsUtResolveADSR(vagatr.adsr1, vagatr.adsr2, &adsr);
    adsr.a_mode = 0;
    adsr.ar = data;
    _SsUtBuildADSR(&adsr, &vagatr.adsr1, &vagatr.adsr2);
    SsUtSetVagAtr(vabId, prog, tone, &vagatr);
}
