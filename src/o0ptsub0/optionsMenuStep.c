// FUNC 800e8284 916 O0PTSUB0
// MATCHING 800e8284 916
typedef struct S { char p[0x48]; short w48; unsigned short w4a; unsigned short w4c; unsigned short w4e; } S;
typedef struct { char p[2]; unsigned short mode; char p4[6]; unsigned char vib; } PadCfg;
extern S *TASK_C;
extern unsigned short JOYPAD_STATE;
extern PadCfg PAD_INFO;
extern unsigned char D_8009D67C, D_8009F0E8;
extern short OPTIONS_PAD_HAS_ACTUATOR;
extern int PadGetState(int port);
extern unsigned char PadInfoAct(int port, int act, int term);
extern void SsSetMono(void);
extern void SsSetStereo(void);
extern void keyOnSFX(int id);
extern void playSFX(int id);
extern void playSFXAndSetNote(int a, int b);
extern void updateSound(void);
extern void optionsMenuDraw(int);

int optionsMenuStep(void)
{
    unsigned short b;
    int s;
    int w;
    unsigned char c;
    PadCfg *cfg = &PAD_INFO;

    switch (TASK_C->w4e) {
    case 0:
        s = PadGetState(0);
        if (s == 6 || s == 2) {
            c = PadInfoAct(0, -1, 0);
            cfg->vib = c;
            OPTIONS_PAD_HAS_ACTUATOR = c != 0;
            TASK_C->w4e++;
        }
        break;
    case 1:
        b = JOYPAD_STATE;
        if (b & 0x40) {
            playSFX(8);
            TASK_C->w4e++;
        } else if (b & 0x2000) {
            playSFXAndSetNote(10, 10);
            return 1;
        } else if (b & 0x4000) {
            keyOnSFX(8);
            if (++D_8009D67C >= 3) D_8009D67C = 0;
        }
        break;
    case 2:
        b = JOYPAD_STATE;
        if (b & 0x10) {
            playSFX(8);
            TASK_C->w4e--;
        } else if (b & 0x40) {
            playSFX(8);
            if (OPTIONS_PAD_HAS_ACTUATOR == 0) TASK_C->w4e = 4;
            else TASK_C->w4e++;
        } else if (b & 0x2000) {
            playSFXAndSetNote(10, 10);
            return 1;
        } else if (b & 0x4000) {
            playSFX(8);
            if (++D_8009F0E8 >= 2) D_8009F0E8 = 0;
            if (D_8009F0E8 == 0) SsSetStereo();
            else SsSetMono();
        }
        break;
    case 3:
        b = JOYPAD_STATE;
        if (b & 0x10) {
            playSFX(8);
            TASK_C->w4e--;
        } else if (b & 0x40) {
            playSFX(8);
            TASK_C->w4e++;
        } else if (b & 0x2000) {
            playSFXAndSetNote(10, 10);
            return 1;
        } else if (b & 0x4000) {
            playSFX(8);
            PAD_INFO.mode ^= 1;
        }
        break;
    case 4:
        b = JOYPAD_STATE;
        if (b & 0x10) {
            playSFX(8);
            if (OPTIONS_PAD_HAS_ACTUATOR == 0) TASK_C->w4e = 2;
            else TASK_C->w4e--;
        } else if (b & 0x40) {
            playSFX(8);
            TASK_C->w4e++;
        } else if (b & 0x2000) {
            playSFXAndSetNote(10, 10);
            return 1;
        } else if (b & 0x4000) {
            playSFX(8);
            TASK_C->w4e = 0;
            TASK_C->w4c = 3;
        }
        break;
    case 5:
        b = JOYPAD_STATE;
        if (b & 0x10) {
            playSFX(8);
            TASK_C->w4e--;
        } else if (b & 0x2000) {
            playSFXAndSetNote(10, 10);
            return 1;
        } else if (b & 0x4000) {
            playSFX(8);
            TASK_C->w4e = 0;
            TASK_C->w4c = 2;
        }
        break;
    }
    updateSound();
    if ((w = TASK_C->w4e) != 0) optionsMenuDraw(w);
    return 0;
}
