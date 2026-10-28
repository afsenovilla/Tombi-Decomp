// FUNC 80067558 60 MAIN0
// MATCHING 80067558 60
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libcd_c_008.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* SPDX-License-Identifier: MIT
   Parts of this file match code of sotn-decomp (src/main/psxsdk/libcd/c_008.c)
   and psyz (decomp/src/libcd/c_008.c), all MIT-licensed: the notices are in
   LICENSES/, and NOTICE.md says more. */
/* PSY-Q 4.5 libcd, member c_008.o: init_ring_status. The first split had it
   in psyq_spu_c.c.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libspu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"
extern s32 D_800A3F04;
extern void (*D_800E9570)(), (*D_800E9574)();

/* libcd init_ring_status(start, count) (C_008.OBJ; the name is sotn-decomp's c_008.c):
   mark count ring headers from index start free: the first word (id and type) of each
   32-byte header of the ring StRingAddr (D_800A3F04) = 0. StClearRing's worker.
   MARKED SPELLING: the header is addressed `StRingAddr + ((i + start) << 5)` on the
   integer ring base, as in StFreeRing and StGetNext of this TU; sotn-decomp's
   `*(s32 *)&ring[i + start]` on a StHEADER pointer scores 10/13 (operand order of the
   address sum). The original construct is not known. */
/* func_80093C08 */
void init_ring_status(s32 start, u32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        *(s32 *)(D_800A3F04 + ((i + start) << 5)) = 0;
    }
}
