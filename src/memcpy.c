// FUNC 8005bd88 52 MAIN0
// MATCHING 8005bd88 52
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libc2_memcpy.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libc2, member memcpy.o: memcpy. The first split had it in
   psyq_gpu_g.c.
   Built with -fno-delayed-branch @delay-fill (config/cc_flags.txt): this object puts the stack adjust BEFORE `jr $ra`.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"
     /* cc97_lib: memcpy */

/* libc2 memcpy(dst, src, n) (MEMCPY.OBJ): copies n bytes from src to dst, lowest
   address first, and returns dst. A null dst copies nothing and returns 0; n <= 0
   copies nothing and returns dst. The parameter types are memory.h's (its
   prototype carries them in a comment). */
/* func_8008E7C8 */
u8 *memcpy(u8 *dst, u8 *src, s32 n) /* port: void *(u_char*,u_char*,int) in eternal-silver; typed as common.h declares it */
{
    u_char *ret;

    if (dst == 0) {
        return 0;
    }
    ret = dst;
    while (n > 0) {
        *dst++ = *src++;
        n--;
    }
    return ret;
}
