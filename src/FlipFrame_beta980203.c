// VERSION BETA_980203 beta980203 80016608 172
#if defined(VERSION_BETA_980203)
// Beta version: PAL FlipFrame.c: index toggled with ^ 1, no ClearOTagR of the new list.
#include "globals.h"
extern unsigned short FRAME_BUFFER_INDEX;
extern char *CURRENT_OT, *prev;
extern void PutDispEnv(void *), PutDrawEnv(void *), f(void *), DrawOTag(void *), ClearOTagR(void *, int);
void FlipFrame(void)
{
    int n = FRAME_BUFFER_INDEX ^ 1;
    int idx = (short)n * 0xd10;
    FRAME_BUFFER_INDEX = n;
    prev = CURRENT_OT;
    CURRENT_OT = OT_FRAMEBUFFER + idx;
    PutDispEnv(CURRENT_OT + 0xca0);
    PutDrawEnv(CURRENT_OT + 0xcb4);
    f(prev + 0xc9c);
    DrawOTag(prev + 0xc9c);
}
#endif
