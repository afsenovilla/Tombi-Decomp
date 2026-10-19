/* Freestanding i386 Linux runtime for the port prototype: entry point, raw syscalls, the PS1 address space,
 * the SIGSEGV trampoline that turns calls to retail code addresses into calls to the native functions, and
 * a SIGFPE handler that gives MIPS results for division by zero.
 *
 * Why freestanding: the game's structures hold 32-bit pointers, so the prototype is an i386 binary, and the
 * build machine has no 32-bit libc.  A real build links SDL2 + libc (docs/PORT.md, "Host layer"); everything
 * here is small and replaceable. */
#include "port_runtime.h"

#define SYS_exit_group 252
#define SYS_read 3
#define SYS_write 4
#define SYS_open 5
#define SYS_close 6
#define SYS_lseek 19
#define SYS_mmap2 192
#define SYS_rt_sigaction 174
#define SYS_rt_sigreturn 173
#define SYS_nanosleep 162
#define SYS_clock_gettime 265
#define SYS_llseek 140
#define SYS_pread64 180

static long sys3(long n, long a, long b, long c) {
    long r;
    __asm__ volatile("int $0x80" : "=a"(r) : "a"(n), "b"(a), "c"(b), "d"(c) : "memory");
    return r;
}
static long sys5(long n, long a, long b, long c, long d, long e) {
    long r;
    __asm__ volatile("int $0x80" : "=a"(r) : "a"(n), "b"(a), "c"(b), "d"(c), "S"(d), "D"(e) : "memory");
    return r;
}
static long sys6(long n, long a, long b, long c, long d, long e, long f) {
    long r;
    /* ebp carries the 6th argument */
    __asm__ volatile("push %%ebp\n\tmov %7, %%ebp\n\tint $0x80\n\tpop %%ebp"
                     : "=a"(r) : "a"(n), "b"(a), "c"(b), "d"(c), "S"(d), "D"(e), "g"(f) : "memory");
    return r;
}

void port_exit(int code) { for (;;) sys3(SYS_exit_group, code, 0, 0); }

static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }
void port_puts(const char *s) { sys3(SYS_write, 2, (long)s, slen(s)); }

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

/* ------------------------------------------------------------------------------------------- files, time */
int port_open(const char *p) { long r = sys3(SYS_open, (long)p, 0 /* O_RDONLY */, 0); return r < 0 ? -1 : (int)r; }
int port_read(int fd, void *b, int n) { return (int)sys3(SYS_read, fd, (long)b, n); }
int port_create(const char *p) { long r = sys3(SYS_open, (long)p, 0x241 /* O_WRONLY|O_CREAT|O_TRUNC */, 0644); return r < 0 ? -1 : (int)r; }
int port_write(int fd, const void *b, int n) { return (int)sys3(SYS_write, fd, (long)b, n); }
int port_mkdir(const char *p) { return (int)sys3(39 /* SYS_mkdir */, (long)p, 0755, 0); }
void port_close(int fd) { sys3(SYS_close, fd, 0, 0); }
int port_pread(int fd, void *b, int n, long long off) {
    return (int)sys5(SYS_pread64, fd, (long)b, n, (long)(unsigned)off, (long)(off >> 32));
}
long long port_filesize(int fd) {
    long long res = 0;
    sys5(SYS_llseek, fd, 0, 0, (long)&res, 2 /* SEEK_END */);
    return res;
}
struct ts { long s, ns; };
unsigned long long port_time_us(void) {
    struct ts t; sys3(SYS_clock_gettime, 1 /* MONOTONIC */, (long)&t, 0);
    return (unsigned long long)t.s * 1000000ull + t.ns / 1000;
}
void port_sleep_us(unsigned long long us) {
    struct ts t = { (long)(us / 1000000ull), (long)(us % 1000000ull) * 1000 };
    sys3(SYS_nanosleep, (long)&t, 0, 0);
}

