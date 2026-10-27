/* The game's language (--lang N, docs/PORT.md "Languages").
 *
 * The disc boots one of EXE/MAIN0..8.EXE: the same program linked once per language with that language's text
 * data, and each MAINn carries its own file table (SYS/LDn*, X?NN, O?PTSUB0, D?SPSUB*, I?NFO*, G?OVER, the 100_?
 * movies).  The port's C is MAIN0's, and its data image and data names are MAIN0 / X0 offsets.  With --lang N the
 * port keeps that layout and brings the language in as data:
 *
 *   1. MAINn.EXE is read from the disc / data folder and ALIGNED with MAIN0.EXE (align2(): a monotonic map of
 *      equal 32-byte windows compared on the high halves of their words, so relinked pointers and immediates do
 *      not break a match; a word with a zero high half, a small number, must match whole; a chunk starts at an
 *      anchor window with some information, confirmed over 256 bytes (64 when the delta is one the first pass
 *      found) and extended backwards; the parts in between, the text tables that changed size, are "gaps").  Every
 *      word of MAIN0's data region whose MAINn counterpart differs by more than the relink is replaced: the file
 *      table (so the language's files are loaded, the movies among them), the message box sizes, the title
 *      lists...  Pointers in MAINn's data are translated back to MAIN0 addresses with the map (then to native
 *      pointers as the relocator does).  A gap of the same size on both sides is copied in place; one that
 *      changed size is copied only up to the first pointer on either side and only when no pointer of MAINn
 *      points into it (a string table read at fixed offsets: O?PTSUB0's title menu), otherwise MAIN0's bytes
 *      stay and the language's live in the "spill", a copy pointers are translated to (the event-title layout
 *      lists, read through the per-event pointer table).
 *   2. Every overlay the game reads into the overlay region (its CdRead is the language's X5NN...) is composed
 *      over its MAIN0 twin, read from the disc with MAIN0's table entry of the same file id: the twin's code (the
 *      port never runs it, but the dispatch signatures and the "code labels" of the relocator are its words) and
 *      its data layout, with the language's data words copied in at the aligned offsets and its pointers
 *      translated, the same gap rules, a spill per load (a ring of RING).  The data names the C uses
 *      (port_x000_image + offset, D0SPSUB's ITEM_NAME_STRINGS...) therefore keep their MAIN0 offsets.
 *   3. Overlays are linked at a base that follows MAINn's bss, 8-aligned (+0x28 for MAIN5, +0x30 for MAIN6), so
 *      every overlay-region address in the language's data is first moved by that delta (ovl_delta, from the
 *      alignment, checked against the overlay's own entry table); a bss item of either image is kept at MAIN0's
 *      address (pairs of bss pointers vote for the bss delta the lone ones use).
 *   4. What the C reads at fixed addresses but changed size in MAINn gets its own treatment: printInfoMessage's
 *      box width constants (port_lang_box_pad, code constants per MAINn) and spawnEventTitle's tile tables
 *      (port_title_*, found in MAINn by their anchor).
 *
 * Limits: a data item that changed size, is pointed to AND is read by the C at a fixed offset shows MAIN0's
 * version; a chunk matched at a wrong delta would copy the wrong words (the window rules above are what keeps
 * that from happening on the nine executables).  Diagnosis: PORT_LANG_ONLY_LINKS=1 replaces the file table only,
 * PORT_LANG_RAW_OVL=1 leaves the language's overlays as read (their words translated by the relocator); -v prints
 * the gaps, -v -v the translated pointers. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"

int port_lang;                                   /* 0: MAIN0, no remapping */
/* printInfoMessage's box width additions by message kind b (0, 1, 4; 2/3 are 0x10 in every MAINn): the addiu
   constants of the switch at 0x8004D7F8 in each MAINn (MAIN5 shares one case for b = 1 and 4) */
int port_lang_box_pad[5] = { 0x60, 0x58, 0x10, 0x10, 0x78 };
static const int box_pads[9][3] = {
    { 0x60, 0x58, 0x78 }, { 0x48, 0x50, 0x60 }, { 0x58, 0x50, 0x70 }, { 0x40, 0x40, 0x50 }, { 0x68, 0x58, 0x30 },
    { 0x48, 0x58, 0x58 }, { 0x38, 0x48, 0x60 }, { 0x38, 0x50, 0x60 }, { 0x40, 0x48, 0x60 },
};
const char *port_lang_name(int n) {
    static const char *names[9] = { "English", "Francais", "Deutsch", "Italiano", "Nederlands", "Espanol", "Portugues", "Svenska", "Dansk" };
    return n >= 0 && n < 9 ? names[n] : "?";
}

/* ------------------------------------------------------------------ alignment of two images */
typedef struct { unsigned a, b, len; } Chunk;                       /* A[a..a+len) == B[b..b+len) (high halves) */
typedef struct { unsigned a, alen, b, blen, spill; int refs; } Gap; /* between two chunks: A-only / B-only bytes; refs: B pointers into it */
struct Map {
    const unsigned char *A, *B; unsigned na, nb;
    Chunk *c; int nc, ccap;
    Gap *g; int ng;
    unsigned char *spill; unsigned nspill;
    unsigned char *ownA, *ownB;      /* buffers freed with the map */
    unsigned char *Bn;               /* a copy of B with its pointer words made native: where the pointers to items that span a changed-size gap go */
    unsigned *tgt; int ntgt;         /* the sorted B offsets B's own pointers (and MAIN's, for an overlay) point to */
    int id;                          /* an overlay map: the FILE_LINKS id (the ring is looked up by it) */
    int relocated;                   /* the spill and Bn hold native pointers (once per map: a reused map is not done again) */
    int bss_delta;                   /* B offset - A offset of the trailing chunk: what follows the image moved by this */
    int bss_seen; int bss_adj;       /* learned from pointer pairs of both images into their bss: B - A (bss_delta until seen) */
    int votes[8][2], nvotes;         /* (delta, count) of those pairs: the most frequent wins when the compose is done */
    int deltas[64], ndeltas;         /* the distinct chunk deltas: a word pair differing by one of them is a relinked reference */
    u32p base_a, base_b;             /* where A and B are loaded (B: the language's link address) */
    const struct Map *other;         /* the MAIN map while an overlay is aligned (pointers into MAIN) */
};
typedef struct Map Map;

