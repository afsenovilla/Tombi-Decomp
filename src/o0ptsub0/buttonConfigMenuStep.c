// FUNC 800e8618 232 O0PTSUB0
// MATCHING 800e8618 232
typedef struct S { char p[0x48]; short w48; unsigned short w4a; unsigned short w4c; unsigned short w4e; } S;
extern S *TASK_C;
extern unsigned short JOYPAD_STATE;
extern short BUTTON_CONFIG_INDEX;
extern void loadButtonConfig(void);
extern void playSFX(int id);
extern void playSFXAndSetNote(int a, int b);
extern void updateSound(void);
extern void buttonConfigMenuDraw(void);

int buttonConfigMenuStep(void)
{
    unsigned short b;
    switch (TASK_C->w4e) {
    case 0:
        TASK_C->w4e++;
        break;
    case 1:
        b = JOYPAD_STATE;
        if (b & 0x2000) {
            playSFXAndSetNote(10, 10);
            TASK_C->w4e = 0;
            TASK_C->w4c = 1;
        } else if (b & 0x4000) {
            playSFX(8);
            if (++BUTTON_CONFIG_INDEX >= 6) BUTTON_CONFIG_INDEX = 0;
            loadButtonConfig();
        }
        updateSound();
        break;
    }
    buttonConfigMenuDraw();
    return 0;
}
