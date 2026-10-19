// FUNC 8001e560 148 MAIN0
// MATCHING 8001e560 148
extern int SoundFindFreeVoice(int);
extern int queueSoundCommand(int, int);
extern unsigned short VOICE_SFX_ID[];

int SfxPlay2(int a, int b)
{
    int v;
    v = SoundFindFreeVoice(a);
    if (v == -1) return -1;
    if (queueSoundCommand((b & 0xff) | 0x8000, v) == -1) return -1;
    if (queueSoundCommand((a & 0x3ff) | 0x400, v) != -1)
        VOICE_SFX_ID[v] = a;
    return v;
}
