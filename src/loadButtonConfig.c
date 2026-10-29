// FUNC 80020eac 104 MAIN0
// MATCHING 80020eac 104
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
