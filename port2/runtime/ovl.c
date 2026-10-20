/* Overlay dispatch and relocation (docs/PORT.md "M3", phase 2).
 *
 * The overlay region (0x800E8028..0x80140000, the array port_x000_image + port_x000_bss) holds one of the area
 * overlays X0nn or one of the SYSTEM overlays at a time; the game loads them with its own CD reads.  Every function
 * start inside the region has a dispatch thunk (build/port2/gen/thunks.S, `port_thunk_<addr>`): a call to it pushes
 * the retail address and asks port_resolve_call for the native code of the overlay resident now (recognised by its
 * header id for the SYSTEM set, by two signature words of its own code for the areas), then jumps there with the
 * caller's stack intact.
 *
 * A file the game reads into the region arrives with retail pointers in its data (handler tables, the entry-point
 * table at its start): port_relocate_overlay turns every aligned word outside the overlay's code ranges that
 * holds a RAM address into the native pointer: a MAIN0/X000 function, a thunk, or the byte of the data image. */
#include <stdio.h>
#include <string.h>
#include "port_runtime.h"

void *port_ram_ptr(u32p a) {
    if (0xA0000000u <= a && a < 0xA0200000u) a -= 0x20000000u;
    if (0x80000000u <= a && a < 0x80200000u) return (unsigned char *)port_ram + (a - 0x80000000u);
    if (0x1F800000u <= a && a < 0x1F800400u) return (unsigned char *)port_scratch + (a - 0x1F800000u);
    if (0x1F801000u <= a && a < 0x1F803000u) return (unsigned char *)port_io + (a - 0x1F801000u);
    return 0;
}
/* 1 when p points into RAM or the scratchpad (diagnostics that follow the game's pointers) */
int port_is_ram_ptr(const void *p) {
    const unsigned char *b = p;
    return (b >= (const unsigned char *)port_ram && b < (const unsigned char *)port_ram + 0x200000) ||
           (b >= (const unsigned char *)port_scratch && b < (const unsigned char *)port_scratch + 0x400);
}

/* ------------------------------------------------------------------ which overlay is resident */
static int sysovl_index(u32p id) {
    for (int i = 0; port_ovls[i].name; i++) if (port_ovls[i].id == id) return i;
    return -1;
}
int port_current_area(void) {
    for (int i = 0; i < port_areas_count; i++) {
        const PortArea *a = &port_areas[i];
        if (*(u32p *)port_ram_ptr(a->sig[0][0]) == a->sig[0][1] && *(u32p *)port_ram_ptr(a->sig[1][0]) == a->sig[1][1]) return i;
    }
    return -1;
}
const char *port_resident_overlay(void) {
    int s = sysovl_index(port_x000_image[0]);
    if (s >= 0) return port_ovls[s].name;
    int a = port_current_area();
    return a >= 0 ? port_areas[a].name : "(none)";
}

/* ------------------------------------------------------------------ dispatch */
static const PortFunc *funcmap_find(u32p addr) {
    int lo = 0, hi = port_funcmap_count - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        if (port_funcmap[m].addr == addr) return &port_funcmap[m];
        if (port_funcmap[m].addr < addr) lo = m + 1; else hi = m - 1;
    }
    return 0;
}
static void *areamap_find(int area, u32p addr) {
    int lo = 0, hi = port_areamap_count - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        const PortAreaFunc *e = &port_areamap[m];
        if (e->area == area && e->addr == addr) return e->fn;
        if (e->area < area || (e->area == area && e->addr < addr)) lo = m + 1; else hi = m - 1;
    }
    return 0;
}
static void *ovlmap_find(u32p id, u32p addr) {
    int lo = 0, hi = port_ovlmap_count - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        const PortOvlFunc *e = &port_ovlmap[m];
        if (e->id == id && e->addr == addr) return e->fn;
        if (e->id < id || (e->id == id && e->addr < addr)) lo = m + 1; else hi = m - 1;
    }
    return 0;
}
static int missing_calls;
static u32p missing_addr[64]; static int missing_n;
static int port_missing(void) { return 0; }
static void note_missing(u32p addr, const char *who) {
    missing_calls++;
    for (int i = 0; i < missing_n; i++) if (missing_addr[i] == addr) return;
    if (missing_n < 64) missing_addr[missing_n++] = addr;
    port_printf("[port2] call to %08x: no native code in %s (frame %d)\n", addr, who, port_frame);
}
int port_missing_calls(void) { return missing_calls; }

#define CACHE 1024
static struct { u32p addr, key; void *fn; } cache[CACHE];
void *port_resolve_call(u32p addr) {
    u32p id = port_x000_image[0];
    int s = sysovl_index(id);
    u32p key;
    int area = -1;
    if (s >= 0) key = 0x10000u | id;
    else { area = port_current_area(); key = (u32p)(area + 1); }
    unsigned h = (addr >> 2 ^ key * 2654435761u) & (CACHE - 1);
    if (cache[h].addr == addr && cache[h].key == key && cache[h].fn) return cache[h].fn;
    void *fn = 0;
    if (s >= 0) fn = ovlmap_find(id, addr);
    else {
        if (area >= 0) fn = areamap_find(area, addr);
        if (!fn) { const PortFunc *f = funcmap_find(addr); if (f) fn = f->fn; }   /* X000 code every area shares */
        if (!fn && area < 0) fn = areamap_find(0, addr);
    }
    if (!fn) { note_missing(addr, port_resident_overlay()); fn = (void *)port_missing; }
    else if (port_verbose > 2) port_printf("[port2] dispatch %08x -> %s (%s)\n", addr, port_sym_near(fn, 0), port_resident_overlay());
    cache[h].addr = addr; cache[h].key = key; cache[h].fn = fn;
    return fn;
}

