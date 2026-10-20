/* Freestanding i386 Win32 runtime for the port: the same interface as sys_linux_i386.c (port_runtime.h) on the
 * Win32 API instead of Linux syscalls.  No C runtime is linked (-nostdlib): the game is compiled with
 * -fno-leading-underscore so that the generated symbol tables (gen_port.py: syms.ld, thunks.S) are the same on
 * both hosts, and the few kernel32/winmm imports are declared here with their decorated names.
 *
 * The PS1 address space: one 2 MB pagefile-backed section mapped at KSEG0 0x80000000 and KSEG1 0xA0000000
 * (MapViewOfFileEx at a fixed address; the image is linked /LARGEADDRESSAWARE and based at 0x10000000 so the
 * upper half is free), and, when Windows left that range free, also at KUSEG 0x00010000.  The scratchpad +
 * I/O page is a VirtualAlloc at 0x1F800000.  Page 0 (BIOS kernel area through NULL pointers) cannot be mapped
 * on Windows: port_low_mem stays -1.
 *
 * Calls to retail addresses: the RAM is mapped PAGE_READWRITE and the image is linked /NXCOMPAT, so a call into
 * it raises an access violation with the faulting EIP = the PS1 address; a vectored exception handler
 * rewrites EIP to the native function (cdecl: arguments and return address already on the stack), exactly as
 * the SIGSEGV trampoline on Linux.  EXCEPTION_INT_DIVIDE_BY_ZERO is given the R3000 result. */
#include "port_runtime.h"

typedef unsigned long DWORD; typedef void *HANDLE; typedef int BOOL; typedef unsigned long long u64;
#define WINAPI __attribute__((stdcall))
#define IMPORT(ret, name, args, dec) ret WINAPI name args __asm__(dec)
IMPORT(HANDLE, CreateFileA, (const char *, DWORD, DWORD, void *, DWORD, DWORD, HANDLE), "_CreateFileA@28");
IMPORT(BOOL, ReadFile, (HANDLE, void *, DWORD, DWORD *, void *), "_ReadFile@20");
IMPORT(BOOL, WriteFile, (HANDLE, const void *, DWORD, DWORD *, void *), "_WriteFile@20");
IMPORT(BOOL, CloseHandle, (HANDLE), "_CloseHandle@4");
IMPORT(BOOL, GetFileSizeEx, (HANDLE, long long *), "_GetFileSizeEx@8");
IMPORT(BOOL, CreateDirectoryA, (const char *, void *), "_CreateDirectoryA@8");
IMPORT(HANDLE, GetStdHandle, (DWORD), "_GetStdHandle@4");
IMPORT(void, ExitProcess, (unsigned), "_ExitProcess@4");
IMPORT(DWORD, GetLastError, (void), "_GetLastError@0");
IMPORT(void *, VirtualAlloc, (void *, unsigned, DWORD, DWORD), "_VirtualAlloc@16");
IMPORT(HANDLE, CreateFileMappingA, (HANDLE, void *, DWORD, DWORD, DWORD, const char *), "_CreateFileMappingA@24");
IMPORT(void *, MapViewOfFileEx, (HANDLE, DWORD, DWORD, DWORD, unsigned, void *), "_MapViewOfFileEx@24");
IMPORT(void *, AddVectoredExceptionHandler, (DWORD, void *), "_AddVectoredExceptionHandler@8");
IMPORT(BOOL, QueryPerformanceCounter, (long long *), "_QueryPerformanceCounter@4");
IMPORT(BOOL, QueryPerformanceFrequency, (long long *), "_QueryPerformanceFrequency@4");
IMPORT(void, Sleep, (DWORD), "_Sleep@4");
IMPORT(char *, GetCommandLineA, (void), "_GetCommandLineA@0");
IMPORT(BOOL, CreatePipe, (HANDLE *, HANDLE *, void *, DWORD), "_CreatePipe@16");
IMPORT(BOOL, SetHandleInformation, (HANDLE, DWORD, DWORD), "_SetHandleInformation@12");
IMPORT(BOOL, PeekNamedPipe, (HANDLE, void *, DWORD, DWORD *, DWORD *, DWORD *), "_PeekNamedPipe@24");
IMPORT(BOOL, CreateProcessA, (const char *, char *, void *, void *, BOOL, DWORD, void *, const char *, void *, void *), "_CreateProcessA@40");
IMPORT(unsigned, timeBeginPeriod, (unsigned), "_timeBeginPeriod@4");
IMPORT(unsigned, VirtualQuery, (const void *, void *, unsigned), "_VirtualQuery@12");
IMPORT(void *, VirtualAllocEx, (HANDLE, void *, unsigned, DWORD, DWORD), "_VirtualAllocEx@20");
IMPORT(BOOL, VirtualFree, (void *, unsigned, DWORD), "_VirtualFree@12");
IMPORT(DWORD, ResumeThread, (HANDLE), "_ResumeThread@4");
IMPORT(DWORD, WaitForSingleObject, (HANDLE, DWORD), "_WaitForSingleObject@8");
IMPORT(BOOL, GetExitCodeProcess, (HANDLE, DWORD *), "_GetExitCodeProcess@8");
IMPORT(DWORD, GetEnvironmentVariableA, (const char *, char *, DWORD), "_GetEnvironmentVariableA@12");
IMPORT(BOOL, SetEnvironmentVariableA, (const char *, const char *), "_SetEnvironmentVariableA@8");

