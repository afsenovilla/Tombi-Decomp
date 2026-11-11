// VERSION SCUS_94236 optsub00 800E7A48 788
#if defined(VERSION_SCUS_94236)
// USA version of PAL o0ptsub0 800e8700 (screenAdjustMenuStep.c)
typedef struct S { char p[0x48]; short w48; unsigned short w4a; unsigned short w4c; unsigned short w4e; } S;
extern S *TASK_C;
extern unsigned short JOYPAD_STATE;
extern short SCREEN_OFFSET_X, SCREEN_OFFSET_Y;
extern volatile unsigned short PAD_PRESSED;
extern unsigned short D_8009EB5A;
extern short D_8009D6B8, D_8009D6BA, D_8009D6BC, D_8009D6BE;
extern short D_8009E3C8, D_8009E3CA, D_8009E3CC, D_8009E3CE;
extern int SCREEN_ADJUST_PAD_LAST, SCREEN_ADJUST_PAD_REPEAT_TIMER;
extern short SCREEN_ADJUST_SAVED_X, SCREEN_ADJUST_SAVED_Y;
extern void playSFX(int id);
extern void playSFXAndSetNote(int a, int b);
extern void updateSound(void);
extern void screenAdjustMenuDraw(int dir);

int screenAdjustMenuStep(void)
{
    int dir;
    int held;
    unsigned short b;

    switch (TASK_C->w4e) {
    case 0:
        SCREEN_ADJUST_PAD_LAST = 0;
        SCREEN_ADJUST_PAD_REPEAT_TIMER = 0;
        SCREEN_ADJUST_SAVED_X = SCREEN_OFFSET_X;
        SCREEN_ADJUST_SAVED_Y = SCREEN_OFFSET_Y;
        TASK_C->w4e++;
        break;
    case 1:
        {
            volatile unsigned short *pad = &PAD_PRESSED;
            dir = 0;
            if (*pad & 0x10) dir = 1;
            else if (*pad & 0x40) dir = 2;
            else if (*pad & 0x80) dir = 3;
            else if (*pad & 0x20) dir = 4;
        }
        held = ~D_8009EB5A & 0xF0;
        if (held != SCREEN_ADJUST_PAD_LAST) {
            SCREEN_ADJUST_PAD_LAST = held;
            SCREEN_ADJUST_PAD_REPEAT_TIMER = 0x10;
        } else if (--SCREEN_ADJUST_PAD_REPEAT_TIMER == 0) {
            SCREEN_ADJUST_PAD_REPEAT_TIMER = 6;
            JOYPAD_STATE |= held;
        }
        b = JOYPAD_STATE;
        if (b & 0x10) {
            if (SCREEN_OFFSET_Y > 0) {
                SCREEN_OFFSET_Y--;
                playSFX(8);
            }
        } else if (b & 0x40) {
            if (SCREEN_OFFSET_Y < 0x10) {
                SCREEN_OFFSET_Y++;
                playSFX(8);
            }
        } else if (b & 0x80) {
            if (SCREEN_OFFSET_X >= -7) {
                SCREEN_OFFSET_X--;
                playSFX(8);
            }
        } else if (b & 0x20) {
            if (SCREEN_OFFSET_X < 8) {
                SCREEN_OFFSET_X++;
                playSFX(8);
            }
        } else if (b & 0x4000) {
            playSFXAndSetNote(10, 10);
            TASK_C->w4e = 0;
            TASK_C->w4c = 1;
        } else if (b & 0x2000) {
            SCREEN_OFFSET_X = SCREEN_ADJUST_SAVED_X;
            SCREEN_OFFSET_Y = SCREEN_ADJUST_SAVED_Y;
            playSFXAndSetNote(0x1B, 0x1C);
            TASK_C->w4e = 0;
            TASK_C->w4c = 1;
        }
        D_8009D6BC = 0x100;
        D_8009D6BE = 0xE0;
        D_8009E3CC = 0x100;
        D_8009E3CE = 0xE0;
        D_8009D6B8 = SCREEN_OFFSET_X;
        D_8009D6BA = SCREEN_OFFSET_Y;
        D_8009E3C8 = SCREEN_OFFSET_X;
        D_8009E3CA = SCREEN_OFFSET_Y;
        updateSound();
        break;
    }
    screenAdjustMenuDraw(dir);
    return 0;
}
#endif
