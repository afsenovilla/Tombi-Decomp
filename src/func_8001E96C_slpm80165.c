// VERSION SLPM_80165 slpm80165 8001E96C 176
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: restarts the area BGM saved in D_800A2A4C (if any), then sets the sound mode from the area table
// (byte +2 of the 4-byte entry of the area D_8009A810): command 0x2000, or 0x9002 / 0x9101 / 0x9240 and 0x2100.
extern short D_800A2A4C;
extern unsigned short D_8009A810;
extern unsigned char D_8006B8C6[];
extern short D_80088FFC;
extern void SsSeqStop(int);
extern void startAreaBgmWithReverb(int);
extern void queueSoundCommand(int, int);

void func_8001E96C(void)
{
    if (D_800A2A4C != -1) {
        SsSeqStop(D_800A2A4C);
        startAreaBgmWithReverb(D_800A2A4C);
        D_800A2A4C = -1;
    }
    if (D_8006B8C6[D_8009A810 * 4] == 0) {
        queueSoundCommand(0x2000, 0);
    } else {
        queueSoundCommand(0x9002, 0);
        queueSoundCommand(0x9101, 0);
        queueSoundCommand(0x9240, 0);
        D_80088FFC = 0;
        queueSoundCommand(0x2100, 0);
    }
}
#endif
