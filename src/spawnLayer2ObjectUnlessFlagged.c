// FUNC 80020d98 276 MAIN0
// MATCHING 80020d98 276
#include "TOBJ.H"
typedef struct { unsigned char b0, b1, b2, b3, b4; } SP;
extern unsigned short GAME;
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned short AREA_SECTION_COUNTS[];
extern unsigned int D_8009CB94[];
extern TObj *allocObjectLayer2(void);

TObj *spawnLayer2ObjectUnlessFlagged(SP *p)
{
    int i, n, k;
    TObj *o;
    char pad;
    k = p->b3;
    n = 0;
    for (i = 0; i < GAME; i++) {
        n += AREA_SECTION_COUNTS[i];
    }
    n += GAME_SELECTED_SECTION;
    if (k >= 32) n++;
    { int s = k % 32; if (D_8009CB94[n] & (1 << s)) return 0; }
    if (p->b4 != 2) return 0;
    o = allocObjectLayer2();
    if (o == 0) return 0;
o->type = p->b0;
o->subtype = p->b1;
o->active = 2;
o->b0c = p->b2 | 0x80;
    o->b6b = p->b3;
    return o;
}
