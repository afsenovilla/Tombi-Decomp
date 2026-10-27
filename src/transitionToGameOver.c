// FUNC 8001c634 84 MAIN0
// MATCHING 8001c634 84
typedef struct Th { char pad[0x4a]; short a; short b; } Th;
extern Th *TASK_C;
extern char DAT_1f8001cf;
extern char DISPLAY_BUFFER_0_BG_R, DISPLAY_BUFFER_0_BG_G, DISPLAY_BUFFER_0_BG_B;
extern char DISPLAY_BUFFER_1_BG_R, DISPLAY_BUFFER_1_BG_G, DISPLAY_BUFFER_1_BG_B;

void transitionToGameOver(void)
{
    int t = (int)TASK_C;
    DAT_1f8001cf = 1;
    DISPLAY_BUFFER_0_BG_R = 0;
    DISPLAY_BUFFER_0_BG_G = 0;
    DISPLAY_BUFFER_0_BG_B = 0;
    DISPLAY_BUFFER_1_BG_R = 0;
    DISPLAY_BUFFER_1_BG_G = 0;
    DISPLAY_BUFFER_1_BG_B = 0;
    *(short *)(t + 0x4a) = 3;
    *(short *)(t + 0x4c) = 0;
}