static void *map_fixed(unsigned addr, unsigned size) {
    /* PROT_READ|PROT_WRITE (no exec: a call into it faults and is resolved below),
       MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED_NOREPLACE */
    long r = sys6(SYS_mmap2, addr, size, 3, 0x02 | 0x20 | 0x100000, -1, 0);
    if ((unsigned long)r != addr) port_fatal("cannot map PS1 memory at %08x (got %lx)", addr, r);
    return (void *)r;
}

/* ------------------------------------------------------------------------------------------- calls to retail addresses */
static int funcs_sorted;
static int is_ovl_id(u32p id) {
    for (const PortOvl *o = port_ovls; o->name; o++) if (o->id == id) return 1;
    return 0;
}
const char *port_resident_overlay(void) {
    u32p id = *(volatile u32p *)PORT_OVL_BASE;
    for (const PortOvl *o = port_ovls; o->name; o++) if (o->id == id) return o->name;
    int ar = port_areas_count ? port_current_area() : 0;
    return ar >= 0 && port_areas_count ? port_areas[ar].name : ar >= 0 ? "X000" : "?";
}
static int is_vary(u32p a) {
    int l = 0, h = port_vary_count - 1;
    while (l <= h) {
        int m = (l + h) / 2;
        if (port_vary_addrs[m] == a) return 1;
        if (port_vary_addrs[m] < a) l = m + 1; else h = m - 1;
    }
    return 0;
}
static int area_matches(int i) {
    const PortArea *r = &port_areas[i];
    return *(volatile u32p *)r->sig[0][0] == r->sig[0][1] && *(volatile u32p *)r->sig[1][0] == r->sig[1][1];
}
static int cur_area = -1;
int port_current_area(void) {
    if (cur_area >= 0 && area_matches(cur_area)) return cur_area;
    int prev = cur_area;
    cur_area = -1;
    for (int i = 0; i < port_areas_count; i++) if (area_matches(i)) { cur_area = i; break; }
    if (cur_area != prev && cur_area >= 0 && port_verbose)
        port_printf("[port] area overlay %s resident (frame %d)\n", port_areas[cur_area].name, port_frame);
    return cur_area;
}
u32p port_force_interp[8];      /* --interp ADDR: run that function in the MIPS interpreter (checks translations) */
void *port_lookup(u32p a) {
    int lo = 0, hi = port_funcmap_count - 1;
    for (int i = 0; i < 8 && port_force_interp[i]; i++) if (port_force_interp[i] == a) return 0;
    { extern void *port_check_hook(u32p); void *c = port_check_hook(a); if (c) return c; }
    if (a >= PORT_OVL_BASE && a < PORT_OVL_END) {
        u32p id = *(volatile u32p *)PORT_OVL_BASE;
        if (is_ovl_id(id)) {           /* a SYSTEM overlay is resident: only its functions live here */
            for (int i = 0; i < port_ovlmap_count; i++)
                if (port_ovlmap[i].id == id && port_ovlmap[i].addr == a) return port_ovlmap[i].fn;
            return 0;
        }
        /* an area overlay: code that differs between the areas is looked up in the resident area's table */
        if (port_areas_count == 0) {
            if (a >= PORT_AREA_OWN_CODE && *(volatile unsigned short *)0x8009C960u != 0) return 0;
        } else if (is_vary(a)) {
            int ar = port_current_area();
            if (ar < 0) return 0;
            int l = 0, h = port_areamap_count - 1;
            while (l <= h) {
                int m = (l + h) / 2;
                const PortAreaFunc *e = &port_areamap[m];
                if (e->area == ar && e->addr == a) return e->fn;
                if (e->area < ar || (e->area == ar && e->addr < a)) l = m + 1; else h = m - 1;
            }
            return 0;                  /* no C for it in this area: the interpreter runs the loaded code */
        }
    }
    (void)funcs_sorted;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        if (port_funcmap[m].addr == a) return port_funcmap[m].fn;
        if (port_funcmap[m].addr < a) lo = m + 1; else hi = m - 1;
    }
    for (const PortFunc *e = port_extra_funcs; e->fn; e++) if (e->addr == a) return e->fn;
    return 0;
}

