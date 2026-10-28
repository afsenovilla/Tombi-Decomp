// FUNC 80071748 20 MAIN0
// MATCHING 80071748 20
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_vm_don.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libsnd, member vm_don.o: _SsVmDamperOn. The first split had it
   in psyq_misc_q.c.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libspu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"
extern u16 D_8009D594;

extern u16 D_8009D594;

/* libsnd _SsVmDamperOn (VM_DON.OBJ): damper pedal on: _svm_damper = 2 (see
   _SsVmDamperOff above). */
/* func_8009A248 */
void _SsVmDamperOn(void) {
    D_8009D594 = 2;
}
