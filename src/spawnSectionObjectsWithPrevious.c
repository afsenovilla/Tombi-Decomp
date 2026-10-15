// FUNC 8004c848 696 MAIN0
// MATCHING 8004c848 696
typedef struct { signed char f0; char p1; unsigned char f2; char p3[0xc]; unsigned char ff; } R;
extern unsigned short DAT_8009c960, DAT_8009c962;
extern char D_8007B720[];
extern char D_8007B73C[];
extern char D_8007B754[];
extern char D_8007B770[];
extern char D_8007B78C[];
extern char D_8007B7CC[];
extern char D_8007B7E0[];
extern char D_8007B7F0[];
extern char D_8007B804[];
extern char D_8007B828[];
extern char D_8007B850[];
extern char D_8007B868[];
extern char D_8007B874[];
extern char D_8007B898[];
extern char D_8007B8B8[];
static char D_8007B8E8[16] = {
    0x58, 3, -0x80, 0x1f, 0x5c, 3, -0x80, 0x1f, 0x60, 3, -0x80, 0x1f, 0x64, 3, -0x80, 0x1f,
};
static char D_8007B8F8[16] = {
    0x58, 3, -0x80, 0x1f, 0x5c, 3, -0x80, 0x1f, 0x60, 3, -0x80, 0x1f, 0x64, 3, -0x80, 0x1f,
};
static R ***DAT_8007b908[20] = {
    (R ***)D_8007B720, (R ***)D_8007B73C, (R ***)D_8007B754, (R ***)D_8007B770,
    (R ***)D_8007B78C, (R ***)D_8007B7CC, (R ***)D_8007B7E0, (R ***)D_8007B73C,
    (R ***)D_8007B7F0, (R ***)D_8007B804, (R ***)D_8007B828, (R ***)D_8007B850,
    (R ***)D_8007B78C, (R ***)D_8007B868, (R ***)D_8007B874, (R ***)D_8007B898,
    (R ***)D_8007B898, (R ***)D_8007B8B8, (R ***)D_8007B8E8, (R ***)D_8007B8F8,
};
extern short DAT_80078790[];
extern int testAreaFlagBit(int);
extern void spawnObjectFromRecord(R *, int);

void spawnSectionObjectsWithPrevious(signed char *o)
{
    R ***tbl;
    R *r;
    short i;
    int s;
    tbl = DAT_8007b908[DAT_8009c960];
    for (r = *tbl[DAT_8009c962 & 7]; r->f2 < 0xff; r++)
        if (r->f0 >= 0) spawnObjectFromRecord(r, -1);
    i = DAT_80078790[DAT_8009c960];
    if (DAT_80078790[DAT_8009c960] >= 16) i = 15;
    for (r = *tbl[i]; r->f2 < 0xff; r++) {
        s = r->ff >> 2;
        if (!testAreaFlagBit(s) && r->f0 == DAT_8009c962) spawnObjectFromRecord(r, s);
    }
    if (o[0xe] > 0) {
        R ***tbl;
        R *r;
        short i;
        int s;
        tbl = DAT_8007b908[DAT_8009c960];
        for (r = *tbl[(DAT_8009c962 - 1) & 7]; r->f2 < 0xff; r++)
            if (r->f0 < 0) spawnObjectFromRecord(r, -1);
        i = DAT_80078790[DAT_8009c960];
        if (DAT_80078790[DAT_8009c960] >= 16) i = 15;
        for (r = *tbl[i]; r->f2 < 0xff; r++) {
            s = r->ff >> 2;
            if (!testAreaFlagBit(s) && r->f0 == -DAT_8009c962) spawnObjectFromRecord(r, s);
        }
    }
}
