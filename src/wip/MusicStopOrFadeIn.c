// FUNC 8001f110 176 MAIN0
extern short DAT_800a3428;
extern short DAT_8009bd14;
extern short DAT_8009bd18;
extern short DAT_8009bd1c;
extern short DAT_8009bd20;
extern short DAT_8009bd24;
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8007833c[];
extern void SsSeqStop(int);
extern void SsSeqClose(int);

void MusicStopOrFadeIn(int a)
{
    unsigned short t;
    if (a) {
        a = 1;
        DAT_8009bd1c = a;
        DAT_8009bd18 = a;
        DAT_8009bd14 = a;
        DAT_8009bd20 = -1;
        DAT_8009bd24 = DAT_8007833c[DAT_8009c960] - 1;
    } else {
        if (DAT_800a3428 != -1) {
            SsSeqStop(DAT_800a3428);
            SsSeqClose(DAT_800a3428);
            DAT_800a3428 = -1;
        }
        DAT_8009bd14 = 0;
    }
}
