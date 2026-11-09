// FUNC 8001eff8 280 MAIN0
// MATCHING 8001eff8 280
// VERSION SLPS_01144 slps01144 8001EF50 256
// VERSION SCUS_94236 scus94236 80020AF0 272
// VERSION BETA_980203 beta980203 80020300 272
#if defined(VERSION_SLPS_01144)
#include "globals.h"
extern short BGM_VAB_ID;
extern unsigned short VAB_IDS[];
extern unsigned char BGM_TRACK_DEFS[];
extern unsigned char DAT_80078521[];
extern unsigned long *SEQ_DATA[];
extern short BGM_TRACK_VOLUME[];
extern void SsSeqStop(int);
extern void SsSeqClose(int);
extern int SsSeqOpen(unsigned long *, short);
extern void SsSetMVol(int, int);
extern void SsSeqSetVol(int, int, int);
extern void SsSeqPlay(int, int, int);

int func_8001EF50(int n)
{
    short v;
    if (BGM_SEQ_ID != -1) {
        SsSeqStop(BGM_SEQ_ID);
        SsSeqClose(BGM_SEQ_ID);
        BGM_SEQ_ID = -1;
    }
    BGM_VAB_ID = VAB_IDS[DAT_80078521[n * 4]];
    BGM_SEQ_ID = SsSeqOpen(SEQ_DATA[BGM_TRACK_DEFS[n * 4]], BGM_VAB_ID);
    SsSetMVol(100, 100);
    v = BGM_TRACK_VOLUME[n];
    SsSeqSetVol(BGM_SEQ_ID, v, v);
    BGM_FADE_ACTIVE = 0;
    SsSeqPlay(BGM_SEQ_ID, 1, 1);
    return 0;
    BGM_MUTED = 0;
}
#elif defined(VERSION_SCUS_94236)
// USA version of PAL main0 8001EFF8 (startBgmTrack.c): PAL C of src/startBgmTrack.c (0.986 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "globals.h"
extern short BGM_VAB_ID;
extern unsigned short VAB_IDS[];
extern unsigned char BGM_TRACK_DEFS[];
extern unsigned char DAT_80078521[];
extern unsigned long *SEQ_DATA[];
extern short BGM_TRACK_VOLUME[];
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
    BGM_SEQ_ID = SsSeqOpen(SEQ_DATA[BGM_TRACK_DEFS[n * 4]], BGM_VAB_ID);
    SsSetMVol(100, 100);
    v = BGM_TRACK_VOLUME[n];
    SsSeqSetVol(BGM_SEQ_ID, v, v);
    BGM_VOLUME = BGM_TRACK_VOLUME[n];
    BGM_FADE_ACTIVE = 0;
    SsSeqPlay(BGM_SEQ_ID, 1, 1);
    return 0;
    BGM_MUTED = 0;
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 8001EFF8 (startBgmTrack.c): SCUS_94236 C of src/startBgmTrack.c (func_80020AF0_scus94236, twin score 0.943) [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL main0 8001EFF8 (startBgmTrack.c): PAL C of src/startBgmTrack.c (0.986 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "globals.h"
extern short BGM_VAB_ID;
extern unsigned short VAB_IDS[];
extern unsigned char BGM_TRACK_DEFS[];
extern unsigned char DAT_80078521[];
extern unsigned long *SEQ_DATA[];
extern short BGM_TRACK_VOLUME[];
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
    BGM_SEQ_ID = SsSeqOpen(SEQ_DATA[BGM_TRACK_DEFS[n * 4]], BGM_VAB_ID);
    SsSetMVol(100, 100);
    v = BGM_TRACK_VOLUME[n];
    SsSeqSetVol(BGM_SEQ_ID, v, v);
    BGM_VOLUME = BGM_TRACK_VOLUME[n];
    BGM_FADE_ACTIVE = 0;
    SsSeqPlay(BGM_SEQ_ID, 1, 1);
    return 0;
    BGM_MUTED = 0;
}
#else
#include "globals.h"
extern short BGM_VAB_ID;
extern unsigned short VAB_IDS[];
extern unsigned char BGM_TRACK_DEFS[];
extern unsigned char DAT_80078521[];
extern unsigned long *SEQ_DATA[];
extern short BGM_TRACK_VOLUME[];
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
    BGM_SEQ_ID = SsSeqOpen(SEQ_DATA[BGM_TRACK_DEFS[n * 4]], BGM_VAB_ID);
    SsSetMVol(100, 100);
    v = BGM_TRACK_VOLUME[n];
    SsSeqSetVol(BGM_SEQ_ID, v, v);
    BGM_VOLUME = BGM_TRACK_VOLUME[n];
    BGM_MUTED = 0;
    BGM_FADE_ACTIVE = 0;
    SsSeqPlay(BGM_SEQ_ID, 1, 1);
    return 0;
}
#endif
