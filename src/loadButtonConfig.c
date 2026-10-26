// FUNC 80020eac 104 MAIN0
// MATCHING 80020eac 104
// VERSION SLPS_01144 slps01144 80021DF4 104
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", loadButtonConfig);
#else
typedef struct { unsigned short a, b, c, d; } E;
extern short BUTTON_CONFIG_INDEX;
extern E BUTTON_CONFIGS[];
extern unsigned short S3C4, S3C6, S3C8, S3CA;

void loadButtonConfig(void)
{
    int i = BUTTON_CONFIG_INDEX;
    S3C4 = BUTTON_CONFIGS[i].a;
    S3C6 = BUTTON_CONFIGS[i].b;
    S3C8 = BUTTON_CONFIGS[i].c;
    S3CA = BUTTON_CONFIGS[i].d;
}
#endif
