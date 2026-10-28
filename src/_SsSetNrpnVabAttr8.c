// FUNC 8006daf4 180 MAIN0
// MATCHING 8006daf4 180
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_de_8.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libsnd, member de_8.o: _SsSetNrpnVabAttr8. The first split had
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

/* libsnd _SsSetNrpnVabAttr8(vabId, prog, tone, vagatr, fn_idx, data) (de_8.o): the NRPN
   data-entry handler for tone attribute 8, the sustain rate with a linear sustain.  The
   by-value VagAtr is scratch: SsUtGetVagAtr reads the tone's attributes
   into it, _SsUtResolveADSR splits its adsr1 / adsr2 (+0x10 / +0x12) into
   nine shorts (0 attack rate, 1 decay rate, 2 sustain level, 3 sustain rate, 4 release
   rate, 5 attack mode, 6 sustain mode, 7 release mode, 8 sustain direction), the sustain
   mode is cleared and the sustain rate set to data, _SsUtBuildADSR packs
   the nine back into adsr1 / adsr2 and SsUtSetVagAtr writes the tone
   back.  fn_idx is not used. */
/* func_80096FB8 */
void _SsSetNrpnVabAttr8(s16 vabId, s16 prog, s16 tone, VagAtr vagatr, s16 fn_idx, u8 data)
{
    SsAdsr adsr;

    SsUtGetVagAtr(vabId, prog, tone, &vagatr);
    _SsUtResolveADSR(vagatr.adsr1, vagatr.adsr2, &adsr);
    adsr.s_mode = 0;
    adsr.sr = data;
    _SsUtBuildADSR(&adsr, &vagatr.adsr1, &vagatr.adsr2);
    SsUtSetVagAtr(vabId, prog, tone, &vagatr);
}
