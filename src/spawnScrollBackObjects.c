// FUNC 8004c6c8 384 MAIN0
// MATCHING 8004c6c8 384
typedef struct { signed char f0; char p1; unsigned char f2; char p3[0xc]; unsigned char ff; } R;
extern unsigned char DAT_8009c981;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern R ***DAT_8007b908[];
extern short AREA_SECTION_COUNTS[];
extern int testAreaFlagBit(int);
extern void spawnObjectFromRecord(R *, int);

void spawnScrollBackObjects(signed char *o)
{
    R ***tbl;
    R *r;
    short i;
    int s;
    if (DAT_8009c981 == 1 && o[0xe] > 0) {
        tbl = DAT_8007b908[GAME];
        for (r = *tbl[(GAME_SELECTED_SECTION - 1) & 7]; r->f2 < 0xff; r++)
            if (r->f0 < 0) spawnObjectFromRecord(r, -1);
        i = AREA_SECTION_COUNTS[GAME];
        if (AREA_SECTION_COUNTS[GAME] >= 16) i = 15;
        for (r = *tbl[i]; r->f2 < 0xff; r++) {
            s = r->ff >> 2;
            if (!testAreaFlagBit(s) && r->f0 == -GAME_SELECTED_SECTION) spawnObjectFromRecord(r, s);
        }
    }
}
