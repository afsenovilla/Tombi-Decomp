// FUNC 8004c570 344 MAIN0
// MATCHING 8004c570 344
typedef struct { signed char active; char x; unsigned char type; char pad[12]; unsigned char b0f; } E;
extern unsigned short GAME;
extern unsigned short DAT_8009c962;
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
    for (p = *tab[DAT_8009c962 & 7]; p->type < 0xff; p++)
        spawnObjectFromRecord(p, -1);
    n = AREA_SECTION_COUNTS[GAME];
    if (n > 15) n = 15;
    p = *tab[n];
    q = &p->type;
    while (*q < 0xff) {
        b = q[0xd] >> 2;
        r = testAreaFlagBit(b);
        if (r == 0) {
            if (p->active == DAT_8009c962 || p->active == ~DAT_8009c962)
                spawnObjectFromRecord(p, b);
        }
        q += 16;
        p++;
    }
}
