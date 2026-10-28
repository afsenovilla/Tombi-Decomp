// FUNC 8005bf38 68 MAIN0
// MATCHING 8005bf38 68
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libc2_strcpy.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libc2, member strcpy.o: strcpy. The first split had it in
   psyq_gpu_b.c.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libspu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"

/* libc2 strcpy(dst, src) (STRCPY.OBJ): copy the string src, terminator included,
   to dst and return dst. A null dst or a null src copies nothing and returns 0. */
/* func_8008B7F8 */
char *strcpy(char *dst, char *src)
{
    char *ret;

    if (dst == 0 || src == 0) {
        return 0;
    }
    ret = dst;
    while ((*dst++ = *src++) != 0) {
    }
    return ret;
}