#define INVALID_HANDLE ((HANDLE)-1)

/* ------------------------------------------------------------------------------------------- fds */
#define MAX_FD 64
static HANDLE fds[MAX_FD];            /* 0..2: std handles; the rest: files and pipes opened here */
static unsigned char fd_nonblock[MAX_FD];
static int fd_new(HANDLE h) {
    if (h == INVALID_HANDLE || !h) return -1;
    for (int i = 3; i < MAX_FD; i++) if (!fds[i]) { fds[i] = h; fd_nonblock[i] = 0; return i; }
    CloseHandle(h);
    return -1;
}
static HANDLE hof(int fd) { return fd >= 0 && fd < MAX_FD ? fds[fd] : 0; }

void port_exit(int code) { for (;;) ExitProcess(code); }
static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }
void port_puts(const char *s) { DWORD n; if (fds[2]) WriteFile(fds[2], s, slen(s), &n, 0); }

/* ------------------------------------------------------------------------------------------- files */
#define GENERIC_READ 0x80000000u
#define GENERIC_WRITE 0x40000000u
#define FILE_SHARE_READ 1
#define FILE_SHARE_WRITE 2
#define OPEN_EXISTING 3
#define CREATE_ALWAYS 2
#define FILE_ATTRIBUTE_NORMAL 0x80
int port_open(const char *p) {
    return fd_new(CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0));
}
int port_create(const char *p) {
    return fd_new(CreateFileA(p, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0));
}
/* --pipe-frames / --pipe-audio name a FIFO on Linux; on Windows they are plain files (a named pipe would need a
   server: use --window instead) */
int port_open_pipe(const char *p) {
    return fd_new(CreateFileA(p, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0));
}
int port_read(int fd, void *b, int n) {
    HANDLE h = hof(fd); DWORD got = 0;
    if (!h) return -1;
    if (fd_nonblock[fd]) {              /* the pad pipe: do not wait for the viewer */
        DWORD avail = 0;
        if (!PeekNamedPipe(h, 0, 0, 0, &avail, 0)) return 0;     /* broken pipe: EOF */
        if (!avail) return -1;                                   /* nothing yet (EAGAIN) */
        if ((int)avail < n) n = (int)avail;
    }
    if (!ReadFile(h, b, n, &got, 0)) return GetLastError() == 109 /* ERROR_BROKEN_PIPE */ ? 0 : -1;
    return (int)got;
}
int port_write(int fd, const void *b, int n) {
    HANDLE h = hof(fd); DWORD put = 0;
    if (!h) return -1;
    if (!WriteFile(h, b, n, &put, 0)) return -1;
    return (int)put;
}
int port_write_all(int fd, const void *b, int n) {
    const char *c = b; int left = n;
    while (left > 0) {
        int r = port_write(fd, c, left);
        if (r <= 0) return n - left;
        c += r; left -= r;
    }
    return n;
}
void port_close(int fd) { HANDLE h = hof(fd); if (h && fd >= 3) { CloseHandle(h); fds[fd] = 0; } }
int port_mkdir(const char *p) { return CreateDirectoryA(p, 0) ? 0 : -1; }
struct overlapped { unsigned long internal, internal_high, off, off_high; HANDLE ev; };
static int last_error;
int port_last_error(void) { return last_error; }
int port_pread(int fd, void *b, int n, long long off) {
    HANDLE h = hof(fd); DWORD got = 0;
    struct overlapped o = { 0, 0, (unsigned long)off, (unsigned long)(off >> 32), 0 };
    if (!h) { last_error = 6 /* ERROR_INVALID_HANDLE */; return -1; }
    last_error = 0;
    if (!ReadFile(h, b, n, &got, &o)) { last_error = (int)GetLastError(); return last_error == 38 /* ERROR_HANDLE_EOF */ ? (int)got : -1; }
    return (int)got;
}
int port_pwrite(int fd, const void *b, int n, long long off) {
    HANDLE h = hof(fd); DWORD put = 0;
    struct overlapped o = { 0, 0, (unsigned long)off, (unsigned long)(off >> 32), 0 };
    if (!h) return -1;
    if (!WriteFile(h, b, n, &put, &o)) return -1;
    return (int)put;
}
long long port_filesize(int fd) { long long s = 0; HANDLE h = hof(fd); if (h) GetFileSizeEx(h, &s); return s; }

