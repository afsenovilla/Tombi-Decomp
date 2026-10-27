// FUNC 80019d88 528 MAIN0
// MATCHING 80019d88 528
// VERSION SLPM_80165 slpm80165 80019A9C 504
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80019D88 (introSequenceHandler.c): no system load list step (a func_80018254 call
// instead), skip with pad bit 0x800 (PAL 0x4008), states 0-5.
typedef struct P { char p0[0x4a]; unsigned short w4a; unsigned short w4c; unsigned short w4e; } P;
extern char *TASK_C;
extern unsigned char DAT_1f8001b4, LOAD_COMPLETE_SYM, MOVIE_PLAY_STATE, MOVIE_ID, MOVIE_SKIP_REQUEST;
extern unsigned short JOYPAD_STATE;
extern int DAT_8009f7e4;
extern unsigned char DAT_8009c974;
extern unsigned short GAME, GAME_SELECTED_SECTION, GAME_SELECTED_SPAWN_POINT;
extern unsigned short DAT_8009d2a8, DAT_8009d2aa, DAT_8009d2ac;
extern void queueSystemLoadList(int, int);
extern void loadCurrentArea(int);
extern void displayLoadingScreen(void);
extern void ThreadCreate(int, void *);
extern char LAB_8001d6a4[];

extern char D_800A2A18[];
extern void func_80018254(char *);
void introSequenceHandler(void)
{
    unsigned char *p974;
    switch (*(unsigned short *)(TASK_C + 0x4c)) {
    case 0:
        func_80018254(D_800A2A18);
        if (DAT_1f8001b4 != 0)
            goto L7;
        DAT_8009f7e4 = 0;
        LOAD_COMPLETE_SYM = 0;
        loadCurrentArea(1);
        goto inc;
    case 1:
        displayLoadingScreen();
        if (LOAD_COMPLETE_SYM == 0)
            return;
        goto inc;
    case 2:
        func_80018254(D_800A2A18);
        MOVIE_PLAY_STATE = 1;
        MOVIE_ID = 1;
        ThreadCreate(1, LAB_8001d6a4);
    inc:
        (*(unsigned short *)(TASK_C + 0x4c))++;
        return;
    case 3:
        if (MOVIE_PLAY_STATE != 0) {
            if ((JOYPAD_STATE & 0x800) == 0)
                return;
            MOVIE_SKIP_REQUEST = 1;
            *(unsigned short *)(TASK_C + 0x4c) = 4;
            return;
        }
        goto L7;
    case 4:
        if (MOVIE_PLAY_STATE != 0)
            return;
    L7:
        *(unsigned short *)(TASK_C + 0x4c) = 5;
        return;
    case 5:
        p974 = &DAT_8009c974;
        *p974 = 1;
        *(unsigned short *)(TASK_C + 0x4c) = 1;
        if (DAT_1f8001b4 != 0) {
            *(unsigned short *)(TASK_C + 0x4c) = 0;
            *p974 = 0;
        }
        *(unsigned short *)(TASK_C + 0x4a) = 1;
        *(unsigned short *)(TASK_C + 0x4e) = 0;
        DAT_8009d2a8 = GAME;
        DAT_8009d2aa = GAME_SELECTED_SECTION;
        DAT_8009d2ac = GAME_SELECTED_SPAWN_POINT;
        return;
    }
}
#else
typedef struct P { char p0[0x4a]; unsigned short w4a; unsigned short w4c; unsigned short w4e; } P;
extern char *TASK_C;
extern unsigned char DAT_1f8001b4, LOAD_COMPLETE_SYM, MOVIE_PLAY_STATE, MOVIE_ID, MOVIE_SKIP_REQUEST;
extern unsigned short JOYPAD_STATE;
extern int DAT_8009f7e4;
extern unsigned char DAT_8009c974;
extern unsigned short GAME, GAME_SELECTED_SECTION, GAME_SELECTED_SPAWN_POINT;
extern unsigned short DAT_8009d2a8, DAT_8009d2aa, DAT_8009d2ac;
extern void queueSystemLoadList(int, int);
extern void loadCurrentArea(int);
extern void displayLoadingScreen(void);
extern void ThreadCreate(int, void *);
extern char LAB_8001d6a4[];

void introSequenceHandler(void)
{
    unsigned char *p974;
    switch (*(unsigned short *)(TASK_C + 0x4c)) {
    case 0:
        queueSystemLoadList(9, 1);
        goto inc;
    case 1:
        if (LOAD_COMPLETE_SYM != 0)
            goto inc;
        return;
    case 2:
        if (DAT_1f8001b4 != 0)
            goto L7;
        DAT_8009f7e4 = 0;
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
        p974 = &DAT_8009c974;
        *p974 = 1;
        *(unsigned short *)(TASK_C + 0x4c) = 1;
        if (DAT_1f8001b4 != 0) {
            *(unsigned short *)(TASK_C + 0x4c) = 0;
            *p974 = 0;
        }
        *(unsigned short *)(TASK_C + 0x4a) = 1;
        *(unsigned short *)(TASK_C + 0x4e) = 0;
        DAT_8009d2a8 = GAME;
        DAT_8009d2aa = GAME_SELECTED_SECTION;
        DAT_8009d2ac = GAME_SELECTED_SPAWN_POINT;
        return;
    }
}
#endif