#define W(p, o) (*(const unsigned *)((p) + (o)))
#define CONFIRM 256                      /* a new delta must hold for this many bytes */
static int is_ram_word(unsigned v) { return (v >= PORT_MAIN0_LOAD && v < 0x80200000u && !(v & 3)) || (v >= 0x1F800000u && v < 0x1F800400u); }
#define is_ram_addr is_ram_word
/* two words match when their high halves do: a relinked pointer or a changed box width still matches; a word
   with a zero high half (a small number) must match whole, or a table of small numbers that only moved by a few
   entries would "match" at the wrong delta */
struct Map;
static int ptr_pair_ok(const struct Map *m, unsigned wa, unsigned wb);
static int is_delta(const struct Map *m, int d);
static const struct Map *eq_ctx;            /* the second pass: pointer words must agree with the first pass's deltas */
static int eq1(const unsigned char *x, const unsigned char *y) {
    unsigned a = W(x, 0), b = W(y, 0);
    if ((a ^ b) & 0xFFFF0000u) return 0;
    if (!(a & 0xFFFF0000u) && a != b) return 0;
    if (eq_ctx && a != b && is_ram_word(a) && !ptr_pair_ok(eq_ctx, a, b)) return 0;
    return 1;
}
static int eq8(const unsigned char *x, const unsigned char *y) {
    for (int i = 0; i < 32; i += 4) {
        unsigned a = W(x, i), b = W(y, i);
        if ((a ^ b) & 0xFFFF0000u) return 0;
        if (!(a & 0xFFFF0000u) && a != b) return 0;
        if (eq_ctx && a != b && is_ram_word(a) && !ptr_pair_ok(eq_ctx, a, b)) return 0;
    }
    return 1;
}
static int distinct8(const unsigned char *x) {      /* windows with fewer than 3 distinct high halves are not anchors */
    unsigned seen[8]; int n = 0;
    for (int i = 0; i < 32; i += 4) {
        unsigned h = W(x, i) >> 16; int k;
        for (k = 0; k < n; k++) if (seen[k] == h) break;
        if (k == n) seen[n++] = h;
    }
    return n >= 3;
}
static unsigned hash8(const unsigned char *x) {
    unsigned h = 2166136261u;
    for (int i = 0; i < 32; i += 4) { h ^= W(x, i) >> 16; h *= 16777619u; }
    return h;
}
static void push_chunk(Map *m, unsigned a, unsigned b, unsigned len) {
    if (m->nc) {    /* the previous chunk closed 28 bytes past its last window start: it may overlap this one */
        Chunk *p = &m->c[m->nc - 1];
        if (p->a + p->len > a) { if (a <= p->a) m->nc--; else p->len = a - p->a; }
    }
    if (m->nc == m->ccap) { m->ccap = m->ccap ? m->ccap * 2 : 256; m->c = realloc(m->c, m->ccap * sizeof *m->c); }
    m->c[m->nc].a = a; m->c[m->nc].b = b; m->c[m->nc].len = len; m->nc++;
}
/* A and B 4-aligned buffers.  Windows of 32 bytes; a chunk is a run of windows matching at one delta; a new delta
   is looked up in a hash table of B's windows, nearest to the current delta first, never going back in B. */
