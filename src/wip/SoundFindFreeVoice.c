// FUNC 8001e430 192 MAIN0
extern unsigned char DAT_8009f0d0[];
extern short DAT_800a3cf6[];
extern short DAT_8009c8ee[];
extern unsigned char DAT_800780c4[];
extern unsigned char DAT_800780c5[];
extern unsigned char *PTR_DAT_800782cc[];

int SoundFindFreeVoice(unsigned a)
{
    int i = 0x17;
    short *p = DAT_800a3cf6;
    int m = -1;
    int k;
    short *q;
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
        if (*q >= (short)PTR_DAT_800782cc[DAT_800780c4[k]][DAT_800780c5[k] * 8 + 5])
            return i;
        i--;
        q--;
    } while (i >= 0x10);
    return -1;
}
