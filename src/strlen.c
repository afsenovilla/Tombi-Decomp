// FUNC 8005bf7c 48 MAIN0
// MATCHING 8005bf7c 48
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libc2_strlen.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libc2, member strlen.o: strlen. The first split had it in
   psyq_gpu_b.c.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libspu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"

/* libc2 strlen(s) (STRLEN.OBJ): the number of bytes before the terminating 0 of
   the string s; 0 for a null s. */
/* func_8008B848 */
int strlen(u8 *s) /* port: s8 in eternal-silver, PAL has lbu */
{
    int n = 0;

    if (s == 0) {
        return 0;
    }
    while (*s++) {
        n++;
    }
    return n;
}
