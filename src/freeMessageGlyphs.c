// FUNC 8002e34c 200 MAIN0
// MATCHING 8002e34c 200
typedef struct A { short n; short x[3]; } A;
typedef struct B { short a; unsigned short m; short c[3]; } B;
typedef struct C { short id; short x[3]; } C;
extern A MESSAGE_SLOTS[];
extern C MESSAGE_GLYPHS[][128];
extern B GLYPH_CACHE[];
void freeMessageGlyphs(unsigned int p)
{
    char pad[4];
    int i;
    int k;
    unsigned int u;
    if (MESSAGE_SLOTS[p].n != -1) {
        for (i = 0; i < MESSAGE_SLOTS[p].n; i++) {
            k = MESSAGE_GLYPHS[p][i].id;
            u = GLYPH_CACHE[k].m & ~(1 << p);
            GLYPH_CACHE[k].m = u;
            if (u == 0) GLYPH_CACHE[k].a = -1;
        }
        MESSAGE_SLOTS[p].n = -1;
    }
}
