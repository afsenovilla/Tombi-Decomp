// FUNC 8004d260 344 MAIN0
// MATCHING 8004d260 344
typedef struct {
    signed char a;
    unsigned char b;
    unsigned char type;
    unsigned char p[12];
    unsigned char f;
} E4d;
extern E4d ***DAT_8007b908[];
extern unsigned short DAT_8009c960, DAT_8009c962;
extern short DAT_80078790[];
extern void spawnObjectFromRecord(E4d *e, int k);
extern int testAreaFlagBit(int k);

void spawnSectionObjectsAlt(void)
{
    E4d ***t = DAT_8007b908[DAT_8009c960];
    E4d *e;
    short n;
    int k;
    for (e = *t[DAT_8009c962 & 7]; e->type < 0xff; e++)
        spawnObjectFromRecord(e, -1);
    n = DAT_80078790[DAT_8009c960];
    if (n >= 16)
        n = 15;
    for (e = *t[n]; e->type < 0xff; e++) {
        k = e->f >> 2;
        if (testAreaFlagBit(k) == 0) {
            if (e->a == DAT_8009c962 || e->a == ~DAT_8009c962)
                spawnObjectFromRecord(e, k);
        }
    }
}