/* ------------------------------------------------------------------------------------------- time */
static long long qpc_freq;
unsigned long long port_time_us(void) {
    long long c = 0;
    if (!qpc_freq) QueryPerformanceFrequency(&qpc_freq);
    QueryPerformanceCounter(&c);
    /* split: c * 1000000 overflows 64 bits after 30 minutes of uptime with a 10 MHz counter (the game then ran
       unpaced: music and banners far too fast) */
    u64 f = (u64)qpc_freq, q = (u64)c / f, r = (u64)c % f;
    return q * 1000000ull + r * 1000000ull / f;
}
/* sleep until an absolute QPC time: timeBeginPeriod(1) is set at start, so Sleep() wakes within about 1 ms (15.6 ms
   without it); sleep until ~2 ms before the deadline, then spin on the counter */
void port_sleep_until_us(unsigned long long deadline) {
    for (;;) {
        u64 now = port_time_us();
        if (now >= deadline) return;
        u64 left = deadline - now;
        if (left > 2500) Sleep((DWORD)((left - 2000) / 1000));
        else break;
    }
    while (port_time_us() < deadline) ;
}
void port_sleep_us(unsigned long long us) { port_sleep_until_us(port_time_us() + us); }

/* ------------------------------------------------------------------------------------------- the viewer process */
/* --window: fds[0] = our write end of the viewer's stdin (video), fds[1] = write end of its audio pipe, fds[2] =
   read end of its pad pipe (non-blocking).  The viewer cannot inherit fd numbers 3 and 4 on Windows: it gets
   the handle values as "--audio-handle N --pad-handle N" on its command line (tombi_view.c). */
struct sec_attr { DWORD n; void *desc; BOOL inherit; };
struct startupinfo {
    DWORD cb; char *reserved, *desktop, *title; DWORD x, y, xs, ys, xc, yc, fill, flags; unsigned short show, cb2;
    unsigned char *res2; HANDLE in, out, err;
};
struct procinfo { HANDLE process, thread; DWORD pid, tid; };
static char cmdline[2048];
static void cat(const char *s) { int n = slen(cmdline), k = 0; while (s[k] && n < (int)sizeof cmdline - 1) cmdline[n++] = s[k++]; cmdline[n] = 0; }
static void cat_u(unsigned v) { char b[12]; int k = 0; do { b[k++] = '0' + v % 10; v /= 10; } while (v); char r[12]; int j = 0; while (k) r[j++] = b[--k]; r[j] = 0; cat(r); }
int port_spawn(const char *path, char *const argv[], int out_fds[3]) {
    struct sec_attr sa = { sizeof sa, 0, 1 };
    HANDLE vr, vw, ar, aw, pr, pw;
    if (!CreatePipe(&vr, &vw, &sa, 1 << 20) || !CreatePipe(&ar, &aw, &sa, 1 << 20) || !CreatePipe(&pr, &pw, &sa, 1 << 16)) return -1;
    SetHandleInformation(vw, 1 /* HANDLE_FLAG_INHERIT */, 0);
    SetHandleInformation(aw, 1, 0);
    SetHandleInformation(pr, 1, 0);
    cmdline[0] = 0;
    cat("\""); cat(path); cat("\"");
    for (int i = 1; argv[i]; i++) { cat(" \""); cat(argv[i]); cat("\""); }
    cat(" --audio-handle "); cat_u((unsigned)ar);
    cat(" --pad-handle "); cat_u((unsigned)pw);
    struct startupinfo si; struct procinfo pi;
    for (unsigned i = 0; i < sizeof si; i++) ((char *)&si)[i] = 0;
    si.cb = sizeof si; si.flags = 0x100 /* STARTF_USESTDHANDLES */;
    si.in = vr; si.out = fds[1] ? fds[1] : INVALID_HANDLE; si.err = fds[2] ? fds[2] : INVALID_HANDLE;
    if (!CreateProcessA(0, cmdline, 0, 0, 1, 0, 0, 0, &si, &pi)) {
        port_printf("[port] CreateProcess(%s) failed (%lu)\n", path, GetLastError());
        return -1;
    }
    CloseHandle(pi.thread); CloseHandle(pi.process);
    CloseHandle(vr); CloseHandle(ar); CloseHandle(pw);
    out_fds[0] = fd_new(vw); out_fds[1] = fd_new(aw); out_fds[2] = fd_new(pr);
    if (out_fds[2] >= 0) fd_nonblock[out_fds[2]] = 1;
    return (int)pi.pid;
}

