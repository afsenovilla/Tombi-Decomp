// FUNC 8004c570 344 MAIN0
// MATCHING 8004c570 344
typedef struct { signed char active; char x; unsigned char type; char pad[12]; unsigned char b0f; } E;
extern unsigned short GAME;
extern unsigned short GAME_SELECTED_SECTION;
extern E **DAT_8007b908[];
extern short AREA_SECTION_COUNTS[];
extern void spawnObjectFromRecord(E *, int);
extern int testAreaFlagBit(int);

void spawnSectionObjects(void)
{
    E ***tab = (E ***)DAT_8007b908[GAME];
    E *p;
    unsigned char *q;
    short n;
    int r;
    char pad;
    unsigned b;
    for (p = *tab[GAME_SELECTED_SECTION & 7]; p->type < 0xff; p++)
        spawnObjectFromRecord(p, -1);
    n = AREA_SECTION_COUNTS[GAME];
    if (n > 15) n = 15;
    p = *tab[n];
    q = &p->type;
    while (*q < 0xff) {
        b = q[0xd] >> 2;
        r = testAreaFlagBit(b);
        if (r == 0) {
            if (p->active == GAME_SELECTED_SECTION || p->active == ~GAME_SELECTED_SECTION)
                spawnObjectFromRecord(p, b);
        }
        q += 16;
        p++;
    }
}
