// VERSION SLPM_80165 slpm80165 8002157C 80
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of markSectionVisited (src/markSectionVisited.c): one visited-section word per area and one bit
// number per section in the table (PAL: a word index and a bit per section).
extern unsigned short GAME[];
extern unsigned char *TAB[];
extern unsigned short GAME_SECTION; /* GAME[1] */

void markSectionVisited(void)
{
    unsigned short *g = GAME;
    unsigned short a = g[0];
    unsigned short *s = g + 0x4b2;
    unsigned char *t = TAB[a];
    s[a] |= 1 << t[GAME_SECTION];
}
#endif
