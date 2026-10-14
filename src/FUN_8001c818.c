// FUNC 8001c818 400 MAIN0
// MATCHING 8001c818 400
typedef struct Th { char pad[0x48]; short a, b; unsigned short c; short d; char pad2[0x5c - 0x50]; short e; } Th;
extern Th *TASK_C;
extern char LOAD_COMPLETE_SYM, DAT_1f8001d0;
extern unsigned short DAT_1f8001fc;
extern void stopPadVibration(void);
extern void MusicStopOrFadeIn(int);
extern void SoundStopAll(void);
extern void queueSystemLoadList(int, int);
extern void SetDispMask(int);
extern void FUN_800e897c(void);
extern void setTask(void *);
extern char LAB_8004dd8c[];

void FUN_8001c818(void)
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
        u->e = 0xf0;
        u->d = 0;
        u->c = u->c + 1;
        SoundStopAll();
        LOAD_COMPLETE_SYM = 0;
        queueSystemLoadList(0x5d, 1);
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
        } else if (s < 200 && (DAT_1f8001fc & 0x6008) != 0) {
            w->e = 0;
        }
        break;
    case 3:
        t->a = 1;
        t->b = 0;
        t->c = 0;
        t->d = 0;
        DAT_1f8001d0 = 0;
        setTask(LAB_8004dd8c);
        break;
    }
}
