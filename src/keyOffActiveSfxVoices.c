// FUNC 8001eacc 152 MAIN0
// MATCHING 8001eacc 152
#include "globals.h"
extern short DAT_8009c8e0[];
extern short DAT_800a3ce8[];
extern void SsUtKeyOffV(short);
void keyOffActiveSfxVoices(void)
{
    int i;
    for (i = 0x10; i < 0x18; i++) {
        if (VOICE_KEY_STATUS[i] != 0) {
            DAT_8009c8e0[i - 0x10] = 0xf;
            DAT_800a3ce8[i - 0x10] = -1;
            SsUtKeyOffV(i);
        }
    }
}