/* ------------------------------------------------------------------------------------------- PS1 memory */
/* i386 CONTEXT offsets: Edi 0x9C Esi 0xA0 Ebx 0xA4 Edx 0xA8 Ecx 0xAC Eax 0xB0 Ebp 0xB4 Eip 0xB8 Esp 0xC4 */
#define CTX(ctx, off) (*(unsigned long *)((char *)(ctx) + (off)))
#define C_EDI 0x9C
#define C_ESI 0xA0
#define C_EBX 0xA4
#define C_EDX 0xA8
#define C_ECX 0xAC
#define C_EAX 0xB0
#define C_EBP 0xB4
#define C_EIP 0xB8
#define C_ESP 0xC4
static unsigned char *scratch;        /* PAGE_EXECUTE_READWRITE */
#define PAGE_READWRITE 4
#define FILE_MAP_ALL_ACCESS 0xF001F
int port_low_mem = -1;
static int kuseg_child;   /* this process was started by relaunch_reserved below */
void port_setup_memory(void) {
    HANDLE m = CreateFileMappingA(INVALID_HANDLE, 0, PAGE_READWRITE, 0, PSX_RAM_SIZE, 0);
    if (!m) port_fatal("CreateFileMapping failed (%lu)", GetLastError());
    if (MapViewOfFileEx(m, FILE_MAP_ALL_ACCESS, 0, 0, PSX_RAM_SIZE, (void *)PSX_RAM_BASE) != (void *)PSX_RAM_BASE)
        port_fatal("cannot map PS1 RAM at %08x (%lu): the image must be large-address-aware", PSX_RAM_BASE, GetLastError());
    if (MapViewOfFileEx(m, FILE_MAP_ALL_ACCESS, 0, 0, PSX_RAM_SIZE, (void *)0xA0000000u) != (void *)0xA0000000u)
        port_fatal("cannot map the KSEG1 mirror at A0000000 (%lu)", GetLastError());
    /* KUSEG mirror (the game masks some addresses with 0xFFFFFF, and walks NULL-based tables into it): the whole
       range when the loader left 0x10000..0x1FFFFF free, else every free 64 KB block of it (a block that is in
       use reads the host's data there, as the warning says; Wine keeps its own data in this range) */
    if (kuseg_child) {   /* the parent reserved the blocks in this process before its heap existed (see start) */
        unsigned n = 0;
        for (unsigned a = 0x10000; a < PSX_RAM_SIZE; a += 0x10000) {
            unsigned long mbi[7] = { 0 };
            VirtualQuery((void *)a, mbi, sizeof mbi);
            if (mbi[1] == a && mbi[4] == 0x2000 /* MEM_RESERVE */ && mbi[3] == 0x10000 && VirtualFree((void *)a, 0, 0x8000 /* MEM_RELEASE */)) n++;
        }
        if (port_verbose) port_printf("[port] KUSEG range: %u blocks of 64 KB reserved by the parent process, released for the mirror\n", n);
    }
    if (port_kuseg_strict == 2) {   /* --no-kuseg: nothing mapped, every access faults into low_access and is logged */
        port_printf("[port] --no-kuseg: the KUSEG mirror is not mapped; every access below 0x200000 is emulated and logged\n");
        for (unsigned a = 0x10000; a < PSX_RAM_SIZE; a += 0x10000) VirtualAlloc((void *)a, 0x10000, 0x2000 /* MEM_RESERVE */, 0x01 /* PAGE_NOACCESS */);
    } else if (port_kuseg_strict == 3) {   /* --fake-kuseg: what a block in use by Windows does to the game */
        port_printf("[port] --fake-kuseg: private memory instead of the mirror (simulates blocks the host owns)\n");
        for (unsigned a = 0x10000; a < PSX_RAM_SIZE; a += 0x10000) VirtualAlloc((void *)a, 0x10000, 0x3000, 0x04);
    }
    else if (MapViewOfFileEx(m, FILE_MAP_ALL_ACCESS, 0, 0x10000, PSX_RAM_SIZE - 0x10000, (void *)0x00010000u) != (void *)0x00010000u) {
        unsigned mapped = 0, missing = 0;
        port_printf("[port] warning: KUSEG mirror 00010000-001FFFFF partly in use by Windows; blocks missing:");
        for (unsigned a = 0x10000; a < PSX_RAM_SIZE; a += 0x10000) {
            unsigned long mbi[7] = { 0 };
            VirtualQuery((void *)a, mbi, sizeof mbi);
            if (mbi[4] == 0x10000 /* MEM_FREE */ && mbi[0] + mbi[3] >= a + 0x10000 &&
                MapViewOfFileEx(m, FILE_MAP_ALL_ACCESS, 0, a, 0x10000, (void *)a) == (void *)a) mapped += 0x10000;
            else { missing += 0x10000; port_printf(" %05x(%s)", a, mbi[4] == 0x10000 ? "free" : mbi[4] == 0x2000 ? "reserved" : "committed"); }
        }
        port_printf("\n[port] %u KB mapped, %u KB missing: a game access to a missing block reads and writes the host's own data there\n",
                    mapped >> 10, missing >> 10);
        if (port_kuseg_strict)
            port_fatal("--kuseg-strict: the KUSEG mirror could not be placed whole");
        port_printf("[port] (the game draws through KUSEG pointers into 00090000-000CFFFF: missing blocks there corrupt the "
                    "host and the picture; --kuseg-strict stops here)\n");
    } else if (port_verbose) port_printf("[port] KUSEG mirror 00010000-001FFFFF mapped whole%s\n", kuseg_child ? " (relaunched)" : "");
    if (VirtualAlloc((void *)PSX_SPAD_BASE, PSX_SPAD_SIZE, 0x3000 /* MEM_COMMIT|MEM_RESERVE */, PAGE_READWRITE) != (void *)PSX_SPAD_BASE)
        port_fatal("cannot map the scratchpad at %08x (%lu)", PSX_SPAD_BASE, GetLastError());
    scratch = VirtualAlloc(0, 0x1000, 0x3000, 0x40 /* PAGE_EXECUTE_READWRITE */);
    if (!scratch) port_fatal("VirtualAlloc failed (%lu)", GetLastError());
    port_low_mem = 0;   /* page 0 is emulated (low_access) */
}

