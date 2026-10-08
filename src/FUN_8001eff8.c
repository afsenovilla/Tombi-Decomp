// FUNC 8001eff8 280 MAIN0
// MATCHING 8001eff8 280
extern short DAT_800a3428;
extern short DAT_8009c8f0;
extern unsigned short DAT_1f8003a8[];
extern unsigned char DAT_80078520[];
extern unsigned char DAT_80078521[];
extern unsigned long *DAT_8009d630[];
extern short DAT_8007836c[];
extern short DAT_8009bd10, DAT_8009bd0c, DAT_8009bd14;
extern void SsSeqStop(int);
extern void SsSeqClose(int);
extern int SsSeqOpen(unsigned long *, short);
extern void SsSetMVol(int, int);
extern void SsSeqSetVol(int, int, int);
extern void SsSeqPlay(int, int, int);

int FUN_8001eff8(int n)
{
    short v;
    if (DAT_800a3428 != -1) {
        SsSeqStop(DAT_800a3428);
        SsSeqClose(DAT_800a3428);
        DAT_800a3428 = -1;
    }
    DAT_8009c8f0 = DAT_1f8003a8[DAT_80078521[n * 4]];
    DAT_800a3428 = SsSeqOpen(DAT_8009d630[DAT_80078520[n * 4]], DAT_8009c8f0);
    SsSetMVol(100, 100);
    v = DAT_8007836c[n];
    SsSeqSetVol(DAT_800a3428, v, v);
    DAT_8009bd10 = DAT_8007836c[n];
    DAT_8009bd0c = 0;
    DAT_8009bd14 = 0;
    SsSeqPlay(DAT_800a3428, 1, 1);
    return 0;
}