/* ------------------------------------------------------------------ relocation of a loaded overlay */
static const PortAddr *addr_find(const PortAddr *t, int n, u32p addr) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        if (t[m].addr == addr) return &t[m];
        if (t[m].addr < addr) lo = m + 1; else hi = m - 1;
    }
    return 0;
}
static const PortOvlCode *ovlcode_for(void) {
    u32p id = port_x000_image[0];
    if (sysovl_index(id) >= 0) {
        for (int i = 0; i < port_ovlcode_count; i++) if (port_ovlcode[i].kind == 1 && port_ovlcode[i].key == id) return &port_ovlcode[i];
        return 0;
    }
    int a = port_current_area();
    if (a < 0) return 0;
    for (int i = 0; i < port_ovlcode_count; i++) if (port_ovlcode[i].kind == 0 && (int)port_ovlcode[i].key == a) return &port_ovlcode[i];
    return 0;
}
static int in_ranges(const PortOvlCode *c, u32p a) {
    int lo = 0, hi = c->n - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        if (a < c->ranges[2 * m]) hi = m - 1;
        else if (a >= c->ranges[2 * m + 1]) lo = m + 1;
        else return 1;
    }
    return 0;
}
void port_relocate_overlay(void *dst, int bytes) {
    u32p base = 0x80000000u + (u32p)((unsigned char *)dst - (unsigned char *)port_ram);
    int ovl = base >= PORT_OVL_BASE && base < PORT_OVL_END;
    const PortOvlCode *c = ovl ? ovlcode_for() : 0;
    if (ovl && !c) { port_printf("[port2] frame %d: %d bytes read into the overlay region at %08x: unknown overlay (id %08x), not relocated\n",
                          port_frame, bytes, base, port_x000_image[0]); return; }
    u32p *w = dst; int n = bytes / 4, nfn = 0, nth = 0, nd = 0, nraw = 0;
    memset(cache, 0, sizeof cache);
    for (int i = 0; i < n; i++) {
        u32p a = base + 4 * i, v = w[i];
        if (c && in_ranges(c, a)) continue;
        if (0x1F800000u <= v && v < 0x1F800400u) { w[i] = (u32p)port_ram_ptr(v); nd++; continue; }
        if (v < PORT_MAIN0_LOAD || v >= 0x80200000u || (v & 3)) continue;
        if (c && c->kind == 1 && v >= PORT_OVL_BASE && v < PORT_OVL_END) {
            /* a SYSTEM overlay's own word pointing into the overlay region: one of its functions (never an X000
               function or a thunk: X000 is not resident), a label inside one, or its own data (D0SPSUB's icon
               table at 0x800FE89C holds 0x800FE13C, which is also an X000 function start) */
            void *fn = ovlmap_find(port_x000_image[0], v);
            if (fn) { w[i] = (u32p)fn; nfn++; continue; }
            if (in_ranges(c, v)) { nraw++; continue; }
            w[i] = (u32p)port_ram_ptr(v); nd++; continue;
        }
        const PortAddr *e = addr_find(port_addrmap, port_addrmap_count, v);
        if (e) { w[i] = (u32p)e->fn; nfn++; continue; }
        e = addr_find(port_thunkmap, port_thunkmap_count, v);
        if (e) { w[i] = (u32p)e->fn; nth++; continue; }
        if (c && in_ranges(c, v)) { nraw++; continue; }   /* a label inside a function of this overlay: left as is */
        w[i] = (u32p)port_ram_ptr(v); nd++;
    }
    if (port_verbose || ovl)
        port_printf("[port2] frame %d: %s loaded (%d bytes at %08x): %d function, %d thunk, %d data pointers relocated, %d code labels kept\n",
                    port_frame, c ? c->name : "a file below the heap", bytes, base, nfn, nth, nd, nraw);
}

/* nearest native function at or below fn (the generated tables) */
const char *port_sym_near(void *fn, unsigned long *off) {
    unsigned long a = (unsigned long)fn, best = 0; const char *name = 0;
    for (int i = 0; i < port_funcmap_count; i++) {
        unsigned long f = (unsigned long)port_funcmap[i].fn;
        if (f <= a && f > best) { best = f; name = port_funcmap[i].name; }
    }
    for (int i = 0; i < port_ovlmap_count; i++) {
        unsigned long f = (unsigned long)port_ovlmap[i].fn;
        if (f <= a && f > best) { best = f; name = port_ovlmap[i].name; }
    }
    for (int i = 0; i < port_areamap_count; i++) {
        unsigned long f = (unsigned long)port_areamap[i].fn;
        if (f <= a && f > best) { best = f; name = port_areas[port_areamap[i].area].name; }
    }
    if (off) *off = a - best;
    return name ? name : "?";
}