static void align(Map *m) {
    const unsigned char *A = m->A, *B = m->B; unsigned na = m->na, nb = m->nb;
    unsigned nbw = nb >= 32 ? (nb - 32) / 4 + 1 : 0, tsize = 1;
    while (tsize < nbw * 2 + 16) tsize <<= 1;
    unsigned *tab = calloc(tsize, sizeof *tab);
    for (unsigned j = 0; j + 32 <= nb; j += 4) {
        if (!distinct8(B + j)) continue;
        unsigned h = hash8(B + j) & (tsize - 1);
        while (tab[h]) h = (h + 1) & (tsize - 1);
        tab[h] = j + 1;
    }
    int curd = 0; unsigned minB = 0;           /* B = A + curd; the next chunk starts at or after minB in B */
    unsigned cs = 0, cb = 0, ce = 0; int open = 0;
    for (unsigned i = 0; i + 32 <= na; i += 4) {
        if (open) {     /* extend the chunk while the windows match at its delta */
            long j = (long)i + curd;
            if (j >= (long)minB && j + 32 <= (long)nb && eq8(A + i, B + j)) { ce = i + 32; continue; }
            push_chunk(m, cs, cb, ce - cs); minB = cb + (ce - cs); open = 0;
        }
        /* a chunk starts at an anchor only (also after a gap, at the same delta): a window with information,
           confirmed over CONFIRM bytes and extended backwards; a pointer table (one high half everywhere) after
           a text table that changed size is reached by the backward extension from the anchor after it, never
           matched at the delta of the text before it */
        if (!distinct8(A + i)) continue;
        /* a new delta: the candidate nearest to the current one, confirmed by the window after it */
        unsigned h = hash8(A + i) & (tsize - 1); long best = -1; long bestd = 0; int probes = 0;
        while (tab[h] && probes++ < 512) {
            unsigned jj = tab[h] - 1;
            if (jj >= minB && eq8(A + i, B + jj)) {
                long d = (long)jj - (long)i, dd = d - curd; if (dd < 0) dd = -dd;
                long bd = bestd - curd; if (bd < 0) bd = -bd;
                if (best < 0 || dd < bd) { best = jj; bestd = d; }
            }
            h = (h + 1) & (tsize - 1);
        }
        if (best >= 0) {    /* confirmed over CONFIRM bytes (the 4-byte records of two languages' title layouts match by chance for a
                               list or two), or over 64 when the second pass knows the delta from the first (a short table between
                               two that changed) */
            int ok = 1; unsigned need = eq_ctx && is_delta(eq_ctx, (int)bestd) ? 64 : CONFIRM;
            for (unsigned k = 4; k <= need - 32 && ok; k += 4) ok = i + k + 32 <= na && (unsigned)best + k + 32 <= nb && eq8(A + i + k, B + best + k);
            if (ok) {
                curd = (int)bestd; cs = i; cb = (unsigned)best; ce = i + 32; open = 1;
                /* extend backwards word by word: a pointer table (one high half everywhere, no anchor in it)
                   right before the anchor belongs to this delta too */
                unsigned pa = m->nc ? m->c[m->nc - 1].a + m->c[m->nc - 1].len : 0, pb = minB;
                while (cs >= pa + 4 && cb >= pb + 4 && eq1(A + cs - 4, B + cb - 4)) { cs -= 4; cb -= 4; }
            }
        }
    }
    if (open) push_chunk(m, cs, cb, ce - cs);
    free(tab);
    /* the gaps (leading, between chunks, trailing) and the spill of the B-only bytes */
    m->g = calloc(m->nc + 1, sizeof *m->g); m->ng = 0; m->nspill = 0;
    unsigned pa = 0, pb = 0;
    for (int k = 0; k <= m->nc; k++) {
        unsigned a1 = k < m->nc ? m->c[k].a : na, b1 = k < m->nc ? m->c[k].b : nb;
        if (b1 < pb) b1 = pb;
        if (a1 > pa || b1 > pb) {
            Gap *g = &m->g[m->ng++];
            g->a = pa; g->alen = a1 - pa; g->b = pb; g->blen = b1 - pb; g->spill = m->nspill;
            m->nspill += (g->blen + 3) & ~3u;
        }
        if (k < m->nc) { pa = m->c[k].a + m->c[k].len; pb = m->c[k].b + m->c[k].len; }
    }
    m->spill = calloc(m->nspill + 4, 1);
    for (int k = 0; k < m->ng; k++) if (m->g[k].blen) memcpy(m->spill + m->g[k].spill, B + m->g[k].b, m->g[k].blen);
    /* which gaps B's own pointers reach: their consumers follow pointers (to the spill), so a copy of the gap's
       head at A's offsets would only mislead the pointers A keeps there */
    for (unsigned o = 0; o + 4 <= nb; o += 4) {
        unsigned v = W(B, o);
        if (v < m->base_b || v >= m->base_b + nb) continue;
        unsigned ob = v - m->base_b;
        for (int k = 0; k < m->ng; k++) if (ob >= m->g[k].b && ob < m->g[k].b + m->g[k].blen && m->g[k].blen != m->g[k].alen) { m->g[k].refs++; break; }
    }
    m->bss_delta = m->nc ? (int)(m->c[m->nc - 1].b + m->c[m->nc - 1].len) - (int)(m->c[m->nc - 1].a + m->c[m->nc - 1].len) : 0;
    m->bss_adj = (m->bss_delta + 7) & ~7; m->bss_seen = 0; m->nvotes = 0;
    m->ndeltas = 0;
    for (int k = 0; k < m->nc; k++) {
        int d = (int)m->c[k].b - (int)m->c[k].a, j;
        for (j = 0; j < m->ndeltas; j++) if (m->deltas[j] == d) break;
        if (j == m->ndeltas && m->ndeltas < 64) m->deltas[m->ndeltas++] = d;
    }
}
static int is_delta(const Map *m, int d) {
    for (int j = 0; j < m->ndeltas; j++) if (m->deltas[j] == d) return 1;
    return d == m->bss_adj || d == m->bss_delta;
}
/* A's pointer wa and B's wb name the same thing when they differ by a delta of the first pass (a chunk, the bss),
   or both point into the same gap (a table whose inside was rearranged, the title layout lists) */
static int ptr_pair_ok(const Map *m, unsigned wa, unsigned wb) {
    if (!is_ram_word(wb)) return 0;
    int d = (int)(wb - wa);
    if (wa >= m->base_a && wa < m->base_a + m->na + 0x10000 && wb >= m->base_b) {
        if (is_delta(m, d - (int)(m->base_b - m->base_a))) return 1;
        for (int k = 0; k < m->ng; k++) {
            const Gap *g = &m->g[k];
            if (wa - m->base_a >= g->a && wa - m->base_a < g->a + g->alen && wb - m->base_b >= g->b && wb - m->base_b < g->b + g->blen) return 1;
        }
        return 0;
    }
    if (m->other) return ptr_pair_ok(m->other, wa, wb);
    return is_delta(m, d);
}
/* a pointer pair (A's word, B's word) both beyond their images: B's bss is not simply the image end moved (the
   linker aligns it), so the delta is learned from such pairs and B's lone bss pointers follow it */
static int bss_pair(Map *m, u32p base_a, u32p base_b, u32p wa, u32p wb) {
    if (wa < base_a + m->na || wb < base_b + m->nb || wa >= PORT_OVL_END || wb >= PORT_OVL_END) return 0;
    int adj = (int)(wb - base_b) - (int)(wa - base_a), j;
    for (j = 0; j < m->nvotes; j++) if (m->votes[j][0] == adj) break;
    if (j == m->nvotes && j < 8) { m->votes[j][0] = adj; m->votes[j][1] = 0; m->nvotes++; }
    if (j < m->nvotes) m->votes[j][1]++;
    if (!m->bss_seen) { m->bss_adj = adj; m->bss_seen = 1; }
    return 1;
}
static void bss_decide(Map *m, const char *what) {
    int best = -1;
    for (int j = 0; j < m->nvotes; j++) if (best < 0 || m->votes[j][1] > m->votes[best][1]) best = j;
    if (best >= 0) m->bss_adj = m->votes[best][0];
    if (port_verbose && m->nvotes > 1) {
        port_printf("[port2]   %s bss: ", what);
        for (int j = 0; j < m->nvotes; j++) port_printf("%+d x%d ", m->votes[j][0], m->votes[j][1]);
        port_printf("-> %+d\n", m->bss_adj);
    }
}
/* two passes: the second one knows the deltas and gaps of the first, so a pointer table whose entries all share
   a high half (identical "empty" entries, entries into a rearranged table) is not matched at a wrong delta */
