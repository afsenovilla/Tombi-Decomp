// VERSION BETA_980203 beta980203 80019890 240
#if defined(VERSION_BETA_980203)
// Beta version: no ClearImage step, the movie thread starts in step 0
typedef struct Th { char pad[0x48]; short a; unsigned short c; } Th;
extern Th *TASK_C;
extern char MOVIE_PLAY_STATE, MOVIE_ID;
extern void SetDispMask(int);
extern void initDisplay(int, int, int);
extern void ThreadCreate(int, void *);
extern void stopBgm(int);
extern void func_8001E914_beta980203();

void bootPlayIntroMovie(void)
{
    Th *t = TASK_C;
    unsigned short c = t->c;
    Th *u;
    switch (c) {
    case 0:
        SetDispMask(0);
        initDisplay(0, 0, 0);
        MOVIE_PLAY_STATE = 1;
        MOVIE_ID = 0;
        u = TASK_C;
        u->c = u->c + 1;
        ThreadCreate(1, func_8001E914_beta980203);
        break;
    case 1:
        if (MOVIE_PLAY_STATE == 0) t->c = c + 1;
        break;
    case 2:
        stopBgm(0);
        {
            Th *w = TASK_C;
            w->a = 4;
            w->c = 0;
        }
        break;
    }
}
#endif
