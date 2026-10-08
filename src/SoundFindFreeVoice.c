// FUNC 8001e430 192 MAIN0
// MATCHING 8001e430 192
extern unsigned char DAT_8009f0d0[];
extern short DAT_800a3cf6[];
extern short DAT_8009c8ee[];
extern unsigned char DAT_800780c4[];
extern unsigned char DAT_800780c5[];
extern unsigned char *PTR_DAT_800782cc[];

int SoundFindFreeVoice(unsigned a)
{
    int i;
    short *p;
    int m;
    int k;
    short *q;
    i = 0x17;
    m = -1;
    p = DAT_800a3cf6;
    do {
        if (!DAT_8009f0d0[i] && *p == m)
            return i;
        i--;
        p--;
    } while (i >= 0x10);
    i = 0x17;
    k = (a & 0xffff) * 2;
    q = DAT_8009c8ee;
    do {
        if (*q >= (short)*(PTR_DAT_800782cc[DAT_800780c4[k]] + (DAT_800780c5[k] << 3) + 5))
            return i;
        i--;
        q--;
    } while (i >= 0x10);
    return -1;
}
