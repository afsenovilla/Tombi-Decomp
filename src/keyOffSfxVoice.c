// FUNC 8001e228 96 MAIN0
// MATCHING 8001e228 96
#include "globals.h"
extern short g(short a);

short keyOffSfxVoice(unsigned short a)
{
    if (VOICE_KEY_STATUS[a] != 0) {
        VOICE_PRIORITY[a] = 0xf;
        return g(a);
    }
    return -1;
}