static int stub_ret0(void) { return 0; }
#define MAX_STUBS 1024
static const char *stub_seen[MAX_STUBS];
static int stub_count[MAX_STUBS];
static int n_stub_seen;
void port_stub(const char *name, u32p addr) {
    int i;
    for (i = 0; i < n_stub_seen; i++) if (stub_seen[i] == name) break;
    if (i == n_stub_seen && n_stub_seen < MAX_STUBS) { stub_seen[n_stub_seen++] = name; }
    if (i < MAX_STUBS) stub_count[i]++;
    if (i < MAX_STUBS && (stub_count[i] == 1 || port_verbose > 1))
        port_printf("[port] stub %s (%08x) frame %d\n", name, addr, port_frame);
}
void port_stub_report(void) {
    port_printf("[port] %d calls ran in the MIPS interpreter (code without C)\n", port_mips_calls());
    port_printf("[port] %d distinct stubbed functions were called:\n", n_stub_seen);
    for (int i = 0; i < n_stub_seen; i++) port_printf("  %6d  %s\n", stub_count[i], stub_seen[i]);
}

static int interpretable(u32p a) {
    return port_interp && a >= 0x80010000u && a < 0x80200000u && !(a & 3) && !port_is_lib_addr(a);
}
void *port_resolve_call(u32p addr, const char *name) {
    void *f = addr ? port_lookup(addr) : 0;
    if (f) return f;
    if (interpretable(addr)) {            /* no C for it: run the retail machine code */
        port_mips_pending = addr;
        return (void *)port_mips_entry;
    }
    port_stub(name ? name : "?", addr);
    return (void *)stub_ret0;
}

/* SIGSEGV: execution reached a retail address (function pointer from retail data or from a table the game
   built).  The faulting EIP *is* the PS1 address; the return address and the arguments are already on the
   stack (cdecl), so redirecting EIP to the native function completes the call. */
struct ksigaction { void *handler; unsigned long flags; void *restorer; unsigned long mask[2]; };
extern void port_sigreturn(void);
__asm__(".text\n.globl port_sigreturn\nport_sigreturn:\n\tmovl $173, %eax\n\tint $0x80\n");

#define UC_EIP(uc) (((unsigned long *)(uc))[5 + 14])
#define UC_ESP(uc) (((unsigned long *)(uc))[5 + 7])
#define UC_EAX(uc) (((unsigned long *)(uc))[5 + 11])
#define UC_EDX(uc) (((unsigned long *)(uc))[5 + 9])
#define UC_CR2(uc) (((unsigned long *)(uc))[5 + 21])

static void on_segv(int sig, void *info, void *uc) {
    unsigned long eip = UC_EIP(uc);
    unsigned long fault = ((unsigned long *)info)[3];
    if (eip == fault && (eip & 0x5FFFFFFFul) < 0x00200000ul) {   /* KUSEG / KSEG0 / KSEG1 RAM address */
        void *f = port_lookup((u32p)(0x80000000ul | (eip & 0x1FFFFFul)));
        if (f) { UC_EIP(uc) = (unsigned long)f; return; }
        if (interpretable((u32p)(0x80000000ul | (eip & 0x1FFFFFul)))) {
            port_mips_pending = (u32p)(0x80000000ul | (eip & 0x1FFFFFul));
            UC_EIP(uc) = (unsigned long)port_mips_entry;
            return;
        }
        port_stub("<retail address without native code>", (u32p)eip);
        UC_EIP(uc) = (unsigned long)stub_ret0;
        return;
    }
    unsigned long ret = *(unsigned long *)UC_ESP(uc);
    {   /* stack scan: words that look like native return addresses (resolve with addr2line -f -e tombi) */
        extern char __executable_start[], etext[];
        unsigned long *sp = (unsigned long *)UC_ESP(uc);
        port_printf("[port] eax=%08lx edx=%08lx; stack:", UC_EAX(uc), UC_EDX(uc));
        for (int i = 0, n = 0; i < 256 && n < 16; i++)
            if (sp[i] >= (unsigned long)__executable_start && sp[i] < (unsigned long)etext) { port_printf(" %08lx", sp[i]); n++; }
        port_printf("\n");
    }
    port_fatal("signal %d: eip=%08lx fault address=%08lx (frame %d, [esp]=%08lx)", sig, eip, fault, port_frame, ret);
}

