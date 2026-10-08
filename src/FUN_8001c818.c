// FUNC 8001c818 400 MAIN0
// MATCHING 8001c818 400
typedef struct Th { char pad[0x48]; short a, b; unsigned short c; short d; char pad2[0x5c - 0x50]; short e; } Th;
extern Th *DAT_1f8001d4;
extern char DAT_1f8001ce, DAT_1f8001d0;
extern unsigned short DAT_1f8001fc;
extern void FUN_800261b0(void);
extern void MusicStopOrFadeIn(int);
extern void SoundStopAll(void);
extern void FUN_8004fa80(int, int);
extern void SetDispMask(int);
extern void FUN_800e897c(void);
extern void FUN_80017328(void *);
extern char LAB_8004dd8c[];

void FUN_8001c818(void)
{
    Th *t = DAT_1f8001d4;
    short s;
    Th *u;
    Th *w;
    switch (t->c) {
    case 0:
        FUN_800261b0();
        MusicStopOrFadeIn(0);
        u = DAT_1f8001d4;
        u->e = 0xf0;
        u->d = 0;
        u->c = u->c + 1;
        SoundStopAll();
        DAT_1f8001ce = 0;
        FUN_8004fa80(0x5d, 1);
        break;
    case 1:
        if (DAT_1f8001ce != 0) {
            SetDispMask(1);
            DAT_1f8001d4->c = DAT_1f8001d4->c + 1;
        }
        break;
    case 2:
        FUN_800e897c();
        w = DAT_1f8001d4;
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
        FUN_80017328(LAB_8004dd8c);
        break;
    }
}
