// VERSION BETA_980203 beta980203 8001A49C 524
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/introSequenceHandler.c with the one-argument queueSystemLoadList(9).
#define IS_DEBUG_MODE_ENABLED D_1F8001B4
typedef struct P { char p0[0x4a]; unsigned short w4a; unsigned short w4c; unsigned short w4e; } P;
extern char *TASK_C;
extern unsigned char IS_DEBUG_MODE_ENABLED, LOAD_COMPLETE_SYM, MOVIE_PLAY_STATE, MOVIE_ID, MOVIE_SKIP_REQUEST;
extern unsigned short JOYPAD_STATE;
extern int LOADING_SCREEN_STATE;
extern unsigned char GAME_AREA_TRANSITION;
extern unsigned short GAME, GAME_SELECTED_SECTION, GAME_SELECTED_SPAWN_POINT;
extern unsigned short LOADED_AREA, LOADED_SECTION, LOADED_SPAWN_POINT;
extern void queueSystemLoadList(int);
extern void loadCurrentArea(int);
extern void displayLoadingScreen(void);
extern void ThreadCreate(int, void *);
extern char LAB_8001d6a4[];

void introSequenceHandler(void)
{
    unsigned char *p974;
    switch (*(unsigned short *)(TASK_C + 0x4c)) {
    case 0:
        queueSystemLoadList(9);
        goto inc;
    case 1:
        if (LOAD_COMPLETE_SYM != 0)
            goto inc;
        return;
    case 2:
        if (IS_DEBUG_MODE_ENABLED != 0)
            goto L7;
        LOADING_SCREEN_STATE = 0;
        LOAD_COMPLETE_SYM = 0;
        loadCurrentArea(1);
        goto inc;
    case 3:
        displayLoadingScreen();
        if (LOAD_COMPLETE_SYM == 0)
            return;
        goto inc;
    case 4:
        MOVIE_PLAY_STATE = 1;
        MOVIE_ID = 1;
        ThreadCreate(1, LAB_8001d6a4);
    inc:
        (*(unsigned short *)(TASK_C + 0x4c))++;
        return;
    case 5:
        if (MOVIE_PLAY_STATE != 0) {
            if ((JOYPAD_STATE & 0x4008) == 0)
                return;
            MOVIE_SKIP_REQUEST = 1;
            JOYPAD_STATE = 0;
            *(unsigned short *)(TASK_C + 0x4c) = 6;
            return;
        }
        goto L7;
    case 6:
        if (MOVIE_PLAY_STATE != 0)
            return;
    L7:
        *(unsigned short *)(TASK_C + 0x4c) = 7;
        return;
    case 7:
        p974 = &GAME_AREA_TRANSITION;
        *p974 = 1;
        *(unsigned short *)(TASK_C + 0x4c) = 1;
        if (IS_DEBUG_MODE_ENABLED != 0) {
            *(unsigned short *)(TASK_C + 0x4c) = 0;
            *p974 = 0;
        }
        *(unsigned short *)(TASK_C + 0x4a) = 1;
        *(unsigned short *)(TASK_C + 0x4e) = 0;
        LOADED_AREA = GAME;
        LOADED_SECTION = GAME_SELECTED_SECTION;
        LOADED_SPAWN_POINT = GAME_SELECTED_SPAWN_POINT;
        return;
    }
}
#endif
