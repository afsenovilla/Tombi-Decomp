// FUNC 8005c020 120 MAIN0
// MATCHING 8005c020 120
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libc2_strncpy.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libc2, member strncpy.o: strncpy. The first split had it in
   psyq_misc_aaj.c.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libspu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"
s32 _get_card_event_x(void);
void bzero(void *, s32);
void _clr_card_event(void);
inline s32 card_frame_read(s32 chan, s32 blk, u8 *buf);
inline s32 card_frame_write(s32 chan, s32 blk, u8 *buf);
char *strncpy(char *, char *, s32);

/* libc2 strncpy (STRNCPY.OBJ): copy at most n characters of src to dst. Once the
   terminating NUL has been copied the rest of the n characters are zero filled; when
   src is n characters or longer dst is left unterminated. Returns dst, or 0 without
   touching anything when either pointer is null. n <= 0 copies nothing and returns
   dst. Caller in the image: _card_create2 (libcard _card_create2), which copies the
   file name into a directory frame with n = 20. */
/* func_800A4C78 */
char *strncpy(char *dst, char *src, s32 n)
{
    char *ret;
    s32 i;

    if (dst == 0 || src == 0) {
        return 0;
    }
    ret = dst;
    for (i = 0; i < n; i++) {
        if ((*dst++ = *src++) == 0) {
            while (++i < n) {
                *dst++ = 0;
            }
            return ret;
        }
    }
    return ret;
}
