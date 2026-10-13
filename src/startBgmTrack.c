// FUNC 8001eff8 280 MAIN0
// MATCHING 8001eff8 280
#include "globals.h"
extern short BGM_VAB_ID;
extern unsigned short VAB_IDS[];
extern unsigned char DAT_80078520[];
extern unsigned char DAT_80078521[];
extern unsigned long *SEQ_DATA[];
extern short DAT_8007836c[];
extern void SsSeqStop(int);
extern void SsSeqClose(int);
extern int SsSeqOpen(unsigned long *, short);
extern void SsSetMVol(int, int);
extern void SsSeqSetVol(int, int, int);
extern void SsSeqPlay(int, int, int);

int startBgmTrack(int n)
{
    short v;
    if (BGM_SEQ_ID != -1) {
        SsSeqStop(BGM_SEQ_ID);
        SsSeqClose(BGM_SEQ_ID);
        BGM_SEQ_ID = -1;
    }
    BGM_VAB_ID = VAB_IDS[DAT_80078521[n * 4]];
    BGM_SEQ_ID = SsSeqOpen(SEQ_DATA[DAT_80078520[n * 4]], BGM_VAB_ID);
    SsSetMVol(100, 100);
    v = DAT_8007836c[n];
    SsSeqSetVol(BGM_SEQ_ID, v, v);
    BGM_VOLUME = DAT_8007836c[n];
    BGM_MUTED = 0;
    BGM_FADE_ACTIVE = 0;
    SsSeqPlay(BGM_SEQ_ID, 1, 1);
    return 0;
}
