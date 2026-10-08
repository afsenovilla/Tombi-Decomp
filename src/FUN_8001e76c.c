// FUNC 8001e76c 464 MAIN0
// MATCHING 8001e76c 464
extern short DAT_8009bd2c;
extern unsigned short DAT_8009bd40;
extern unsigned char DAT_8009f0d0[];
extern short DAT_8009c8c0[];
extern unsigned char DAT_800780c4[];
extern unsigned char *PTR_DAT_800782cc[];
extern unsigned short DAT_1f8003a8[];
extern short DAT_8009bd44, DAT_8009bd48, DAT_8009bd58, DAT_8009bd50, DAT_8009bd54, DAT_8009bd4c;
extern short DAT_800a3cc8[];
extern void SsUtKeyOffV(int);
extern int SoundFindFreeVoice(unsigned);
extern int FUN_8001f3fc(unsigned, int);

int FUN_8001e76c(unsigned p1, unsigned p2, unsigned p3, unsigned p4)
{
    unsigned short voice;
    int r, i;
    unsigned char *q;
    unsigned idx;
    unsigned sv;
    int t;

    if (DAT_8009bd2c != 0) {
        voice = DAT_8009bd40;
        if (DAT_8009f0d0[voice] != 0) {
            DAT_8009c8c0[voice] = 0xf;
            SsUtKeyOffV((short)voice);
        }
        DAT_8009bd2c = 0;
    }
    sv = p1; asm("" : "=r"(sv) : "0"(sv));
    idx = (sv & 0xffff) * 2;
    q = PTR_DAT_800782cc[DAT_800780c4[idx]] + DAT_800780c4[idx + 1] * 8;
    DAT_8009bd44 = DAT_1f8003a8[q[0]];
    DAT_8009bd48 = q[1];
    t = q[3];
    DAT_8009bd58 = 0;
    DAT_8009bd50 = 0;
    DAT_8009bd54 = t;
    DAT_8009bd4c = t;
    r = SoundFindFreeVoice(p1);
    if (r == -1) return -1;
    if (FUN_8001f3fc(p1 & 0x3ff, r) == -1) return -1;
    if (FUN_8001f3fc(p2 & 0xff | 0xa000, r) == -1) return -1;
    if (FUN_8001f3fc(p3 & 0xff | 0xa100, r) == -1) return -1;
    if (FUN_8001f3fc(p4 & 0xff | 0xa200, r) == -1) return -1;
    if (FUN_8001f3fc(0xa300, r) != -1)
        DAT_800a3cc8[r] = sv;
    return r;
}
