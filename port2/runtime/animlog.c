/* --log-anim [FILE] (research, off by default; docs/CUT_CONTENT.md "Unreferenced animation scripts: runtime check").
 *
 * Watches the animation-script pointer of the game objects (TObj +0x24) and the frame each object draws:
 *   - port_anim_note(o, where) runs after every object update of the main pool (updateMainObjects) and inside the
 *     animation helpers (AnimLoadDuration, the call that follows every `o->anim = script`; AnimJump; AnimAdvance,
 *     AnimAdvanceWithBox, tickAnimationFixedRate), hooked by port/fixes.txt;
 *   - port_anim_draw(o) runs in drawObject for every visible object and records the frame number of the current
 *     script entry (the u16 at +0).
 * A pointer inside one of the script windows below (the scripts no pointer, table or code constant references) is
 * printed at once as "[anim] HIT ...".  At exit the summary (every distinct script address and frame per overlay /
 * category / type) goes to FILE, or to the log when no FILE was given.  The windows are retail addresses of the
 * resident area overlay; the port keeps overlay data at its retail offset inside port_ram. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"

int port_log_anim;
const char *port_log_anim_path;

static const struct { const char *ovl; unsigned lo, hi; const char *what; } watch[] = {
    {"X009", 0x8012F144, 0x8012F18C, "jellyfish 43-50 (3 scripts)"},
    {"X011", 0x8011C6E4, 0x8011C72C, "flying octopus 43-50 (3 scripts)"},
    {"X003", 0x80139130, 0x80139150, "swaying hop plant 12-15"},
    {"X003", 0x8013A074, 0x8013A0D4, "wind-blown villager 29-35"},
    {"X003", 0x8013A0E4, 0x8013A144, "wind-blown villager 38-44"},
    {"X003", 0x8013A194, 0x8013A1C4, "wind-blown villager 24-27, 46-47"},
    {"X004", 0x80134E7C, 0x80134EBC, "stone slab 16-19 / 19-16"},
    {"X004", 0x801354E4, 0x8013558C, "NPC frames 185-196 (a +12 copy of the 0x8013540C set)"},
    {"X008", 0x80118504, 0x8011855C, "unknown owner 26-33"},
    {"X010", 0x80131DFC, 0x80131E2C, "AREA10 decor 5-10"},
    {"X014", 0x8012A090, 0x8012A0D8, "AREA14 12-20"},
    {"X014", 0x8012A58C, 0x8012A60C, "AREA14 decor block"},
    {"X014", 0x8012A614, 0x8012A6CC, "AREA14 decor block"},
    {"X014", 0x8012A6D4, 0x8012A72C, "AREA14 decor block"},
    {"X014", 0x8012A758, 0x8012A7A0, "AREA14 decor block"},
    {"X002", 0x8011F2BC, 0x8011F2DC, "AREA02 decor 6-9"},
    {"X019", 0x80116178, 0x80116198, "AREA19 decor 6-9"},
};
/* the jellyfish / octopus script blocks: drawing frames 42..50 from a script of these blocks is flagged too */
static const struct { const char *ovl; unsigned lo, hi; } jelly[] = {
    {"X009", 0x8012EECC, 0x8012F18C}, {"X011", 0x8011C46C, 0x8011C72C},
};

#define NREC 16384
typedef struct { unsigned key, val; unsigned short kind, ovl; unsigned first, count; } Rec;
static Rec recs[NREC];
static int nrec;
static const char *ovl_names[64];
static int novl;

