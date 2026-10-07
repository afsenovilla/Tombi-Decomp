// FUNC 8001e560 148 MAIN0
extern int SoundFindFreeVoice(void);
extern int g(int a, int b);
extern unsigned short SFX[];

int SfxPlay2(unsigned short a, unsigned int b)
{
    int v = SoundFindFreeVoice();
    if (v != -1) {
        if (g((b & 0xff) | 0x8000, v) != -1) {
            if (g((a & 0x3ff) | 0x400, v) != -1) SFX[v] = a;
            return v;
        }
    }
    return -1;
}
