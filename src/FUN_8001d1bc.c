// FUNC 8001d1bc 432 MAIN0
// MATCHING 8001d1bc 432
extern unsigned short DAT_8009c960[];
extern unsigned short DAT_8009c962;
extern unsigned short DAT_8009f838;
extern unsigned char *PTR_DAT_80077c4c[];
extern unsigned char DAT_8009d2c3;
extern unsigned char DAT_1f8003d3;
extern char DAT_80077c9c[];
extern char DAT_80077ca4[];
extern char DAT_80077cb0[];
extern void FUN_8004fb68(int);
extern void SoundStopAll(void);
extern void FUN_8001eacc(void);

int FUN_8001d1bc(int force)
{
    int idx;
    unsigned char b;
    int ret;
    unsigned char *t;
    unsigned short *g;

    g = DAT_8009c960;
    idx = g[0] + DAT_8009f838;
    t = PTR_DAT_80077c4c[idx];
    b = t[DAT_8009c962];
    ret = -1;
    if (b == 9 && (DAT_8009d2c3 & 2))
        b = 10;
    if (b == 11 && (DAT_8009d2c3 & 4))
        b = 12;
    if (DAT_1f8003d3 != b || force) {
        FUN_8004fb68(b);
        SoundStopAll();
        ret = b;
        DAT_1f8003d3 = b;
    }
    if (idx == 1 || idx == 7) {
        FUN_8001eacc();
        FUN_8004fb68(DAT_80077c9c[g[1]]);
    } else if (idx == 10 && g[1] != 3 && g[1] != 7) {
        FUN_8001eacc();
        FUN_8004fb68(DAT_80077ca4[g[1]]);
    } else if (idx == 12) {
        FUN_8001eacc();
        FUN_8004fb68(DAT_80077cb0[g[1]]);
    }
    return ret;
}
