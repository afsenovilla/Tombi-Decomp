// FUNC 8004c848 696 MAIN0
// MATCHING 8004c848 696
typedef struct { signed char f0; char p1; unsigned char f2; char p3[0xc]; unsigned char ff; } R;
extern unsigned short DAT_8009c960, DAT_8009c962;
extern R ***DAT_8007b908[];
extern short DAT_80078790[];
extern int FUN_80020a78(int);
extern void FUN_8004cb00(R *, int);

void FUN_8004c848(signed char *o)
{
    R ***tbl;
    R *r;
    short i;
    int s;
    tbl = DAT_8007b908[DAT_8009c960];
    for (r = *tbl[DAT_8009c962 & 7]; r->f2 < 0xff; r++)
        if (r->f0 >= 0) FUN_8004cb00(r, -1);
    i = DAT_80078790[DAT_8009c960];
    if (DAT_80078790[DAT_8009c960] >= 16) i = 15;
    for (r = *tbl[i]; r->f2 < 0xff; r++) {
        s = r->ff >> 2;
        if (!FUN_80020a78(s) && r->f0 == DAT_8009c962) FUN_8004cb00(r, s);
    }
    if (o[0xe] > 0) {
        R ***tbl;
        R *r;
        short i;
        int s;
        tbl = DAT_8007b908[DAT_8009c960];
        for (r = *tbl[(DAT_8009c962 - 1) & 7]; r->f2 < 0xff; r++)
            if (r->f0 < 0) FUN_8004cb00(r, -1);
        i = DAT_80078790[DAT_8009c960];
        if (DAT_80078790[DAT_8009c960] >= 16) i = 15;
        for (r = *tbl[i]; r->f2 < 0xff; r++) {
            s = r->ff >> 2;
            if (!FUN_80020a78(s) && r->f0 == -DAT_8009c962) FUN_8004cb00(r, s);
        }
    }
}