static void align2(Map *m) {
    align(m);
    Map first = *m;
    first.c = malloc(m->nc * sizeof *m->c); memcpy(first.c, m->c, m->nc * sizeof *m->c);
    first.g = malloc((m->ng + 1) * sizeof *m->g); memcpy(first.g, m->g, m->ng * sizeof *m->g);
    free(m->c); free(m->g); free(m->spill); m->c = 0; m->g = 0; m->spill = 0; m->nc = m->ccap = m->ng = 0;
    eq_ctx = &first;
    align(m);
    eq_ctx = 0;
    free(first.c); free(first.g);
}
static void map_free(Map *m) { free(m->c); free(m->g); free(m->spill); free(m->ownA); free(m->ownB); free(m->Bn); free(m->tgt); memset(m, 0, sizeof *m); }

/* B offset -> A offset (>= 0), or -1 with *spill set (a B-only byte), or -2 (beyond B) */
static long b2a(const Map *m, unsigned ob, unsigned char **spill) {
    *spill = 0;
    if (ob >= m->nb && m->nc) return -2;
    int lo = 0, hi = m->nc - 1;
    while (lo <= hi) {
        int k = (lo + hi) / 2; const Chunk *c = &m->c[k];
        if (ob < c->b) hi = k - 1; else if (ob >= c->b + c->len) lo = k + 1; else return (long)c->a + (ob - c->b);
    }
    for (int k = 0; k < m->ng; k++) if (ob >= m->g[k].b && ob < m->g[k].b + m->g[k].blen) {
        if (m->g[k].alen == m->g[k].blen) return (long)m->g[k].a + (ob - m->g[k].b);   /* copied in place: one copy, the game may write to it */
        *spill = m->spill + m->g[k].spill + (ob - m->g[k].b); return -1;
    }
    return -2;
}
/* A offset -> the B offset holding its language version: the chunk, the same offset of a gap of the same size in
   both (text that changed without moving anything), or the head of a gap that changed size as long as neither
   side holds a pointer there (a string table at the head of the gap; past the first pointer the structures no
   longer line up and A keeps its words: B's are reachable through the spill).  -1: none. */
static long a2b(const Map *m, unsigned oa) {
    int lo = 0, hi = m->nc - 1;
    while (lo <= hi) {
        int k = (lo + hi) / 2; const Chunk *c = &m->c[k];
        if (oa < c->a) hi = k - 1; else if (oa >= c->a + c->len) lo = k + 1; else return (long)c->b + (oa - c->a);
    }
    for (int k = 0; k < m->ng; k++) {
        const Gap *g = &m->g[k];
        if (oa < g->a || oa >= g->a + g->alen) continue;
        if (g->alen == g->blen) return (long)g->b + (oa - g->a);
        if (oa - g->a >= g->blen || g->refs) return -1;
        for (unsigned o = 0; o <= oa - g->a; o += 4)
            if (is_ram_word(W(m->A, g->a + o)) || is_ram_word(W(m->B, g->b + o))) return -1;
        return (long)g->b + (oa - g->a);
    }
    return -1;
}

/* ------------------------------------------------------------------ the maps in use */
static Map main_map;                      /* MAIN0.EXE (A) vs MAINn.EXE (B), both without the 0x800 header */
static int ovl_delta;                     /* the overlay region of MAINn starts this much higher */
static const unsigned char *main0_links;  /* MAIN0's file table (1119 x 8 bytes) in the MAIN0.EXE buffer */
#define LINKS_OFF 0x6DEF0u                /* FILE_LINKS 0x8007DEF0 - 0x80010000 */
#define LINKS_N 1119
#define DATA_OFF 0x674F0u                 /* MAIN0's .data: 0x800774F0 */
#define RING 4
static struct { Map m; unsigned base_b; } ovl_ring[RING];   /* the overlay maps of the last RING loads */
static int ovl_cur = -1, ovl_loads;
static int ovl_composed_pending;

typedef struct { u32p addr; unsigned char *native; } Xlat;
/* ---- items that span a changed-size gap.  A pointer into a chunk names an item whose bytes line up with
   MAIN0's, so it gets MAIN0's layout (the live image: the C names it, the game writes to it).  But an item that
   is read through its own internal offsets, a message bank (D0SPSUB3's WFM3 at 0x800EF600: a header, an offset
   table, the texts) or a string table, has no fixed size: when the language's version is longer, the alignment
   puts the extra bytes in gaps, the composed image holds MAIN0's bytes less those gaps while the offsets copied
   in are the language's, and the consumer reads garbage (the "EEEEE" message box that overran the glyph tables
   and zeroed the overlay region).  So a pointer whose item, up to the next pointer target in B, crosses a gap
   that changed size resolves into Bn, a copy of the language's image whose own pointer words were made native:
   the item is whole there, with the language's offsets. */
