// FUNC 8001f4bc 252 MAIN0
// MATCHING 8001f4bc 252
extern short BGM_SEQ_ID, JINGLE_SEQ_ID;
extern short VOICE_SFX_ID[], VOICE_PRIORITY[];
extern short BGM_FADE_ACTIVE, PITCH_SLIDE_ACTIVE, SOUND_QUEUE_TAIL, SOUND_QUEUE_HEAD, SOUND_QUEUE_COUNT;
extern void g1(int a);
extern void g2(int a);
extern void g3(int a, int b);

void SoundStopAll(void)
{
    int i;
    if (BGM_SEQ_ID != -1) {
        g1(BGM_SEQ_ID);
        g2(BGM_SEQ_ID);
        BGM_SEQ_ID = -1;
    }
    if (JINGLE_SEQ_ID != -1) {
        g1(JINGLE_SEQ_ID);
        g2(JINGLE_SEQ_ID);
        JINGLE_SEQ_ID = -1;
    }
    BGM_FADE_ACTIVE = 0;
    PITCH_SLIDE_ACTIVE = 0;
    for (i = 0; i < 24; i++) {
        VOICE_PRIORITY[i] = 0xf;
        VOICE_SFX_ID[i] = -1;
    }
    SOUND_QUEUE_TAIL = 0;
    SOUND_QUEUE_HEAD = 0;
    SOUND_QUEUE_COUNT = 0;
    g3(0, 0xffffff);
}
