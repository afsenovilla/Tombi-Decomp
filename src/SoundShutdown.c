// FUNC 8001f820 48 MAIN0
// MATCHING 8001f820 48
#include "globals.h"
extern void f1(void);
extern void f2(void);

void SoundShutdown(void)
{
    SOUND_INITIALIZED = 0;
    f1();
    f2();
}