static int cmp_u(const void *x, const void *y) { unsigned a = *(const unsigned *)x, b = *(const unsigned *)y; return a < b ? -1 : a > b; }
static void add_targets(Map *m, const unsigned char *P, unsigned np, unsigned lo, unsigned hi, int *cap) {
    for (unsigned o = 0; o + 4 <= np; o += 4) {
        unsigned v = W(P, o);
        if (v < lo || v >= hi || (v & 3)) continue;
        if (m->ntgt == *cap) { *cap = *cap ? *cap * 2 : 1024; m->tgt = realloc(m->tgt, *cap * sizeof *m->tgt); }
        m->tgt[m->ntgt++] = v - lo;
    }
}
static void targets(Map *m) {
    int cap = 0; m->ntgt = 0;
    add_targets(m, m->B, m->nb, m->base_b, m->base_b + m->nb, &cap);
    if (m->other && m->other->B) add_targets(m, m->other->B, m->other->nb, m->base_b, m->base_b + m->nb, &cap);
    if (m->ntgt) qsort(m->tgt, m->ntgt, sizeof *m->tgt, cmp_u);
}
static int item_spills(const Map *m, unsigned ob, unsigned oa) {
    if (m == &main_map) { if (oa < DATA_OFF) return 0; }
    else { const PortOvlCode *c = port_ovl_code_ranges(); if (c && port_ovl_in_ranges(c, PORT_OVL_BASE + oa)) return 0; }
    unsigned next = m->nb;
    int lo = 0, hi = m->ntgt - 1;
    while (lo <= hi) { int k = (lo + hi) / 2; if (m->tgt[k] <= ob) lo = k + 1; else { next = m->tgt[k]; hi = k - 1; } }
    for (int k = 0; k < m->ng; k++) {
        const Gap *g = &m->g[k];
        if (g->alen != g->blen && g->b > ob && g->b < next) return 1;
    }
    return 0;
}
static unsigned char *bn_get(Map *m) {
    if (!m->Bn) { m->Bn = malloc(m->nb + 4); memcpy(m->Bn, m->B, m->nb); }
    return m->Bn;
}
/* a MAINn / language-overlay address -> the MAIN0 / X0 layout (addr), or a native pointer into a spill */
static Xlat xlate(u32p v) {
    Xlat x = { v, 0 };
    if (!port_lang) return x;
    if (v >= PORT_MAIN0_LOAD && v < PORT_MAIN0_LOAD + main_map.nb) {
        long a = b2a(&main_map, v - PORT_MAIN0_LOAD, &x.native);
        if (a >= 0) { x.addr = PORT_MAIN0_LOAD + (u32p)a; if (item_spills(&main_map, v - PORT_MAIN0_LOAD, (unsigned)a)) x.native = bn_get(&main_map) + (v - PORT_MAIN0_LOAD); }
        else if (a == -2) x.addr = v - main_map.bss_adj;
        return x;
    }
    if (v >= PORT_MAIN0_LOAD && v < PORT_OVL_BASE + ovl_delta) { x.addr = v - main_map.bss_adj; return x; }
    if (v >= PORT_OVL_BASE + ovl_delta && v < PORT_OVL_END) {
        const Map *m = ovl_cur >= 0 ? &ovl_ring[ovl_cur].m : 0;
        u32p base_b = PORT_OVL_BASE + ovl_delta;
        if (m && m->nc && v - base_b < m->nb) {
            long a = b2a(m, v - base_b, &x.native);
            if (a >= 0) { x.addr = PORT_OVL_BASE + (u32p)a; if (item_spills(m, v - base_b, (unsigned)a)) x.native = bn_get((Map *)m) + (v - base_b); }
            return x;
        }
        x.addr = v - ovl_delta - (m && m->nc ? m->bss_adj : 0);
        return x;
    }
    return x;
}
/* what port_relocate_overlay would make of a MAIN0-layout address */
static unsigned char *native_of(u32p a) {
    const PortAddr *t = port_addrmap; int lo = 0, hi = port_addrmap_count - 1;
    while (lo <= hi) { int k = (lo + hi) / 2; if (t[k].addr == a) return t[k].fn; if (t[k].addr < a) lo = k + 1; else hi = k - 1; }
    t = port_thunkmap; lo = 0; hi = port_thunkmap_count - 1;
    while (lo <= hi) { int k = (lo + hi) / 2; if (t[k].addr == a) return t[k].fn; if (t[k].addr < a) lo = k + 1; else hi = k - 1; }
    return port_ram_ptr(a);
}
/* for the relocator: a word of a file read with the language's table (a load list) in MAIN0 terms; returns 1
   with *native set when the word already is the final pointer (a spill) */
int port_lang_xlate_word(u32p *v, void **native) {
    if (!port_lang) return 0;
    Xlat x = xlate(*v);
    if (x.native) { *native = x.native; return 1; }
    *v = x.addr;
    return 0;
}
extern unsigned port_wfm3_len(const unsigned char *p, unsigned n);
#define wfm3_len port_wfm3_len
static void relocate_words(unsigned char *p, unsigned n) {
    for (unsigned o = 0; o + 4 <= n; o += 4) {
        unsigned bank = wfm3_len(p + o, n - o);          /* a message bank: glyph codes, never pointers (ovl.c) */
        if (bank) { o += bank - 4; continue; }
        unsigned v = W(p, o);
        if (!is_ram_addr(v)) continue;
        Xlat x = xlate(v);
        *(unsigned *)(p + o) = x.native ? (unsigned)(unsigned long)x.native : (unsigned)(unsigned long)native_of(x.addr);
    }
}
/* after the compose (bss_adj decided): the spill's words, and Bn's (every map gets one: an item pointer handed
   out before this point already points into it) */
static void relocate_spill(Map *m) {
    if (m->relocated) return;
    m->relocated = 1;
    relocate_words(m->spill, m->nspill);
    relocate_words(bn_get(m), m->nb);
}


/* ---- named blobs read through a fixed address.  D0SPSUB3's item-message bank is a WFM3 file inside the overlay
   (0x800EF600: the C takes its address, D_1F8003A0 = D_800EF600, and reads it through its own offset table), so
   no pointer rule can send the consumer elsewhere: the language's bank must be whole at MAIN0's offset.  A WFM3
   header carries its length (+8), so the language's bank is copied there as a unit; when it is longer, the bytes
   it grows into must be a gap of the twin (MAIN0-only or changed bytes nothing points to), never a chunk. */
