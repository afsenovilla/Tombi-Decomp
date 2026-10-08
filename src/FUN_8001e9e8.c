// FUNC 8001e9e8 188 MAIN0
// MATCHING 8001e9e8 188
extern short SoundFindFreeVoice(void);
extern unsigned char DAT_800780c4[];
extern unsigned char DAT_800780c5[];
extern unsigned char *PTR_DAT_800782cc[];
extern short DAT_1f8003a8[];
extern short SsUtKeyOnV(short, int, int, int, int, int, int, int);

int FUN_8001e9e8(unsigned a, short b)
{
    int i;
    unsigned char *p;
    short v = SoundFindFreeVoice();
    i = (a & 0x3ff) * 2;
    p = PTR_DAT_800782cc[DAT_800780c4[i]] + DAT_800780c5[i] * 8;
    return SsUtKeyOnV(v, DAT_1f8003a8[p[0]], p[1], p[2], b, 0, p[4], p[4]);
}
