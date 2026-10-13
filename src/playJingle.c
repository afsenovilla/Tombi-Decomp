// FUNC 8001f2ec 272 MAIN0
// MATCHING 8001f2ec 272
extern short JINGLE_SEQ_ID;
extern short BGM_SEQ_ID;
extern short JINGLE_VAB_ID;
extern int SEQ_DATA[];
extern unsigned short DAT_1f8003ac;
extern void SsSeqStop(int);
extern void SsSeqClose(int);
extern int SsSeqOpen(int, short);
extern void SsSetMVol(int, int);
extern void SsSeqSetVol(int, int, int);
extern void SsSeqPlay(int, int, int);
int playJingle(int p)
{
    int q;
    if (JINGLE_SEQ_ID != -1) {
        SsSeqStop(JINGLE_SEQ_ID);
        SsSeqClose(JINGLE_SEQ_ID);
        JINGLE_SEQ_ID = -1;
    }
    if (p == 1 && BGM_SEQ_ID != -1) {
        SsSeqStop(BGM_SEQ_ID);
        SsSeqClose(BGM_SEQ_ID);
        BGM_SEQ_ID = -1;
    }
    JINGLE_VAB_ID = DAT_1f8003ac;
    JINGLE_SEQ_ID = SsSeqOpen(SEQ_DATA[p], DAT_1f8003ac);
    SsSetMVol(100, 100);
    SsSeqSetVol(JINGLE_SEQ_ID, 0x50, 0x50);
    SsSeqPlay(JINGLE_SEQ_ID, 1, 1);
    return 0;
}
