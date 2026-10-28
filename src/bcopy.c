// FUNC 8005bd24 52 MAIN0
// MATCHING 8005bd24 52
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libc2_bcopy.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libc2, member bcopy.o: bcopy. The first split had it in
   psyq_gpu_b.c.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libspu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"

/* libc2 bcopy(src, dst, n) (BCOPY.OBJ): copy n bytes from src to dst, low address
   first, and return src. A null src copies nothing and returns 0. */
/* func_8008B6E8 */
u8 *bcopy(u8 *src, u8 *dst, s32 n)
{
    u8 *ret;

    if (src == 0) {
        return 0;
    }
    ret = src;
    while (n > 0) {
        *dst++ = *src++;
        n--;
    }
    return ret;
}
