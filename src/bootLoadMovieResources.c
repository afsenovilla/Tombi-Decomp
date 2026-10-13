// FUNC 8004e200 212 MAIN0
// MATCHING 8004e200 212
extern unsigned short *DAT_1f8001d4;
extern char DAT_1f8001ce;
extern void SetDispMask(int);
extern void cdSeekStream(int);
extern void loadSoundSet(int);
extern void queueSystemLoadList(int, int);

void bootLoadMovieResources(void)
{
    unsigned short *o = DAT_1f8001d4;
    short s = o[0x25];
    unsigned short v = s;
    switch (v) {
    case 0:
        SetDispMask(0);
        DAT_1f8001ce = 0;
        loadSoundSet(2);
        queueSystemLoadList(2, 1);
        DAT_1f8001d4[0x25] = DAT_1f8001d4[0x25] + 1;
        break;
    case 1:
        if (DAT_1f8001ce != 0) {
            o[0x25] = s + 1;
            cdSeekStream(0);
        }
        break;
    case 2:
        o[0x24] = 4;
        o[0x25] = 0;
        break;
    }
}