/* ------------------------------------------------------------------------------------------- calls to retail addresses */
/* This part is the same as sys_linux_i386.c (lookup tables, stubs, interpreter fallback). */
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
u32p port_force_interp[8];
void *port_lookup(u32p a) {
    int lo = 0, hi = port_funcmap_count - 1;
    for (int i = 0; i < 8 && port_force_interp[i]; i++) if (port_force_interp[i] == a) return 0;
    { extern void *port_check_hook(u32p); void *c = port_check_hook(a); if (c) return c; }
    if (a >= PORT_OVL_BASE && a < PORT_OVL_END) {
        u32p id = *(volatile u32p *)PORT_OVL_BASE;
        if (is_ovl_id(id)) {
            for (int i = 0; i < port_ovlmap_count; i++)
                if (port_ovlmap[i].id == id && port_ovlmap[i].addr == a) return port_ovlmap[i].fn;
            return 0;
        }
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
            return 0;
        }
    }
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
static void *resolve_call_1(u32p addr, const char *name) {
    void *f = addr ? port_lookup(addr) : 0;
    port_last_resolve_addr = addr; port_last_resolve_name = name;
    if (f) return port_last_resolve_fn = f;
    if (interpretable(addr)) { port_mips_pending = addr; return port_last_resolve_fn = (void *)port_mips_entry; }
    port_stub(name ? name : "?", addr);
    return port_last_resolve_fn = (void *)stub_ret0;
}
void *port_resolve_call(u32p addr, const char *name) {
    void *f = resolve_call_1(addr, name);
    port_note_resolve(addr, name, f);   /* the last 8 answers are printed by a fault report */
    return f;
}

