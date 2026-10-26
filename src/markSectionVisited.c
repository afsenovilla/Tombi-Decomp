// FUNC 800216c4 88 MAIN0
// MATCHING 800216c4 88
extern unsigned short GAME[];
extern unsigned char *TAB[];

void markSectionVisited(void)
{
    unsigned char *p = TAB[GAME[0]] + GAME[1] * 2;
    unsigned short *s = GAME + 0x4b2;
    s[p[0]] |= 1 << p[1];
}
