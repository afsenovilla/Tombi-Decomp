// FUNC 8001f2ec 272 MAIN0
extern short DAT_8009f0c8;
extern short DAT_800a3428;
extern short DAT_800a3cb8;
extern int DAT_8009d630[];
extern unsigned short DAT_1f8003ac;
extern void SsSeqStop(int);
extern void SsSeqClose(int);
extern int SsSeqOpen(int, short);
extern void SsSetMVol(int, int);
extern void SsSeqSetVol(int, int, int);
extern void SsSeqPlay(int, int, int);
int FUN_8001f2ec(int p)
{
    if (DAT_8009f0c8 != -1) {
        SsSeqStop(DAT_8009f0c8);
        SsSeqClose(DAT_8009f0c8);
        DAT_8009f0c8 = -1;
    }
    if (p == 1 && DAT_800a3428 != -1) {
        SsSeqStop(DAT_800a3428);
        SsSeqClose(DAT_800a3428);
        DAT_800a3428 = -1;
    }
    int q = DAT_8009d630[p];
    DAT_800a3cb8 = DAT_1f8003ac;
    DAT_8009f0c8 = SsSeqOpen(q, DAT_1f8003ac);
    SsSetMVol(100, 100);
    SsSeqSetVol(DAT_8009f0c8, 0x50, 0x50);
    SsSeqPlay(DAT_8009f0c8, 1, 1);
    return 0;
}