/* Memory accesses below 0x200000 that Windows cannot map: page 0 (the BIOS kernel area, read by the game through
   NULL pointers) and, when the KUSEG mirror could not be placed, the masked RAM addresses 0x10000..0x1FFFFF
   (port_low_emulate, sys_common.c; the game's own masks are rewritten to KSEG0 by gen_port.py, so this is a
   fallback, tallied by port_low_report) */
static int low_access(void *ctx, unsigned long eip, unsigned long fault) {
    unsigned long neweip;
    int r = port_low_emulate(eip, fault, scratch, &neweip);
    if (r == PORT_LOW_JUMP) { CTX(ctx, C_EIP) = neweip; return 1; }
    if (r == PORT_LOW_REGS) {
        if (CTX(ctx, C_ESI) < 0x00200000ul) CTX(ctx, C_ESI) |= 0x80000000ul;
        if (CTX(ctx, C_EDI) < 0x00200000ul) CTX(ctx, C_EDI) |= 0x80000000ul;
        return 1;
    }
    return 0;
}

/* Vectored exception handler.  EXCEPTION_POINTERS = { EXCEPTION_RECORD *, CONTEXT * }; i386 CONTEXT offsets:
   Edi 0x9C Esi 0xA0 Ebx 0xA4 Edx 0xA8 Ecx 0xAC Eax 0xB0 Ebp 0xB4 Eip 0xB8 Esp 0xC4. */
#define EXCEPTION_ACCESS_VIOLATION 0xC0000005u
#define EXCEPTION_INT_DIVIDE_BY_ZERO 0xC0000094u
/* where the loader put the image minus where it was linked (0x10000000, port/CMakeLists.txt): non-zero means every
   rel32 call to a syms.ld address misses by this much (see on_exception) */
