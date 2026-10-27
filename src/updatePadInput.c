// FUNC 8001f850 148 MAIN0
// MATCHING 8001f850 148
extern volatile unsigned short PAD_PRESSED[];
extern volatile unsigned short DAT_8009d672;
extern volatile unsigned short DAT_8009d674;
extern volatile unsigned short DAT_8009d676;
extern unsigned short JOYPAD_STATE;
extern unsigned short DAT_1f8001fe;
extern unsigned short readPadPort(int);
extern void updatePadVibration(void);

void updatePadInput(void)
{
    DAT_8009d674 = PAD_PRESSED[0];
    DAT_8009d676 = DAT_8009d672;
    PAD_PRESSED[0] = readPadPort(0);
    JOYPAD_STATE = ~DAT_8009d674 & PAD_PRESSED[0];
    DAT_1f8001fe = ~PAD_PRESSED[0] & DAT_8009d674;
    updatePadVibration();
}