static int in_chunk(const Map *m, unsigned oa) {
    for (int k = 0; k < m->nc; k++) if (oa >= m->c[k].a && oa < m->c[k].a + m->c[k].len) return 1;
    return 0;
}
static int named_blobs(const Map *m, unsigned char *img, unsigned size0, unsigned from) {
    int n = 0;
    for (unsigned oa = from; oa + 12 <= m->na; oa += 4) {
        if (memcmp(m->A + oa, "WFM3", 4)) continue;
        long ob = a2b(m, oa);
        if (ob < 0 || (unsigned)ob + 12 > m->nb || memcmp(m->B + ob, "WFM3", 4)) continue;
        unsigned la = wfm3_len(m->A + oa, m->na - oa), lb = wfm3_len(m->B + ob, m->nb - (unsigned)ob);
        if (!la || !lb || oa + lb > size0) continue;
        int ok = 1;
        for (unsigned o = oa + la; o < oa + lb; o += 4) if (in_chunk(m, o)) { ok = 0; break; }
        if (!ok) { port_printf("[port2]   --lang: WFM3 bank at %08x: the language's (%u bytes, MAIN0's %u) would overrun a matched chunk: left composed\n", m->base_a + oa, lb, la); continue; }
        memcpy(img + oa, m->B + ob, lb);
        if (port_verbose) port_printf("[port2]   --lang: WFM3 bank at %08x: the language's %u bytes in place (MAIN0's %u)\n", m->base_a + oa, lb, la);
        n++;
    }
    return n;
}

/* A C-named item read through its name (a fixed MAIN0 offset) whose language version changed size: the C asks
   for the language's copy with port_lang_item(name) (port/fixes.txt adds the call: O0PTSUB0's TITLE_MENU_GLYPHS,
   the -3-ended glyph list of the title carousel, 80 bytes in MAIN0 and 96 in MAIN5, truncated in place).  The
   language's item is whole in Bn; an item that did not change stays where it is. */
void *port_lang_item(void *p) {
    if (!port_lang) return p;
    const unsigned char *b = p; Map *m; unsigned oa;
    if (b >= (const unsigned char *)port_main0_image && b < (const unsigned char *)port_main0_image + main_map.na) { m = &main_map; oa = b - (const unsigned char *)port_main0_image; }
    else if (b >= (const unsigned char *)port_x000_image && b < (const unsigned char *)port_x000_image + (PORT_OVL_END - PORT_OVL_BASE) && ovl_cur >= 0 && ovl_ring[ovl_cur].m.nc) {
        m = &ovl_ring[ovl_cur].m; oa = b - (const unsigned char *)port_x000_image; if (oa >= m->na) return p;
    } else return p;
    for (int k = 0; k < m->ng; k++) {
        const Gap *g = &m->g[k];
        if (oa >= g->a && oa < g->a + g->alen && g->alen != g->blen) return oa - g->a < g->blen ? bn_get(m) + g->b + (oa - g->a) : p;
    }
    long ob = a2b(m, oa);
    if (ob >= 0 && item_spills(m, (unsigned)ob, oa)) return bn_get(m) + ob;
    return p;
}

/* ------------------------------------------------------------------ MAIN: file table and resident data */
/* The event-title tile tables of spawnEventTitle (D_800804E0: u16 tile kind per layout flag, D_800804EC: s16 x
   offset per flag, 6 entries in MAIN0, 11 in MAIN5, 9 in MAIN6, 5 in MAIN7): the language's lists use flags up to
   its own count, and the tables sit at other addresses with other sizes, so the C takes these copies when set.
   Found in MAINn after the anchor 00 00 B4 00 00 00 00 00 (D_800804D8, notes/formats.md): the u16 list, padded to
   4, then the x offsets, one u32 each, -20 * m .. +20 * m' in steps of 20. */
