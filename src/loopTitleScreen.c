// FUNC 8004e41c 1272 MAIN0
// MATCHING 8004e41c 1272
typedef struct {
    char p0[0x48]; short w48; unsigned short w4a; unsigned short w4c; char p4e[0x58 - 0x4e]; short w58;
    char p5a[0x68 - 0x5a]; unsigned char b68, b69, b6a, b6b;
} G;
extern G *TASK_C;
extern unsigned char D_1f8001cc;
extern short D_1f8001f4;
extern unsigned short D_1f8001fc, D_1f8001f8;
extern int D_1f800164;
extern char LOAD_BUFFER_ALT[];
extern unsigned short D_8007DE98[];
extern unsigned short D_8009D670;
extern void func_800198B4();
extern void SetDispMask(int);
extern void FUN_80016ca8(int, int, int);
extern void func_800E8A5C(void);
extern void startBgmTrack(int);
extern void func_800E972C(int, int, int);
extern void func_800E8A94(int *);
extern void playSFX(int);
extern void stopBgm(int);
extern void playSFXAndSetNote(int, int);
extern void setTask(void *);
extern void func_800E9D08(int, int);
extern void updateSound(void);
void loopTitleScreen(int *p)
{
    int d;
    switch (TASK_C->w4a) {
    case 0:
        SetDispMask(0);
        TASK_C->w4c = 0;
        TASK_C->w4a++;
    case 1:
        if (D_1f8001cc == 0) {
            FUN_80016ca8(0, 0, 0);
            TASK_C->w4a++;
        }
        break;
    case 2:
        func_800E8A5C();
        SetDispMask(1);
        startBgmTrack(0);
        D_1f800164 = (int)(LOAD_BUFFER_ALT + D_1f8001f4 * 0xc000) & 0xffffff;
        func_800E972C(0x30, 0xd0, 1);
        TASK_C->b69 = 0;
        TASK_C->b6a = D_8007DE98[TASK_C->b69];
        TASK_C->b6b = D_8007DE98[TASK_C->b69];
        TASK_C->w58 = 0x3cc;
        TASK_C->w4a++;
        break;
    case 3:
        D_1f800164 = (int)(LOAD_BUFFER_ALT + D_1f8001f4 * 0xc000) & 0xffffff;
        if (--TASK_C->w58 <= 0) {
            TASK_C->w4a++;
        }
        func_800E8A94(p);
        if (*(volatile unsigned short *)&D_8009D670 != 0) {
            TASK_C->w58 = 0x3cc;
        }
        switch (TASK_C->w4c) {
        case 0:
            if ((D_1f8001fc & 0x80) && TASK_C->b69 != 0) {
                TASK_C->b69--;
                TASK_C->b6b = D_8007DE98[TASK_C->b69];
                TASK_C->w4c++;
                playSFX(8);
            }
            if ((D_1f8001fc & 0x20) && TASK_C->b69 < 2) {
                TASK_C->b69++;
                TASK_C->b6b = D_8007DE98[TASK_C->b69];
                TASK_C->w4c++;
                playSFX(8);
            }
            if (D_1f8001fc & 0x4008) {
                stopBgm(0);
                switch (TASK_C->b69) {
                case 0:
                    TASK_C->b68 = 0;
                    playSFXAndSetNote(10, 10);
                    setTask(func_800198B4);
                    break;
                case 1:
                    TASK_C->b68 = 1;
                    playSFXAndSetNote(10, 10);
                    setTask(func_800198B4);
                    break;
                case 2:
                    TASK_C->w48 = 2;
                    TASK_C->w4a = 0;
                    playSFXAndSetNote(10, 10);
                    break;
                }
            }
            break;
        case 1:
            if (TASK_C->b6a != TASK_C->b6b) {
                d = -4;
                if (TASK_C->b6a < TASK_C->b6b) d = 4;
                TASK_C->b6a += d;
                if (TASK_C->b6b == TASK_C->b6a) {
                    TASK_C->w4c--;
                }
            }
            break;
        }
        func_800E972C(0x30, 0xd0, 1);
        if (D_1f8001f8 & 0x20) {
            func_800E972C(0x38, 0xa0, 0);
        }
        func_800E9D08(TASK_C->b6a, TASK_C->b69);
        updateSound();
        break;
    case 4:
        stopBgm(0);
        *p = 0;
        TASK_C->w48 = 3;
        TASK_C->w4a = 0;
        break;
    }
}
