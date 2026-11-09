// FUNC 80025f40 192 MAIN0
// MATCHING 80025f40 192
typedef struct { char c[6]; } B;
extern B DAT_80010238;
extern char PAD_INFO[];
extern unsigned short PAD_VIBRATION_ENABLED;
extern unsigned short PAD_ACT_BUFFER;
extern unsigned char PAD_ACT_BUFFER_B;
extern unsigned char DAT_8009d615;
extern unsigned char PAD_VIBRATION_STATE;
extern unsigned char PAD_VIBRATION_TIMER;
extern void PadSetAct(unsigned char, void *, int, int);

void startPadVibration(int a, int b, int c, int d)
{
    B buf;
    char *p;
    buf = DAT_80010238;
    p = PAD_INFO;
    if (PAD_VIBRATION_ENABLED != 0 && PAD_VIBRATION_STATE == 0 && PAD_ACT_BUFFER == 0) {
        DAT_8009d615 = c;
        PAD_ACT_BUFFER_B = b;
        PadSetAct(a, p + 4, 2, d);
        PAD_VIBRATION_TIMER = d;
        PAD_VIBRATION_STATE = 1;
    }
}