int port_title_k;
unsigned short port_title_e0[16];
struct { unsigned short unk0, unk2; } port_title_ec[16];
static void title_tables(const unsigned char *B, unsigned nb) {
    static const unsigned char anchor[8] = { 0, 0, 0xB4, 0, 0, 0, 0, 0 };
    unsigned at = 0;
    for (unsigned o = DATA_OFF; o + 8 <= nb; o += 4) if (!memcmp(B + o, anchor, 8)) { at = o; break; }
    if (!at) { port_printf("[port2] --lang: the event-title tables of MAIN%d were not found: MAIN0's are used\n", port_lang); return; }
    unsigned e0 = at + 8, ec = e0;
    while (ec + 4 <= nb && !((W(B, ec) & 0xFFFF) >= 0x8000 && !(W(B, ec) >> 16))) ec += 4;   /* the first negative x offset */
    int k = 0; int prev = 0;
    for (unsigned o = ec; o + 4 <= nb && k < 16; o += 4, k++) {
        unsigned w = W(B, o); int x = (short)(w & 0xFFFF);
        if (w >> 16 || x % 20 || (k && x != prev + 20)) break;
        prev = x;
    }
    if (k < 2 || ec - e0 < 2u * k || ec - e0 > 2u * k + 2) { port_printf("[port2] --lang: unexpected event-title tables in MAIN%d (%d entries, %u bytes): MAIN0's are used\n", port_lang, k, ec - e0); return; }
    for (int i = 0; i < k; i++) { port_title_e0[i] = B[e0 + 2 * i] | B[e0 + 2 * i + 1] << 8; port_title_ec[i].unk0 = W(B, ec + 4 * i) & 0xFFFF; port_title_ec[i].unk2 = W(B, ec + 4 * i) >> 16; }
    port_title_k = k;
    if (port_verbose) port_printf("[port2]   event-title tables of MAIN%d at %08x: %d entries\n", port_lang, PORT_MAIN0_LOAD + at, k);
}
extern unsigned char *port_cd_read_named(const char *path, unsigned *size);
extern unsigned char FILE_LINKS[];
void port_lang_init(void) {
    if (!port_lang) return;
    char name[32]; unsigned n0, nn;
    unsigned char *m0 = port_cd_read_named("EXE/MAIN0.EXE", &n0);
    snprintf(name, sizeof name, "EXE/MAIN%d.EXE", port_lang);
    unsigned char *mn = port_cd_read_named(name, &nn);
    if (!m0 || !mn || n0 < 0x800 + LINKS_OFF + LINKS_N * 8 || nn < 0x1000 || memcmp(mn, "PS-X EXE", 8))
        port_fatal("--lang %d: cannot read EXE/MAIN0.EXE and %s from the disc / data folder", port_lang, name);
    main_map.A = m0 + 0x800; main_map.na = n0 - 0x800; main_map.B = mn + 0x800; main_map.nb = nn - 0x800;
    main_map.base_a = main_map.base_b = PORT_MAIN0_LOAD; main_map.other = 0; main_map.ownB = mn;
    align2(&main_map);
    targets(&main_map);
    ovl_delta = (main_map.bss_delta + 7) & ~7;        /* the overlay region starts 8-aligned after MAINn's bss (X?00 entry tables: +0x28 for MAIN5, +0x30 for MAIN6) */
    main0_links = m0 + 0x800 + LINKS_OFF;
    for (int i = 0; i < 5; i++) port_lang_box_pad[i] = i == 2 || i == 3 ? 0x10 : box_pads[port_lang][i == 4 ? 2 : i];
    /* MAIN0's data region: every word whose MAINn version differs by more than the relink */
    unsigned char *img = (unsigned char *)port_main0_image;
    int changed = 0, ptrs = 0, spilled = 0, links = 0;
    int only_links = getenv("PORT_LANG_ONLY_LINKS") != 0;     /* diagnosis: MAINn's file table alone */
    for (unsigned oa = DATA_OFF; oa + 4 <= main_map.na; oa += 4) {
        long ob = a2b(&main_map, oa);
        if (ob < 0 || (unsigned)ob + 4 > main_map.nb) continue;
        unsigned wa = W(main_map.A, oa), wb = W(main_map.B, ob);
        if (wa == wb) continue;
        if (only_links && !(oa >= LINKS_OFF && oa < LINKS_OFF + LINKS_N * 8)) continue;
        unsigned *dst = (unsigned *)(img + oa);
        if (!is_ram_addr(wb) && (is_delta(&main_map, (int)(wb - wa)) || (int)(wb - wa) == ovl_delta)) continue;   /* an unaligned or partial reference moved by the relink */
        if ((wa >> 26) == 2 && (wb >> 26) == 2 && is_delta(&main_map, (int)((wb - wa) << 2))) continue;          /* a j instruction in the data (a stub) */
        if (is_ram_addr(wb)) {
            /* a pointer into the overlay region names an X0 item in MAIN0's terms, and the X0 twin is what will be resident */
            if (wa >= PORT_OVL_BASE && wa < PORT_OVL_END && wb >= PORT_OVL_BASE + ovl_delta && wb < PORT_OVL_END) continue;
            if (bss_pair(&main_map, PORT_MAIN0_LOAD, PORT_MAIN0_LOAD, wa, wb)) continue;    /* MAIN0's bss item, the one the C names */
            Xlat x = xlate(wb);
            if (!x.native && x.addr == wa) continue;           /* the same thing, relinked */
            *dst = x.native ? (unsigned)(unsigned long)x.native : (unsigned)(unsigned long)native_of(x.addr);
            ptrs++; if (x.native) spilled++;
            if (port_verbose > 1 && ptrs <= 40) port_printf("[port2]   MAIN %08x: %08x -> %08x (MAIN0 %08x)%s\n", PORT_MAIN0_LOAD + oa, wb, x.addr, wa, x.native ? " spill" : "");
        } else *dst = wb;
        changed++;
        if (oa >= LINKS_OFF && oa < LINKS_OFF + LINKS_N * 8) links++;
        else if (port_verbose > 2) port_printf("[port2]   MAIN %08x: %08x (MAIN0 %08x)\n", PORT_MAIN0_LOAD + oa, wb, wa);
    }
    bss_decide(&main_map, "MAIN");
    relocate_spill(&main_map);
    named_blobs(&main_map, img, main_map.na, DATA_OFF);
    title_tables(main_map.B, main_map.nb);
    port_printf("[port2] language %d (%s): %s aligned with MAIN0.EXE (%d chunks, %d gaps, %u spill bytes, data end %+d, bss %+d%s); %d data words replaced "
                "(%d pointers, %d into the spill, %d in the file table); overlays expected %+d\n", port_lang, port_lang_name(port_lang), name,
                main_map.nc, main_map.ng, main_map.nspill, main_map.bss_delta, main_map.bss_adj, main_map.bss_seen ? "" : " (assumed)", changed, ptrs, spilled, links / 2, ovl_delta);
    if (port_verbose) for (int k = 0; k < main_map.ng; k++)
        port_printf("[port2]   MAIN gap %08x..%08x (%u) <- MAIN%d %08x (%u)%s\n", PORT_MAIN0_LOAD + main_map.g[k].a, PORT_MAIN0_LOAD + main_map.g[k].a + main_map.g[k].alen,
                    main_map.g[k].alen, port_lang, PORT_MAIN0_LOAD + main_map.g[k].b, main_map.g[k].blen, main_map.g[k].refs ? " (pointed to)" : "");
    /* m0 stays: main0_links, main_map.A; mn (main_map.B, ownB): the targets of the overlay maps, MAIN's Bn */
}

/* ------------------------------------------------------------------ overlays */
static int bcd(unsigned char b) { return (b >> 4) * 10 + (b & 15); }
static int link_lba(const unsigned char *e) { return (bcd(e[0]) * 60 + bcd(e[1])) * 75 + bcd(e[2]) - 150; }
static unsigned link_size(const unsigned char *e) { return e[4] | e[5] << 8 | e[6] << 16 | (unsigned)e[7] << 24; }
extern const PortOvlCode *port_ovl_code_ranges(void);
extern int port_ovl_in_ranges(const PortOvlCode *c, u32p a);

/* After a CdRead of file `id` (the language's table) into the overlay region at dst: compose it over its MAIN0 twin.
   Returns 1 when done (the relocator then runs on MAIN0-layout words). */
