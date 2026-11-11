// VERSION BETA_980203 beta980203 8001CE0C 396
#if defined(VERSION_BETA_980203)
// Beta version: PAL gameOverScreenState.c: 300-frame timer (PAL 240), skip below 240 (PAL 200), one-argument queueSystemLoadList.
#define IN_GAME D_1F8001D0
typedef struct Th { char pad[0x48]; short a, b; unsigned short c; short d; char pad2[0x5c - 0x50]; short e; } Th;
extern Th *TASK_C;
extern char LOAD_COMPLETE_SYM, IN_GAME;
extern unsigned short JOYPAD_STATE;
extern void stopPadVibration(void);
extern void MusicStopOrFadeIn(int);
extern void SoundStopAll(void);
extern void queueSystemLoadList(int);
extern void SetDispMask(int);
extern void FUN_800e897c(void);
extern void setTask(void *);
extern char LAB_8004dd8c[];

void gameOverScreenState(void)
{
    Th *t = TASK_C;
    short s;
    Th *u;
    Th *w;
    switch (t->c) {
    case 0:
        stopPadVibration();
        MusicStopOrFadeIn(0);
        u = TASK_C;
        u->e = 300;
        u->d = 0;
        u->c = u->c + 1;
        SoundStopAll();
        LOAD_COMPLETE_SYM = 0;
        queueSystemLoadList(0x5d);
        break;
    case 1:
        if (LOAD_COMPLETE_SYM != 0) {
            SetDispMask(1);
            TASK_C->c = TASK_C->c + 1;
        }
        break;
    case 2:
        FUN_800e897c();
        w = TASK_C;
        s = w->e - 1;
        w->e = s;
        if (s == -1) {
            w->c = w->c + 1;
        } else if (s < 240 && (JOYPAD_STATE & 0x6008) != 0) {
            w->e = 0;
        }
        break;
    case 3:
        t->a = 1;
        t->b = 0;
        t->c = 0;
        t->d = 0;
        IN_GAME = 0;
        setTask(LAB_8004dd8c);
        break;
    }
}
#endif
