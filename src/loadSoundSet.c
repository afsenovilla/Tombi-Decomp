// FUNC 8004fb68 64 MAIN0
// MATCHING 8004fb68 64
// VERSION SLPS_01144 slps01144 8002073C 88
#if defined(VERSION_SLPS_01144)
#include "globals.h"
#include "spad.h"
extern void queueLoadList(int);
extern unsigned short D_801FD8E0;
extern void func_8001F574(void);
extern void ThreadCreate(int, void *);

/* JP loadSoundSet also starts the load thread when none runs (PAL: queueSystemLoadList) */
void func_8002073C(int i)
{
    queueLoadList(SOUND_SET_LOAD_LISTS[i]);
    ((unsigned char *)&SPAD.b1c2)[0xc] = 0;
    if (D_801FD8E0 == 0) ThreadCreate(2, func_8001F574);
}
#else
#include "globals.h"
extern int LOAD_NEXT_ADDR;
extern int DAT_1f8002ac;
extern void queueLoadList(int);

void loadSoundSet(int i)
{
    queueLoadList(SOUND_SET_LOAD_LISTS[i]);
    DAT_1f8002ac = LOAD_NEXT_ADDR;
}
#endif
