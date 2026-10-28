// FUNC 8006dfd8 36 MAIN0
// MATCHING 8006dfd8 36
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_de_15.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libsnd, member de_15.o: _SsSetNrpnVabAttr15. The first split
   had it in psyq_math_b.c.
   Built with -fno-delayed-branch @delay-fill (config/cc_flags.txt): this object puts the stack adjust BEFORE `jr $ra`.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"

/* libsnd _SsSetNrpnVabAttr15(vabId, prog, tone, vagatr, fn_idx, data) (de_15.o): the NRPN
   data-entry handler for attribute 15 (DE_REV_TYPE), the reverb type: SsUtSetReverbType
   (data).  The signature is the tone-attribute family's
   (libsnd.h); the by-value VagAtr and the other parameters are not used, and the one
   trace the VagAtr leaves is `sw $a3` homing its first word.  The 12 bytes of padding
   after the code belong to this object, not to the function, so they are emitted
   explicitly. */
/* func_800974A8 */
void _SsSetNrpnVabAttr15(s16 vabId, s16 prog, s16 tone, VagAtr vagatr, s16 fn_idx, u8 data)
{
    SsUtSetReverbType(data);
}
