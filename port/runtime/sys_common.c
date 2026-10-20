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

/* ------------------------------------------------------------------------------------------- fault diagnostics */
/* Nearest native function at or below a host address, from the tables gen_port.py generates (funcmap, ovlmap,
   areamap, the hand-written extras: every native function the port knows by its retail address); the fault
   handlers print it for each stack word that looks like a return address.  Sorted by native address on first use. */
#define SYM_MAX 8192
static struct { unsigned long a; const char *n; } syms[SYM_MAX];
static int nsyms = -1;
static void sym_add(void *fn, const char *n) { if (nsyms < SYM_MAX && fn) { syms[nsyms].a = (unsigned long)fn; syms[nsyms].n = n; nsyms++; } }
static void sym_init(void) {
    nsyms = 0;
    for (int i = 0; i < port_funcmap_count; i++) sym_add(port_funcmap[i].fn, port_funcmap[i].name);
    for (int i = 0; i < port_ovlmap_count; i++) sym_add(port_ovlmap[i].fn, port_ovlmap[i].name);
    for (int i = 0; i < port_areamap_count; i++)   /* (no name in the table: the area overlay's) */
        sym_add(port_areamap[i].fn, port_areamap[i].area < port_areas_count ? port_areas[port_areamap[i].area].name : "area");
    for (const PortFunc *e = port_extra_funcs; e->fn; e++) sym_add(e->fn, e->name);
    for (int i = 0; i < port_libmap_count; i++) sym_add(port_libmap[i].fn, port_libmap[i].name);   /* shims too */
    for (int gap = nsyms / 2; gap > 0; gap /= 2)   /* shell sort */
        for (int i = gap; i < nsyms; i++) {
            unsigned long a = syms[i].a; const char *n = syms[i].n; int j = i;
            while (j >= gap && syms[j - gap].a > a) { syms[j] = syms[j - gap]; j -= gap; }
            syms[j].a = a; syms[j].n = n;
        }
}
const char *port_sym_near(unsigned long a, unsigned long *off) {
    if (nsyms < 0) sym_init();
    int lo = 0, hi = nsyms - 1, best = -1;
    while (lo <= hi) { int m = (lo + hi) / 2; if (syms[m].a <= a) { best = m; lo = m + 1; } else hi = m - 1; }
    if (best < 0 || a - syms[best].a > 0x4000) return 0;   /* (runtime functions are in no table: a far offset is one of those) */
    if (off) *off = a - syms[best].a;
    return syms[best].n;
}
/* the last call thunk resolved (thunks.S: a call to a function without native code asks port_resolve_call):
   when EIP is a bogus address the resolver's answer is the first suspect */
u32p port_last_resolve_addr; const char *port_last_resolve_name; void *port_last_resolve_fn;
static struct { u32p a; const char *n; void *f; int frame; } resolve_ring[8]; static unsigned resolve_count;
void port_note_resolve(u32p addr, const char *name, void *fn) {
    port_last_resolve_addr = addr; port_last_resolve_name = name; port_last_resolve_fn = fn;
    unsigned i = resolve_count++ & 7;
    resolve_ring[i].a = addr; resolve_ring[i].n = name; resolve_ring[i].f = fn; resolve_ring[i].frame = port_frame;
    if (port_verbose > 2) port_printf("[port] call thunk: %s (%08x) -> %p (frame %d)\n", name ? name : "?", addr, fn, port_frame);
}
void port_dump_stack(const unsigned long *sp, unsigned long lo, unsigned long hi) {
    for (int i = 0, n = 0; i < 256 && n < 16; i++)
        if (sp[i] >= lo && sp[i] < hi) {
            unsigned long off; const char *nm = port_sym_near(sp[i], &off);
            if (nm) port_printf(" %08lx(%s+0x%lx)", sp[i], nm, off); else port_printf(" %08lx", sp[i]);
            n++;
        }
    port_printf("\n");
    if (port_last_resolve_addr)
        port_printf("[port] last call thunk resolved: %s (%08x) -> %p\n", port_last_resolve_name ? port_last_resolve_name : "?",
                    port_last_resolve_addr, port_last_resolve_fn);
    if (resolve_count > 1) {   /* the 8 before it, newest first */
        port_printf("[port] call thunks before it:");
        for (unsigned k = 2; k <= 8 && k <= resolve_count; k++) {
            unsigned i = (resolve_count - k) & 7;
            port_printf(" %s(%08x)->%p@%d", resolve_ring[i].n ? resolve_ring[i].n : "?", resolve_ring[i].a, resolve_ring[i].f, resolve_ring[i].frame);
        }
        port_printf("\n");
    }
    { extern unsigned port_mips_cur_pc, port_mips_cur_entry;
      if (port_mips_cur_pc) port_printf("[port] in the MIPS interpreter: pc %08x (function %08x)\n", port_mips_cur_pc, port_mips_cur_entry); }
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

