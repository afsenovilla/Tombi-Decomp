// VERSION BETA_980203 beta980203 80021C8C 232
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/updatePadInput.c that also reads pad 2 and keeps its pressed / released edges (D_800A30BC / D_800A52CA).
#define JOYPAD_RELEASED D_1F8001FE
extern volatile unsigned short PAD_PRESSED[];
extern volatile unsigned short DAT_8009d672;
extern volatile unsigned short PAD_PRESSED_PREV;
extern volatile unsigned short DAT_8009d676;
extern unsigned short JOYPAD_STATE;
extern unsigned short D_800A30BC, D_800A52CA; /* pad 2 pressed / released edges */
extern unsigned short JOYPAD_RELEASED;
extern unsigned short readPadPort(int);
extern void updatePadVibration(void);

void updatePadInput(void)
{
    PAD_PRESSED_PREV = PAD_PRESSED[0];
    DAT_8009d676 = DAT_8009d672;
    PAD_PRESSED[0] = readPadPort(0);
    DAT_8009d672 = readPadPort(1);
    D_800A30BC = ~DAT_8009d676 & DAT_8009d672;
    D_800A52CA = ~DAT_8009d672 & DAT_8009d676;
    JOYPAD_STATE = ~PAD_PRESSED_PREV & PAD_PRESSED[0];
    JOYPAD_RELEASED = ~PAD_PRESSED[0] & PAD_PRESSED_PREV;
    updatePadVibration();
}
#endif
