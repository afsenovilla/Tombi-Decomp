/* Runtime code shared by the freestanding hosts (sys_linux_i386.c, sys_win32_i386.c): printf and the 64-bit
 * division helpers gcc calls (no libgcc in the freestanding build). */
#include "port_runtime.h"

static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }

/* ------------------------------------------------------------------------------------------- printf */
int port_vsnprintf(char *buf, int n, const char *f, __builtin_va_list ap) {
    int o = 0;
#define PUT(c) do { if (o < n - 1) buf[o] = (c); o++; } while (0)
    for (; *f; f++) {
        if (*f != '%') { PUT(*f); continue; }
        f++;
        int left = 0, zero = 0, width = 0, prec = -1, lng = 0;
        for (;; f++) { if (*f == '-') left = 1; else if (*f == '0') zero = 1; else if (*f == '+' || *f == ' ' || *f == '#') ; else break; }
        if (*f == '*') { width = __builtin_va_arg(ap, int); f++; } else while (*f >= '0' && *f <= '9') width = width * 10 + *f++ - '0';
        if (*f == '.') { f++; prec = 0; if (*f == '*') { prec = __builtin_va_arg(ap, int); f++; } else while (*f >= '0' && *f <= '9') prec = prec * 10 + *f++ - '0'; }
        while (*f == 'l' || *f == 'h') { if (*f == 'l') lng++; f++; }
        char tmp[40]; const char *s = tmp; int len = 0, neg = 0;
        switch (*f) {
        case 'd': case 'i': case 'u': case 'x': case 'X': case 'p': case 'o': {
            unsigned long long v;
            if (*f == 'd' || *f == 'i') {
                long long sv = lng > 1 ? __builtin_va_arg(ap, long long) : __builtin_va_arg(ap, int);
                if (sv < 0) { neg = 1; v = -sv; } else v = sv;
            } else v = lng > 1 ? __builtin_va_arg(ap, unsigned long long) : __builtin_va_arg(ap, unsigned int);
            int base = (*f == 'x' || *f == 'X' || *f == 'p') ? 16 : *f == 'o' ? 8 : 10;
            const char *dg = *f == 'X' ? "0123456789ABCDEF" : "0123456789abcdef";
            char r[24]; int k = 0;
            do { r[k++] = dg[v % base]; v /= base; } while (v);
            while (k < prec) r[k++] = '0';
            if (neg) tmp[len++] = '-';
            if (zero && !left) while (len + k < width) tmp[len++] = '0';
            while (k) tmp[len++] = r[--k];
            break;
        }
        case 'c': tmp[0] = (char)__builtin_va_arg(ap, int); len = 1; break;
        case 's': s = __builtin_va_arg(ap, const char *); if (!s) s = "(null)"; len = slen(s); if (prec >= 0 && len > prec) len = prec; break;
        case '%': tmp[0] = '%'; len = 1; break;
        default: tmp[0] = '%'; tmp[1] = *f; len = 2; break;
        }
        if (!left) while (width-- > len) PUT(' ');
        for (int i = 0; i < len; i++) PUT(s[i]);
        if (left) while (width-- > len) PUT(' ');
    }
    if (n > 0) buf[o < n ? o : n - 1] = 0;
    return o;
#undef PUT
}

void port_printf(const char *f, ...) {
    char b[1024]; __builtin_va_list ap; __builtin_va_start(ap, f);
    port_vsnprintf(b, sizeof b, f, ap); __builtin_va_end(ap); port_puts(b);
}

void port_fatal(const char *f, ...) {
    char b[1024]; __builtin_va_list ap; __builtin_va_start(ap, f);
    port_vsnprintf(b, sizeof b, f, ap); __builtin_va_end(ap);
    port_puts("[port] FATAL: "); port_puts(b); port_puts("\n");
    port_exit(1);
}

/* ------------------------------------------------------------------------------------------- 64-bit helpers (no libgcc) */
static unsigned long long udivmod(unsigned long long n, unsigned long long d, unsigned long long *rem) {
    unsigned long long q = 0, r = 0;
    if (!d) { if (rem) *rem = n; return ~0ull; }
    for (int i = 63; i >= 0; i--) {
        r = (r << 1) | ((n >> i) & 1);
        if (r >= d) { r -= d; q |= 1ull << i; }
    }
    if (rem) *rem = r;
    return q;
}
unsigned long long __udivdi3(unsigned long long a, unsigned long long b) { return udivmod(a, b, 0); }
unsigned long long __umoddi3(unsigned long long a, unsigned long long b) { unsigned long long r; udivmod(a, b, &r); return r; }
unsigned long long __udivmoddi4(unsigned long long a, unsigned long long b, unsigned long long *r) { return udivmod(a, b, r); }
long long __divdi3(long long a, long long b) {
    int neg = (a < 0) ^ (b < 0);
    unsigned long long q = udivmod(a < 0 ? -a : a, b < 0 ? -b : b, 0);
    return neg ? -(long long)q : (long long)q;
}
long long __moddi3(long long a, long long b) {
    unsigned long long r; udivmod(a < 0 ? -a : a, b < 0 ? -b : b, &r);
    return a < 0 ? -(long long)r : (long long)r;
}

