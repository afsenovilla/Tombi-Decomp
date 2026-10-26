// FUNC 8001e688 228 MAIN0
// MATCHING 8001e688 228
extern unsigned short GAME;
extern short VOICE_SFX_ID[];
extern int SoundFindFreeVoice(int a);
extern int queueSoundCommand(int a, int b);

int playSFXWithNoteAndVolume(unsigned int a, unsigned int b, unsigned int c)
{
    int v;
    if ((GAME == 2 || GAME == 0x13) && a == 0x32) a = 0x61;
    v = SoundFindFreeVoice(a);
    if (v == -1 || queueSoundCommand((b & 0xff) | 0x8000, v) == -1 || queueSoundCommand((c & 0xff) | 0x8100, v) == -1)
        return -1;
    if (queueSoundCommand((a & 0x3ff) | 0xc00, v) != -1)
        VOICE_SFX_ID[v] = a;
    return v;
}
