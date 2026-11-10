// VERSION SLPM_80165 slpm80165 8001E1A8 136
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of stopBgm (src/stopBgm.c): the sound command's parameter nibble picks a stop (0: the sequence
// is stopped and closed) or a fade start (0x100: flag only).
extern short BGM_SEQ_ID;
extern short BGM_FADE_ACTIVE;
extern void SsSeqStop(short);
extern void SsSeqClose(short);

void func_8001E1A8(int cmd)
{
    switch (cmd & 0xf00) {
    case 0:
        if (BGM_SEQ_ID != -1) {
            SsSeqStop(BGM_SEQ_ID);
            SsSeqClose(BGM_SEQ_ID);
            BGM_SEQ_ID = -1;
        }
        BGM_FADE_ACTIVE = 0;
        break;
    case 0x100:
        __asm__ volatile("");
        BGM_FADE_ACTIVE = 1;
        break;
    }
}
#endif