static int ovl_id(const char *n) {
    for (int i = 0; i < novl; i++) if (!strcmp(ovl_names[i], n)) return i;
    if (novl < 64) ovl_names[novl] = n;
    return novl < 64 ? novl++ : 63;
}
static int rec(int kind, int ovl, unsigned key, unsigned val) {
    unsigned h = (key * 2654435761u ^ val * 40503u ^ (unsigned)kind * 977u ^ (unsigned)ovl * 131u) % NREC;
    for (int n = 0; n < NREC; n++, h = (h + 1) % NREC) {
        Rec *r = &recs[h];
        if (!r->count) {
            if (nrec >= NREC - 64) return 0;
            r->key = key; r->val = val; r->kind = (unsigned short)kind; r->ovl = (unsigned short)ovl;
            r->first = (unsigned)port_frame; r->count = 1; nrec++;
            return 1;
        }
        if (r->key == key && r->val == val && r->kind == kind && r->ovl == ovl) { r->count++; return 0; }
    }
    return 0;
}
static unsigned retail(const void *p) {
    const unsigned char *b = p, *r = (const unsigned char *)port_ram;
    return (b >= r && b < r + 0x200000) ? 0x80000000u + (unsigned)(b - r) : 0;
}
static unsigned room(void) {
    unsigned short *g = port_ram_ptr(0x8009C960u);
    return (unsigned)g[0] << 8 | (g[1] & 0xFF);
}

static void summary(void);
static void setup(void) {
    static int done;
    if (done) return;
    done = 1;
    atexit(summary);
}

/* key: area<<24 | section<<16 | category<<8 | type */
static unsigned objkey(const unsigned char *o) { return room() << 16 | (unsigned)(o[0x1C] & 0x7F) << 8 | o[2]; }

/* --log-anim-force CAT:TYPE[,CAT:TYPE...]: every main-pool object of those types is put through b04 (mode) 1..3 x step
   0..11 (state and substep 0), one combination every 110 frames, so that each branch of its update switch runs and
   selects its animation whether or not the player triggers it.  Research only: the forced states can do anything. */
static struct { unsigned char cat, type; } force[16];
static int nforce, force_start = -1;
static struct { unsigned addr; int combo; } forced[256];
void port_log_anim_force(const char *spec) {
    while (spec && *spec && nforce < 16) {
        unsigned c, t;
        if (sscanf(spec, "%u:%x", &c, &t) != 2) break;
        force[nforce].cat = (unsigned char)c; force[nforce].type = (unsigned char)t; nforce++;
        spec = strchr(spec, ',');
        if (spec) spec++;
    }
}
static void force_step(unsigned char *o) {
    int k;
    for (k = 0; k < nforce; k++) if ((o[0x1C] & 0x7F) == force[k].cat && o[2] == force[k].type) break;
    if (k == nforce) return;
    if (force_start < 0) force_start = port_frame + 60;
    if (port_frame < force_start) return;
    int combo = (port_frame - force_start) / 110 % 36;
    unsigned a = 0x80000000u + (unsigned)(o - (unsigned char *)port_ram), h = (a / 0xD4) & 255;
    while (forced[h].addr && forced[h].addr != a) h = (h + 1) & 255;
    if (forced[h].addr == a && forced[h].combo == combo) return;
    forced[h].addr = a; forced[h].combo = combo;
    o[4] = (unsigned char)(1 + combo / 12); o[5] = (unsigned char)(combo % 12); o[6] = 0; o[7] = 0;
    port_printf("[anim] FORCE frame %d object %08X cat %d type %02X: b04 %d step %d\n", port_frame, a, o[0x1C] & 0x7F, o[2], o[4], o[5]);
}

