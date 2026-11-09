// FUNC 8001f850 148 MAIN0
// MATCHING 8001f850 148
extern volatile unsigned short PAD_PRESSED[];
extern volatile unsigned short DAT_8009d672;
extern volatile unsigned short PAD_PRESSED_PREV;
extern volatile unsigned short DAT_8009d676;
extern unsigned short JOYPAD_STATE;
extern unsigned short JOYPAD_RELEASED;
extern unsigned short readPadPort(int);
extern void updatePadVibration(void);

void updatePadInput(void)
{
    PAD_PRESSED_PREV = PAD_PRESSED[0];
    DAT_8009d676 = DAT_8009d672;
    PAD_PRESSED[0] = readPadPort(0);
    JOYPAD_STATE = ~PAD_PRESSED_PREV & PAD_PRESSED[0];
    JOYPAD_RELEASED = ~PAD_PRESSED[0] & PAD_PRESSED_PREV;
    updatePadVibration();
}
