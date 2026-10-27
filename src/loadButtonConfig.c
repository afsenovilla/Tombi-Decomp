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
extern unsigned short BUTTON_MASK_A, BUTTON_MASK_B, BUTTON_MASK_C, BUTTON_MASK_D;

void loadButtonConfig(void)
{
    int i = BUTTON_CONFIG_INDEX;
    BUTTON_MASK_A = BUTTON_CONFIGS[i].a;
    BUTTON_MASK_B = BUTTON_CONFIGS[i].b;
    BUTTON_MASK_C = BUTTON_CONFIGS[i].c;
    BUTTON_MASK_D = BUTTON_CONFIGS[i].d;
}
#endif