void port_anim_note(void *obj, int where) {
    if (!port_log_anim || !obj) return;
    setup();
    const unsigned char *o = obj;
    if (where == 0 && nforce) force_step((unsigned char *)obj);
    unsigned a = retail(*(void *const *)(o + 0x24));
    if (!a || a < 0x800E8028u || a >= 0x80140000u) return;      /* scripts of MAIN0 (Tombi) and non-pointers */
    const char *ov = port_resident_overlay();
    int id = ovl_id(ov);
    rec(0, id, objkey(o), a);
    if (where == 0 && rec(4, id, objkey(o), retail(o)))         /* a new pool slot: where it is (for --poke) */
        port_printf("[anim] OBJ frame %d %s room %04X cat %d type %02X sub %d at %08X pos %d %d %d script %08X\n", port_frame, ov,
                    room(), o[0x1C] & 0x7F, o[2], o[3], retail(o), *(const short *)(o + 0x12), *(const short *)(o + 0x16),
                    *(const short *)(o + 0x1A), a);
    for (unsigned i = 0; i < sizeof watch / sizeof *watch; i++)
        if (!strcmp(ov, watch[i].ovl) && a >= watch[i].lo && a < watch[i].hi) {
            port_printf("[anim] HIT frame %d %s %08X (%s) by %s: object cat %d type %02X sub %d step %d state %d, room %04X\n",
                        port_frame, ov, a, watch[i].what, where == 0 ? "update" : where == 1 ? "AnimLoadDuration" :
                        where == 2 ? "advance" : "AnimJump", o[0x1C] & 0x7F, o[2], o[3], o[5], o[6], room());
            rec(2, id, objkey(o), a);
        }
}

void port_anim_draw(void *obj) {
    if (!port_log_anim || !obj) return;
    setup();
    const unsigned char *o = obj;
    unsigned a = retail(*(void *const *)(o + 0x24));
    if (!a || a < 0x800E8028u || a >= 0x80140000u) return;
    unsigned short frame = *(const unsigned short *)port_ram_ptr(a);
    const char *ov = port_resident_overlay();
    int id = ovl_id(ov);
    rec(1, id, objkey(o), frame);
    for (unsigned i = 0; i < sizeof jelly / sizeof *jelly; i++)
        if (!strcmp(ov, jelly[i].ovl) && a >= jelly[i].lo && a < jelly[i].hi && frame >= 42 && frame <= 50) {
            port_printf("[anim] HIT frame %d %s draws frame %d (script entry %08X): object cat %d type %02X sub %d, room %04X\n",
                        port_frame, ov, frame, a, o[0x1C] & 0x7F, o[2], o[3], room());
            rec(3, id, objkey(o), frame);
        }
}

static int cmp(const void *x, const void *y) {
    const Rec *a = x, *b = y;
    if (a->ovl != b->ovl) return strcmp(ovl_names[a->ovl], ovl_names[b->ovl]);
    if (a->key != b->key) return a->key < b->key ? -1 : 1;
    if (a->kind != b->kind) return a->kind - b->kind;
    return a->val < b->val ? -1 : a->val > b->val;
}
static void summary(void) {
    Rec *v = malloc(sizeof(Rec) * (nrec + 1));
    int n = 0, hits = 0;
    if (!v) return;
    for (int i = 0; i < NREC; i++) if (recs[i].count) v[n++] = recs[i];
    qsort(v, n, sizeof *v, cmp);
    FILE *f = port_log_anim_path ? fopen(port_log_anim_path, "w") : NULL;
    for (int i = 0; i < n; i++) if (v[i].kind >= 2) hits++;
    char line[256];
#define OUT(...) do { snprintf(line, sizeof line, __VA_ARGS__); if (f) fputs(line, f); else port_printf("%s", line); } while (0)
    OUT("[anim] summary: %d records, %d watched hits (kind 0 = script pointer seen, 1 = frame drawn, 2/3 = watched, 4 = object address)\n", n, hits);
    for (int i = 0; i < n; i++)
        OUT("%s room %02X:%02X cat %d type %02X kind %d %s%X first %u count %u\n", ovl_names[v[i].ovl], v[i].key >> 24,
            (v[i].key >> 16) & 0xFF, (v[i].key >> 8) & 0xFF, v[i].key & 0xFF, v[i].kind, v[i].kind & 1 ? "frame " : "",
            v[i].val, v[i].first, v[i].count);
#undef OUT
    if (f) fclose(f);
    free(v);
}