/* SIGFPE (x86 traps on division by zero, the R3000 does not): emulate LO = rs < 0 ? 1 : -1, HI = rs */
static void on_fpe(int sig, void *info, void *uc) {
    unsigned char *p = (unsigned char *)UC_EIP(uc);
    int len = 0;
    if (p[0] == 0xF7) {
        unsigned char modrm = p[1]; int mod = modrm >> 6, rm = modrm & 7;
        len = 2;
        if (mod != 3 && rm == 4) len++;                         /* SIB */
        if (mod == 1) len += 1; else if (mod == 2) len += 4;
        else if (mod == 0 && rm == 5) len += 4;
        else if (mod == 0 && rm == 4 && (p[2] & 7) == 5) len += 4;
    }
    if (!len) port_fatal("SIGFPE at %08lx", UC_EIP(uc));
    long rs = (long)UC_EAX(uc);
    UC_EDX(uc) = (unsigned long)rs;
    UC_EAX(uc) = rs < 0 ? 1 : (unsigned long)-1;
    UC_EIP(uc) += len;
    (void)sig; (void)info;
}

static void install(int sig, void *h) {
    struct ksigaction sa = { h, 4 /* SA_SIGINFO */ | 0x04000000 /* SA_RESTORER */ | 0x40000000 /* SA_NODEFER */,
                             (void *)port_sigreturn, { 0, 0 } };
    sys5(SYS_rt_sigaction, sig, (long)&sa, 0, 8, 0);
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

/* ------------------------------------------------------------------------------------------- entry */
int port_verbose;
extern int port_main(int argc, char **argv);

static void map_shared(int fd, unsigned addr, unsigned size, unsigned off) {
    long r = sys6(SYS_mmap2, addr, size, 3, 0x01 /* MAP_SHARED */ | 0x100000, fd, off >> 12);
    if ((unsigned long)r != addr) port_fatal("cannot map PS1 RAM mirror at %08x (got %lx)", addr, r);
}

void port_setup_memory(void) {
    /* One 2 MB RAM object seen at KSEG0 0x80000000 (what the game uses), KUSEG 0x00000000 (the game masks
       addresses with 0xFFFFFF, e.g. compactOrderingTable) and KSEG1 0xA0000000.  The first 64 KB of KUSEG
       (BIOS kernel area) cannot be mapped by a Linux process (vm.mmap_min_addr) and is left out. */
    long fd = sys3(356 /* memfd_create */, (long)"psx-ram", 0, 0);
    if (fd < 0) port_fatal("memfd_create failed (%ld)", fd);
    sys3(93 /* ftruncate */, fd, PSX_RAM_SIZE, 0);
    map_shared((int)fd, PSX_RAM_BASE, PSX_RAM_SIZE, 0);
    map_shared((int)fd, 0x00010000u, PSX_RAM_SIZE - 0x10000u, 0x10000u);
    map_shared((int)fd, 0xA0000000u, PSX_RAM_SIZE, 0);
    map_fixed(PSX_SPAD_BASE, PSX_SPAD_SIZE);
    install(11, (void *)on_segv);   /* SIGSEGV */
    install(8, (void *)on_fpe);     /* SIGFPE */
    install(7, (void *)on_segv);    /* SIGBUS */
}

__attribute__((used, noreturn)) static void cstart(unsigned long *sp) {
    int argc = (int)sp[0];
    char **argv = (char **)(sp + 1);
    port_exit(port_main(argc, argv));
}
__asm__(".text\n.globl _start\n_start:\n\txorl %ebp, %ebp\n\tmovl %esp, %eax\n\tandl $-16, %esp\n"
        "\tsubl $12, %esp\n\tpushl %eax\n\tcall cstart\n\thlt\n");
