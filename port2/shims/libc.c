/* (Windows: the shims below are the CRT names themselves, not mingw's inline C99 wrappers) */
#undef __USE_MINGW_ANSI_STDIO
#define __USE_MINGW_ANSI_STDIO 0
/* Psy-Q libc (libc2) replacements on top of the host libc: only what differs from glibc or must stay quiet.
 * rand() is the Psy-Q LCG (the game's behaviour depends on its sequence); printf/puts/putchar of the game print
 * only with -v (they override glibc's for this executable; the runtime logs through fprintf(stderr)). */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "port_runtime.h"
/* (not <stdlib.h>: this file defines rand / srand itself; clang on Android rejects the implicit declaration) */
extern char *getenv(const char *);

typedef unsigned int size_t_;
/* names the game's sources use for the same retail routines */
void *mcpy(void *d, const void *s, size_t_ n) { return memcpy(d, s, n); }
void *memset_(void *d, int c, size_t_ n) { return memset(d, c, n); }
void *memfill(void *d, int c, size_t_ n) { return memset(d, c, n); }

static unsigned long rand_next = 0x24040001;   /* Psy-Q rand(): same LCG */
int rand(void) {
    rand_next = rand_next * 1103515245 + 12345;
    return (rand_next >> 16) & 0x7FFF;
}
void srand(unsigned int s) { rand_next = s; }

int printf(const char *f, ...) {
    char out_buf[1024];
    va_list ap;
    va_start(ap, f);
    int len = vsnprintf(out_buf, sizeof out_buf, f, ap);
    va_end(ap);
    if (port_verbose) fprintf(stderr, "[game] %s", out_buf);
    return len;
}
int puts(const char *s) {
    if (port_verbose) fprintf(stderr, "[game] %s\n", s);
    return 0;
}
int putchar(int c) {
    if (port_verbose) fputc(c, stderr);
    return c;
}

/* 0x8005BD24: Psy-Q's bcopy, a forward byte loop; lzDecompress relies on the overlap (dst - off -> dst, n bytes)
   repeating the pattern, which memmove would not do.  The loop reads through a volatile pointer and this file never
   calls it by the name "bcopy": the compilers know bcopy as memmove (GCC and clang turned the bcopy() call that was
   here into a jump to memmove, which left the old buffer bytes after the first `off` of an overlapping copy: the
   specks of the Million-Year-Old Man's room, docs/PORT.md "LZ overlap"), and may recognise a plain byte loop as
   memcpy/memmove.  PORT_LZ_MEMMOVE=1 (research) brings the old behaviour back, to compare frames. */
static void psx_bcopy(const void *s, void *d, unsigned int n) {
    const volatile unsigned char *src = s;
    unsigned char *dst = d;
    while (n--) *dst++ = *src++;
}
void bcopy(const void *s, void *d, unsigned int n) { psx_bcopy(s, d, n); }
void bzero(void *d, unsigned int n) { memset(d, 0, n); }      /* (not in the Windows CRT) */
void FUN_8005bd24(const void *s, void *d, unsigned int n)
{
    static int use_memmove = -1;
    if (use_memmove < 0) {
        const char *env = getenv("PORT_LZ_MEMMOVE");
        use_memmove = env && *env == '1';
    }
    if (use_memmove) memmove(d, s, n);
    else psx_bcopy(s, d, n);
}