extern char __ImageBase[] __asm__("___ImageBase");
static unsigned long image_delta;
#ifndef PORT_LINK_BASE
#define PORT_LINK_BASE 0x10000000ul   /* -Wl,--image-base in port/CMakeLists.txt */
#endif
#define EXCEPTION_CONTINUE_EXECUTION (-1)
#define EXCEPTION_CONTINUE_SEARCH 0
static long WINAPI on_exception(void **ep) {
    unsigned long *rec = ep[0]; void *ctx = ep[1];
    unsigned long code = rec[0], eip = CTX(ctx, C_EIP);
    if (code == EXCEPTION_INT_DIVIDE_BY_ZERO) {   /* x86 traps, the R3000 gives LO = rs < 0 ? 1 : -1, HI = rs */
        unsigned char *p = (unsigned char *)eip;
        int len = 0;
        if (p[0] == 0xF7) {
            unsigned char modrm = p[1]; int mod = modrm >> 6, rm = modrm & 7;
            len = 2;
            if (mod != 3 && rm == 4) len++;
            if (mod == 1) len += 1; else if (mod == 2) len += 4;
            else if (mod == 0 && rm == 5) len += 4;
            else if (mod == 0 && rm == 4 && (p[2] & 7) == 5) len += 4;
        }
        if (!len) port_fatal("divide by zero at %08lx", eip);
        long rs = (long)CTX(ctx, C_EAX);
        CTX(ctx, C_EDX) = (unsigned long)rs;
        CTX(ctx, C_EAX) = rs < 0 ? 1 : (unsigned long)-1;
        CTX(ctx, C_EIP) += len;
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    if (code != EXCEPTION_ACCESS_VIOLATION) return EXCEPTION_CONTINUE_SEARCH;
    unsigned long fault = rec[6];   /* ExceptionInformation[1] */
    if (eip != fault && fault < 0x00200000ul && low_access(ctx, eip, fault)) return EXCEPTION_CONTINUE_EXECUTION;
    /* a relocated image (the loader did not get the preferred base 0x10000000: ASLR, or the range in use): every
       `call FUN_800xxxxx` to a syms.ld address is a rel32 computed at link time, so it lands at the retail
       address + the relocation delta (eip=7f3206e8 for 800406e8 with the image at 0f2e0000); undo the delta */
    if (eip == fault && image_delta && ((eip - image_delta) & 0x5FFFFFFFul) < 0x00200000ul) {
        static int n; if (n++ < 8 || port_verbose > 1) port_printf("[port] jump to %08lx = retail %08lx + image delta (relocated image)\n", eip, eip - image_delta);
        eip -= image_delta; fault = eip;
    }
    if (eip == fault && (eip & 0x5FFFFFFFul) < 0x00200000ul) {
        u32p a = (u32p)(0x80000000ul | (eip & 0x1FFFFFul));
        void *f = port_lookup(a);
        if (port_verbose > 1) { static int n; if (n++ < 4000) port_printf("[port] redirect %08lx -> %p%s (frame %d)\n", eip, f, f ? "" : " (no native code)", port_frame); }
        if (f) { CTX(ctx, C_EIP) = (unsigned long)f; return EXCEPTION_CONTINUE_EXECUTION; }
        if (interpretable(a)) {
            port_mips_pending = a;
            CTX(ctx, C_EIP) = (unsigned long)port_mips_entry;
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        port_stub("<retail address without native code>", (u32p)eip);
        CTX(ctx, C_EIP) = (unsigned long)stub_ret0;
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    unsigned long *sp = (unsigned long *)CTX(ctx, C_ESP);
    {   /* stack scan: words inside the image (wherever the loader put it), with the nearest native function
           (sys_common.c port_dump_stack); the symbols in the image are at <image base> + the offsets of
           i686-w64-mingw32-nm tombi.exe - 0x10000000 */
        unsigned long base = (unsigned long)__ImageBase, off; const char *nm = port_sym_near(eip, &off);
        port_printf("[port] eax=%08lx edx=%08lx ecx=%08lx ebx=%08lx esi=%08lx edi=%08lx ebp=%08lx esp=%08lx (image base %08lx, thread %d)\n",
                    CTX(ctx, C_EAX), CTX(ctx, C_EDX), CTX(ctx, C_ECX), CTX(ctx, C_EBX), CTX(ctx, C_ESI), CTX(ctx, C_EDI), CTX(ctx, C_EBP),
                    (unsigned long)sp, base, port_thread_current());
        if (nm) port_printf("[port] eip in %s+0x%lx\n", nm, off);
        port_printf("[port] stack:");
        port_dump_stack(sp, base, base + 0x01000000ul);
    }
    port_low_report();   /* emulated low accesses so far: a partial KUSEG mirror in use shows here */
    port_fatal("access violation: eip=%08lx fault address=%08lx (%s, frame %d, [esp]=%08lx)%s", eip, fault,
               rec[5] == 8 ? "execute" : rec[5] ? "write" : "read", port_frame, sp[0],   /* ExceptionInformation[0] */
               eip == fault ? ": a jump to a bogus address (a function pointer or return address read from memory)" : "");
}

/* gcc probes the stack of frames over 4 KB through this (Windows grows the stack one guard page at a time) */
__asm__(".text\n.globl __chkstk_ms\n.globl ___chkstk_ms\n__chkstk_ms:\n___chkstk_ms:\n"
        "\tpushl %ecx\n\tpushl %eax\n\tcmpl $0x1000, %eax\n\tleal 12(%esp), %ecx\n\tjb 1f\n"
        "2:\tsubl $0x1000, %ecx\n\torl $0, (%ecx)\n\tsubl $0x1000, %eax\n\tcmpl $0x1000, %eax\n\tja 2b\n"
        "1:\tsubl %eax, %ecx\n\torl $0, (%ecx)\n\tpopl %eax\n\tpopl %ecx\n\tret\n");

/* ------------------------------------------------------------------------------------------- entry */
int port_verbose, port_kuseg_strict;
char **port_envp;
extern int port_main(int argc, char **argv);
static char *argv_buf[64];
static char cmd_copy[4096];

/* GetCommandLineA -> argv: quotes group words, a backslash before a quote escapes it (enough for file paths) */
static int split_args(char *s, char **argv, int max) {
    int argc = 0;
    while (*s && argc < max - 1) {
        while (*s == ' ' || *s == '\t') s++;
        if (!*s) break;
        char *out = s; argv[argc++] = s;
        int q = 0;
        while (*s && (q || (*s != ' ' && *s != '\t'))) {
            if (*s == '"') { q = !q; s++; continue; }
            if (*s == '\\' && s[1] == '"') s++;
            *out++ = *s++;
        }
        if (*s) s++;
        *out = 0;
    }
    argv[argc] = 0;
    return argc;
}

/* The KUSEG mirror needs 0x10000..0x1FFFFF free, and Windows puts its own early allocations there (the process
   parameters, the default heap, sometimes a thread stack): the game draws through KUSEG pointers (masked with
   0xFFFFFF: the ordering tables and primitive buffers at 0x80090000-0x800CFFFF), so a block in use by Windows
   means the game writes its packets over the heap and reads them back from the KSEG0 view (garbage picture, then
   a crash into a wild pointer).  When the range is not free, the exe starts itself again suspended, reserves every
   free 64 KB block of the range in the child before its loader runs (no heap yet), resumes it and waits; the child
   (PORT_KUSEG_CHILD=1) releases the reservations in port_setup_memory and maps the mirror there. */
static int kuseg_free(void) {
    for (unsigned a = 0x10000; a < PSX_RAM_SIZE; a += 0x10000) {
        unsigned long mbi[7] = { 0 };
        VirtualQuery((void *)a, mbi, sizeof mbi);
        if (mbi[4] != 0x10000 /* MEM_FREE */) return 0;
    }
    return 1;
}
static void relaunch_reserved(void) {
    struct startupinfo si; struct procinfo pi;
    for (unsigned i = 0; i < sizeof si; i++) ((char *)&si)[i] = 0;
    si.cb = sizeof si; si.flags = 0x100 /* STARTF_USESTDHANDLES */;
    si.in = fds[0] ? fds[0] : INVALID_HANDLE; si.out = fds[1] ? fds[1] : INVALID_HANDLE; si.err = fds[2] ? fds[2] : INVALID_HANDLE;
    SetEnvironmentVariableA("PORT_KUSEG_CHILD", "1");
    cmdline[0] = 0; cat(GetCommandLineA());
    if (!CreateProcessA(0, cmdline, 0, 0, 1, 4 /* CREATE_SUSPENDED */, 0, 0, &si, &pi)) {
        port_printf("[port] relaunch with the KUSEG range reserved failed (%lu): continuing in this process\n", GetLastError());
        SetEnvironmentVariableA("PORT_KUSEG_CHILD", 0);
        return;
    }
    unsigned n = 0;
    for (unsigned a = 0x10000; a < PSX_RAM_SIZE; a += 0x10000)
        if (VirtualAllocEx(pi.process, (void *)a, 0x10000, 0x2000 /* MEM_RESERVE */, 0x01 /* PAGE_NOACCESS */) == (void *)a) n++;
    if (port_verbose) port_printf("[port] KUSEG range 00010000-001FFFFF not free in this process: relaunched with %u of 31 blocks reserved (pid %lu)\n", n, pi.pid);
    ResumeThread(pi.thread);
    WaitForSingleObject(pi.process, (DWORD)-1);
    DWORD code = 1;
    GetExitCodeProcess(pi.process, &code);
    port_exit((int)code);
}

__attribute__((used, noreturn)) void start(void) {
    fds[0] = GetStdHandle((DWORD)-10); fds[1] = GetStdHandle((DWORD)-11); fds[2] = GetStdHandle((DWORD)-12);
    if (fds[2] == INVALID_HANDLE) fds[2] = 0;
    const char *c = GetCommandLineA();
    int n = 0; while (c[n] && n < (int)sizeof cmd_copy - 1) { cmd_copy[n] = c[n]; n++; } cmd_copy[n] = 0;
    int argc = split_args(cmd_copy, argv_buf, 64);
    for (int i = 1; i < argc; i++) if (argv_buf[i][0] == '-' && argv_buf[i][1] == 'v' && !argv_buf[i][2]) port_verbose++;
    { char b[4]; kuseg_child = GetEnvironmentVariableA("PORT_KUSEG_CHILD", b, sizeof b) > 0; }
    if (!kuseg_child && !kuseg_free()) relaunch_reserved();
    timeBeginPeriod(1);
    image_delta = (unsigned long)__ImageBase - PORT_LINK_BASE;
    if (image_delta)
        port_printf("[port] warning: the image was loaded at %08lx instead of its link base %08lx (ASLR or the range in use): "
                    "calls to retail addresses are corrected in the exception handler\n", (unsigned long)__ImageBase, PORT_LINK_BASE);
    else if (port_verbose) port_printf("[port] image at its link base %08lx\n", PORT_LINK_BASE);
    AddVectoredExceptionHandler(1, (void *)on_exception);
    port_exit(port_main(argc, argv_buf));
}
