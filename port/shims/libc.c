/* Psy-Q libc (libc2) replacements.  Signatures follow the Psy-Q headers; cdecl tolerates the game's looser
 * declarations.  These also serve gcc's own implicit memcpy/memset calls. */
#include "port_runtime.h"

typedef unsigned int size_t_;

void *memcpy(void *d, const void *s, size_t_ n) {
    unsigned char *a = d; const unsigned char *b = s;
    while (n--) *a++ = *b++;
    return d;
}
void *memmove(void *d, const void *s, size_t_ n) {
    unsigned char *a = d; const unsigned char *b = s;
    if (a < b) while (n--) *a++ = *b++;
    else { a += n; b += n; while (n--) *--a = *--b; }
    return d;
}
void *memset(void *d, int c, size_t_ n) { unsigned char *a = d; while (n--) *a++ = (unsigned char)c; return d; }
int memcmp(const void *x, const void *y, size_t_ n) {
    const unsigned char *a = x, *b = y;
    for (; n; n--, a++, b++) if (*a != *b) return *a - *b;
    return 0;
}
void *memchr(const void *s, int c, size_t_ n) {
    const unsigned char *a = s;
    for (; n; n--, a++) if (*a == (unsigned char)c) return (void *)a;
    return 0;
}
void bzero(void *d, int n) { memset(d, 0, n); }
void bcopy(const void *s, void *d, int n) { memmove(d, s, n); }
/* names the game's sources use for the same retail routines */
void *mcpy(void *d, const void *s, size_t_ n) { return memcpy(d, s, n); }
void *memset_(void *d, int c, size_t_ n) { return memset(d, c, n); }

size_t_ strlen(const char *s) { size_t_ n = 0; while (s[n]) n++; return n; }
char *strcpy(char *d, const char *s) { char *r = d; while ((*d++ = *s++)); return r; }
char *strncpy(char *d, const char *s, size_t_ n) {
    char *r = d;
    while (n && (*d = *s)) { d++; s++; n--; }
    while (n--) *d++ = 0;
    return r;
}
char *strcat(char *d, const char *s) { strcpy(d + strlen(d), s); return d; }
char *strncat(char *d, const char *s, size_t_ n) {
    char *e = d + strlen(d);
    while (n-- && *s) *e++ = *s++;
    *e = 0;
    return d;
}
int strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}
int strncmp(const char *a, const char *b, size_t_ n) {
    for (; n; n--, a++, b++) { if (*a != *b) return (unsigned char)*a - (unsigned char)*b; if (!*a) break; }
    return 0;
}
char *strstr(const char *h, const char *n) {
    size_t_ l = strlen(n);
    for (; *h; h++) if (!strncmp(h, n, l)) return (char *)h;
    return l ? 0 : (char *)h;
}
int toupper(int c) { return c >= 'a' && c <= 'z' ? c - 32 : c; }
int abs(int x) { return x < 0 ? -x : x; }

static unsigned long rand_next = 0x24040001;   /* Psy-Q rand(): same LCG */
int rand(void) { rand_next = rand_next * 1103515245 + 12345; return (rand_next >> 16) & 0x7FFF; }
void srand(unsigned int s) { rand_next = s; }

int vsprintf(char *buf, const char *f, __builtin_va_list ap) { return port_vsnprintf(buf, 0x7FFFFFFF, f, ap); }
int sprintf(char *buf, const char *f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    int r = port_vsnprintf(buf, 0x7FFFFFFF, f, ap); __builtin_va_end(ap);
    return r;
}
int printf(const char *f, ...) {
    char b[1024]; __builtin_va_list ap; __builtin_va_start(ap, f);
    int r = port_vsnprintf(b, sizeof b, f, ap); __builtin_va_end(ap);
    if (port_verbose) { port_puts("[game] "); port_puts(b); }
    return r;
}
int puts(const char *s) { if (port_verbose) { port_puts(s); port_puts("\n"); } return 0; }
int putchar(int c) { char b[2] = { (char)c, 0 }; if (port_verbose) port_puts(b); return c; }
