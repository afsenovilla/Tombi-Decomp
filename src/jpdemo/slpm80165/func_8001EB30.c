// JPDEMO slpm80165 8001EB30 132
// Demo-only helper: stops the jingle sequence (restarting the area BGM), resets the sound state and keys all voices off.
extern short D_800A2A4C;
extern unsigned short D_8009F960;
extern short D_80088FEC, D_80089004, D_8009B5D4, D_800A3720;
extern void SsSeqStop(short);
extern void startAreaBgmWithReverb(short);
extern void SpuSetKey(int, int);
void func_8001EB30(void)
{
    if (D_800A2A4C != -1) {
        SsSeqStop(D_800A2A4C);
        startAreaBgmWithReverb(D_800A2A4C);
        D_800A2A4C = -1;
    }
    D_80088FEC = 0;
    D_80089004 = 0;
    D_8009B5D4 = 0;
    D_800A3720 = D_8009F960;
    SpuSetKey(0, 0xffffff);
}
