// FUNC 8001f658 56 MAIN0
// MATCHING 8001f658 56
#include "globals.h"
extern void SsSeqSetVol(int, int, int);

void MusicUnmute(void)
{
    BGM_MUTED = 0;
    SsSeqSetVol(BGM_SEQ_ID, BGM_VOLUME, BGM_VOLUME);
}
