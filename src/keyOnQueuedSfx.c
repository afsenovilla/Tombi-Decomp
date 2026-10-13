// FUNC 8001dfec 512 MAIN0
// MATCHING 8001dfec 512
typedef struct { short a; short ch; } CH;
extern unsigned char *getSfxVabOffset(int);
extern short SOUND_QUEUE_HEAD;
extern CH SOUND_QUEUE[];
extern unsigned char VOICE_KEY_STATUS[];
extern short VOICE_SFX_ID[];
extern short VOICE_PRIORITY[];
extern short VAB_IDS[];
extern short SFX_NOTE_OVERRIDE;
extern short SFX_VOLUME_OVERRIDE;
extern int LAST_KEYON_VOICE;
extern short FUN_80070d44(int, int, int, int, int, int, int, int);

int keyOnQueuedSfx(int p)
{
    unsigned char *e;
    short *s;
    int ch;
    e = getSfxVabOffset(p & 0x3ff);
    ch = SOUND_QUEUE[SOUND_QUEUE_HEAD].ch;
    VOICE_KEY_STATUS[ch] = 1;
    VOICE_SFX_ID[ch] = -1;
    s = &VOICE_PRIORITY[ch];
    if (*s < e[5])
        return -1;
    *s = e[5];
    switch (p & 0xc00) {
    case 0:
        LAST_KEYON_VOICE = FUN_80070d44(ch, VAB_IDS[e[0]], e[1], e[2], e[3], 0, e[4], e[4]);
        break;
    case 0x400:
        LAST_KEYON_VOICE = FUN_80070d44(ch, VAB_IDS[e[0]], e[1], e[2], SFX_NOTE_OVERRIDE, 0, e[4], e[4]);
        break;
    case 0x800:
        LAST_KEYON_VOICE = FUN_80070d44(ch, VAB_IDS[e[0]], e[1], e[2], e[3], 0, SFX_VOLUME_OVERRIDE, SFX_VOLUME_OVERRIDE);
        break;
    case 0xc00:
        LAST_KEYON_VOICE = FUN_80070d44(ch, VAB_IDS[e[0]], e[1], e[2], SFX_NOTE_OVERRIDE, 0, SFX_VOLUME_OVERRIDE, SFX_VOLUME_OVERRIDE);
        break;
    }
    return LAST_KEYON_VOICE;
}
