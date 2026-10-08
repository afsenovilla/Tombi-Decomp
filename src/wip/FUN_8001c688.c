// FUNC 8001c688 400 MAIN0
typedef struct { char p0[0x48]; unsigned short s48, s4a, s4c, s4e; char p1[0x5c - 0x50]; short s5c; } G;
extern G *DAT_1f8001d4;
extern unsigned char DAT_1f8001ce, DAT_1f8001d0;
extern unsigned short DAT_1f8001fc;
extern void FUN_8004fa80(int, int);
extern void FUN_800e8b08(void);
extern void MusicStopOrFadeIn(int);
extern void SoundUpdate(void);
extern void SoundStopAll(void);
extern void FUN_80017328(void *);
extern char LAB_8004dd8c[];

void FUN_8001c688(void)
{
    G *g;
    short t;
    switch (DAT_1f8001d4->s4c) {
    case 0:
        g = DAT_1f8001d4;
        t = g->s4c;
        g->s5c = 0xf0;
        g->s4e = 0;
        DAT_1f8001ce = 0;
        g->s4c = t + 1;
        FUN_8004fa80(0x5d, 1);
        break;
    case 1:
        if (DAT_1f8001ce != 0) DAT_1f8001d4->s4c = 2;
        break;
    case 2:
        FUN_800e8b08();
        g = DAT_1f8001d4;
        t = g->s5c - 1;
        g->s5c = t;
        if (t == -1) {
            g->s4c = g->s4c + 1;
        } else {
            if (t > 0x3c && (DAT_1f8001fc & 0x6008) != 0) g->s5c = 0x3c;
            if (DAT_1f8001d4->s5c == 0x3c) MusicStopOrFadeIn(1);
        }
        SoundUpdate();
        break;
    case 3:
        SoundStopAll();
        g = DAT_1f8001d4;
        DAT_1f8001d0 = 0;
        g->s48 = 1;
        g->s4a = 0;
        g->s4c = 0;
        g->s4e = 0;
        FUN_80017328(LAB_8004dd8c);
        break;
    }
}
