// FUNC 80026000 432 MAIN0
// MATCHING 80026000 432
typedef struct { char c[6]; } B;
extern B DAT_80010238;
extern char DAT_8009d610[];
extern unsigned short DAT_8009d612;
extern unsigned short DAT_8009d614;
extern unsigned char DAT_8009d614b;
extern unsigned char DAT_8009d615;
extern unsigned char DAT_8009d616;
extern unsigned char DAT_8009d617;
extern void PadSetAct();
extern int PadGetState(int);
extern void PadSetActAlign(int, void *);

void updatePadVibration(void)
{
    B buf;
    char *p;
    buf = DAT_80010238;
    p = DAT_8009d610;
    if (DAT_8009d612 == 0)
        return;
    switch (DAT_8009d616) {
    case 0:
        if (DAT_8009d614 != 0) {
            DAT_8009d614b = 0;
            DAT_8009d615 = 0;
            PadSetAct(0, p + 4, 2);
        }
        if (PadGetState(0) == 6)
            PadSetActAlign(0, &buf);
        break;
    case 1:
        if (PadGetState(0) == 6) {
            PadSetActAlign(0, &buf);
            DAT_8009d616 = 2;
        }
        break;
    case 2:
        if (DAT_8009d617 != 0) {
            DAT_8009d617--;
        } else {
            DAT_8009d614b = 0;
            DAT_8009d615 = 0;
            PadSetAct(0, p + 4, 2);
            DAT_8009d617 = 0;
            DAT_8009d616 = 3;
        }
        break;
    case 3:
        if (PadGetState(0) == 6) {
            PadSetActAlign(0, &buf);
            DAT_8009d616 = 0;
        }
        break;
    }
}