int port_lang_overlay_loaded(void *dst, int bytes, int id) {
    if (!port_lang || getenv("PORT_LANG_RAW_OVL")) return 0;     /* diagnosis: the language's overlay as read, its words translated by the relocator */
    u32p base = PORT_OVL_BASE + (u32p)((unsigned char *)dst - (unsigned char *)port_x000_image);
    if (base != PORT_OVL_BASE) { port_printf("[port2] frame %d: --lang: %d bytes read into the overlay region at %08x: not an overlay start, left as is\n", port_frame, bytes, base); return 0; }
    if (id < 0 || id >= LINKS_N) { port_printf("[port2] frame %d: --lang: overlay read not from the file table: not composed\n", port_frame); return 0; }
    const unsigned char *e0 = main0_links + id * 8, *en = FILE_LINKS + id * 8;
    int lba0 = link_lba(e0), lban = link_lba(en); unsigned size0 = link_size(e0), sizen = link_size(en);
    if (lba0 == lban) return 0;                          /* the same file in both tables: nothing to do */
    unsigned sect0 = (size0 + 2047) / 2048, bytes0 = sect0 * 2048;
    if (bytes0 > PORT_OVL_END - PORT_OVL_BASE || sizen > (unsigned)bytes) { port_printf("[port2] frame %d: --lang: file %d: twin %u bytes, read %d: not composed\n", port_frame, id, size0, bytes); return 0; }
    /* the map of this file: the one of its last load when the ring still holds it (the same alignment, the same
       spill and Bn: pointers the game kept from that load stay valid), else a new slot */
    int slot = -1;
    for (int k = 0; k < RING; k++) if (ovl_ring[k].m.nc && ovl_ring[k].m.id == id) slot = k;
    Map *m;
    if (slot >= 0) m = &ovl_ring[slot].m;
    else {
        unsigned char *twin = malloc(bytes0 + 32), *lang = malloc(sizen + 32);
        if (port_cd_read_sectors(lba0, sect0, twin, 2048)) { port_printf("[port2] frame %d: --lang: cannot read the MAIN0 twin of file %d (lba %d)\n", port_frame, id, lba0); free(twin); free(lang); return 0; }
        memcpy(lang, dst, sizen);
        /* the twin is the skeleton (code and data layout); the slot of the ring */
        slot = ovl_loads++ % RING;
        map_free(&ovl_ring[slot].m);
        m = &ovl_ring[slot].m;
        m->A = twin; m->na = size0; m->B = lang; m->nb = sizen; m->ownA = twin; m->ownB = lang; m->id = id;
        m->base_a = PORT_OVL_BASE; m->base_b = PORT_OVL_BASE + ovl_delta; m->other = &main_map;
        align2(m);
        targets(m);
    }
    const unsigned char *twin = m->A, *lang = m->B;
    ovl_cur = slot;
    {   /* the overlay's own idea of its base: the first entry of its pointer table (D?SPSUB2 has none) */
        unsigned p0 = W(twin, 4), pn = W(lang, 4);
        if (p0 >= PORT_OVL_BASE && p0 < PORT_OVL_END && pn >= PORT_OVL_BASE && pn < PORT_OVL_END && (int)(pn - p0) != ovl_delta) {
            port_printf("[port2] frame %d: --lang: file %d is linked %+d from its MAIN0 twin, not %+d as MAIN%d's bss says: using %+d\n", port_frame, id, (int)(pn - p0), ovl_delta, port_lang, (int)(pn - p0));
            ovl_delta = (int)(pn - p0);
        }
    }
    memcpy(dst, twin, bytes0);
    if ((unsigned)bytes > bytes0) memset((unsigned char *)dst + bytes0, 0, bytes - bytes0);
    const PortOvlCode *code = port_ovl_code_ranges();
    int changed = 0, ptrs = 0, spilled = 0;
    unsigned char *img = dst;
    for (unsigned oa = 4; oa + 4 <= size0; oa += 4) {
        if (code && port_ovl_in_ranges(code, PORT_OVL_BASE + oa)) continue;
        long ob = a2b(m, oa);
        if (ob < 0 || (unsigned)ob + 4 > sizen) continue;
        unsigned wa = W(twin, oa), wb = W(lang, ob);
        if (wa == wb) continue;
        unsigned *w = (unsigned *)(img + oa);
        if (!is_ram_addr(wb) && (is_delta(m, (int)(wb - wa) - ovl_delta) || is_delta(&main_map, (int)(wb - wa)))) continue;   /* relinked, not a whole pointer */
        if (is_ram_addr(wb)) {
            if (bss_pair(m, PORT_OVL_BASE, PORT_OVL_BASE + ovl_delta, wa, wb)) continue;   /* the X0 bss item */
            Xlat x = xlate(wb);
            if (!x.native && x.addr == wa) continue;
            *w = x.native ? (unsigned)(unsigned long)x.native : x.addr;      /* MAIN0-layout: the relocator finishes it */
            ptrs++; if (x.native) spilled++;
            if (port_verbose > 1) port_printf("[port2]   ovl +%06x: %08x -> %08x (MAIN0 %08x)%s\n", oa, wb, x.addr, wa, x.native ? " spill" : "");
        } else *w = wb;
        changed++;
    }
    bss_decide(m, "overlay");
    relocate_spill(m);
    int blobs = named_blobs(m, img, bytes0, 4);
    port_printf("[port2] frame %d: --lang: file %d composed over its MAIN0 twin (%u vs %u bytes: %d chunks, %d gaps, %u spill bytes, tail %+d%s): "
                "%d data words from the language (%d pointers, %d into the spill%s%s)\n", port_frame, id, sizen, size0, m->nc, m->ng, m->nspill, m->bss_delta,
                code ? "" : ", no code ranges known", changed, ptrs, spilled, blobs ? ", a WFM3 bank whole" : "", slot >= 0 && m->relocated && ovl_loads ? "" : "");
    if (port_verbose) for (int k = 0; k < m->ng; k++)
        port_printf("[port2]   gap %08x..%08x (%u) <- %08x (%u)%s\n", PORT_OVL_BASE + m->g[k].a, PORT_OVL_BASE + m->g[k].a + m->g[k].alen, m->g[k].alen,
                    PORT_OVL_BASE + ovl_delta + m->g[k].b, m->g[k].blen, m->g[k].refs ? " (pointed to)" : "");
    ovl_composed_pending = 1;
    /* twin and lang stay referenced by the map (A/B) until the slot is reused */
    return 1;
}
/* the relocator asks once per read whether the region was just composed (then its words are MAIN0-layout) */
int port_lang_take_composed(void) { int c = ovl_composed_pending; ovl_composed_pending = 0; return c; }
