// FUNC 80019d88 528 MAIN0
// MATCHING 80019d88 528
typedef struct P { char p0[0x4a]; unsigned short w4a; unsigned short w4c; unsigned short w4e; } P;
extern char *TASK_C;
extern unsigned char DAT_1f8001b4, LOAD_COMPLETE_SYM, DAT_1f8001cc, DAT_1f8001cd, DAT_1f8001d3;
extern unsigned short DAT_1f8001fc;
extern int DAT_8009f7e4;
extern unsigned char DAT_8009c974;
extern unsigned short DAT_8009c960, DAT_8009c962, DAT_8009c982;
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
        DAT_1f8001cc = 1;
        DAT_1f8001cd = 1;
        ThreadCreate(1, LAB_8001d6a4);
    inc:
        (*(unsigned short *)(TASK_C + 0x4c))++;
        return;
    case 5:
        if (DAT_1f8001cc != 0) {
            if ((DAT_1f8001fc & 0x4008) == 0)
                return;
            DAT_1f8001d3 = 1;
            DAT_1f8001fc = 0;
            *(unsigned short *)(TASK_C + 0x4c) = 6;
            return;
        }
        goto L7;
    case 6:
        if (DAT_1f8001cc != 0)
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
        DAT_8009d2a8 = DAT_8009c960;
        DAT_8009d2aa = DAT_8009c962;
        DAT_8009d2ac = DAT_8009c982;
        return;
    }
}
