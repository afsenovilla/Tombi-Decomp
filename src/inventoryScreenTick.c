// FUNC 8001b328 372 MAIN0
// MATCHING 8001b328 372
// VERSION SLPS_01144 slps01144 8001AE18 292
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
#include "globals.h"
typedef struct { char p[0x4e]; unsigned short w; } O;
extern unsigned short TICK_COUNT;
extern short FRAME_BUFFER_INDEX, D45ea;
extern unsigned int NEXT_PRIM;
extern O *TASK_C;
extern int func_800F0AC0(void), func_800EBC00(void), func_800EEFBC(void);
extern void func_800F0D90(void), func_800EC604(void), func_800EF1AC(void);
extern void FUN_800efd6c(void), FUN_800ea37c(void), FUN_800edf08(void), FUN_8005f060(int), FUN_8001dbdc(void);

void func_8001AE18(void)
{
    int r;
    TICK_COUNT = TICK_COUNT + 1;
    NEXT_PRIM = (unsigned int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
    switch (D45ea) {
    case 0: case 1: case 2: case 3: case 6:
        r = func_800F0AC0();
        func_800F0D90();
        break;
    case 4: case 5:
        r = func_800EBC00();
        func_800EC604();
        break;
    case 7:
        r = func_800EEFBC();
        func_800EF1AC();
        break;
    }
    if (r == 2) {
        FUN_8005f060(0);
        TASK_C->w = 1;
        goto end;
    }
    if (r != 0)
        TASK_C->w = TASK_C->w + 1;
end:
    FUN_8001dbdc();
}
#endif
#else
#include "globals.h"
typedef struct { char p[0x4e]; unsigned short w; } O;
extern unsigned short TICK_COUNT;
extern short FRAME_BUFFER_INDEX, D45ea;
extern unsigned int NEXT_PRIM;
extern O *TASK_C;
extern int FUN_800efaa4(void), FUN_800e9888(void), FUN_800edd28(void);
extern void FUN_800efd6c(void), FUN_800ea37c(void), FUN_800edf08(void), FUN_8005f060(int), FUN_8001dbdc(void);

void inventoryScreenTick(void)
{
    int r;
    TICK_COUNT = TICK_COUNT + 1;
    NEXT_PRIM = (unsigned int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
    switch (D45ea) {
    case 0: case 1: case 2: case 3: case 6:
        r = FUN_800efaa4();
        if (r == 0) FUN_800efd6c();
        break;
    case 4: case 5:
        r = FUN_800e9888();
        if (r == 0) FUN_800ea37c();
        break;
    case 7:
        r = FUN_800edd28();
        if (r == 0) FUN_800edf08();
        break;
    }
    if (D45ea == 5) {
        if (r == 1) {
            TASK_C->w = 7;
            goto end;
        }
    } else if (r == 2) {
        FUN_8005f060(0);
        TASK_C->w = 1;
        goto end;
    }
    if (r != 0)
        TASK_C->w = TASK_C->w + 1;
end:
    FUN_8001dbdc();
}
#endif
