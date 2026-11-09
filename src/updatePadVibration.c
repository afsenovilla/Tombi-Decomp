// FUNC 80026000 432 MAIN0
// MATCHING 80026000 432
typedef struct { char c[6]; } B;
extern B DAT_80010238;
extern char PAD_INFO[];
extern unsigned short PAD_VIBRATION_ENABLED;
extern unsigned short PAD_ACT_BUFFER;
extern unsigned char PAD_ACT_BUFFER_B;
extern unsigned char DAT_8009d615;
extern unsigned char PAD_VIBRATION_STATE;
extern unsigned char PAD_VIBRATION_TIMER;
extern void PadSetAct();
extern int PadGetState(int);
extern void PadSetActAlign(int, void *);

void updatePadVibration(void)
{
    B buf;
    char *p;
    buf = DAT_80010238;
    p = PAD_INFO;
    if (PAD_VIBRATION_ENABLED == 0)
        return;
    switch (PAD_VIBRATION_STATE) {
    case 0:
        if (PAD_ACT_BUFFER != 0) {
            PAD_ACT_BUFFER_B = 0;
            DAT_8009d615 = 0;
            PadSetAct(0, p + 4, 2);
        }
        if (PadGetState(0) == 6)
            PadSetActAlign(0, &buf);
        break;
    case 1:
        if (PadGetState(0) == 6) {
            PadSetActAlign(0, &buf);
            PAD_VIBRATION_STATE = 2;
        }
        break;
    case 2:
        if (PAD_VIBRATION_TIMER != 0) {
            PAD_VIBRATION_TIMER--;
        } else {
            PAD_ACT_BUFFER_B = 0;
            DAT_8009d615 = 0;
            PadSetAct(0, p + 4, 2);
            PAD_VIBRATION_TIMER = 0;
            PAD_VIBRATION_STATE = 3;
        }
        break;
    case 3:
        if (PadGetState(0) == 6) {
            PadSetActAlign(0, &buf);
            PAD_VIBRATION_STATE = 0;
        }
        break;
    }
}
