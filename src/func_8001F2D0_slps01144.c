// near-twin of PAL stopBgm (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 8001F2D0 132
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
extern short BGM_SEQ_ID;
extern unsigned short SOUND_QUEUE_TAIL, SOUND_QUEUE_HEAD, SOUND_QUEUE_COUNT, BGM_FADE_ACTIVE, PITCH_SLIDE_ACTIVE;
extern void SsSeqStop(short);
extern void SsSeqClose(short);
extern void SpuSetKey(int, unsigned long);

void func_8001F2D0(int fadeOut)
{
    if (BGM_SEQ_ID != -1) {
        SsSeqStop(BGM_SEQ_ID);
        SsSeqClose(BGM_SEQ_ID);
        BGM_SEQ_ID = -1;
    }
    BGM_FADE_ACTIVE = 0;
    PITCH_SLIDE_ACTIVE = 0;
    SOUND_QUEUE_COUNT = 0;
    SOUND_QUEUE_HEAD = SOUND_QUEUE_TAIL;
    SpuSetKey(0, 0xffffff);
}
#endif
