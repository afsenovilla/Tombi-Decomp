// near-twin of PAL bootLoadMovieResources (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 8001C174 268
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct { char p0[0x48]; short w48, w4a, w4c, w4e, w50, w52, w54, w56, w58, w5a, w5c, w5e; } Tsk;
extern Tsk *D_1F8001D4;
#define TASK_C D_1F8001D4
extern char D_80010270[];
extern void stopBgm(int);
extern void func_8001F2D0();
extern void drawDebugText(int, int, int, char *);
extern void func_80018A6C_slps01144(void);
extern void setTask(void (*)(void));

void func_8001C174(void)
{
    Tsk *t = TASK_C;
    switch ((unsigned short)t->w4c) {
    case 0:
        stopBgm(0);
        TASK_C->w5c = 90;
        TASK_C->w4c++;
        func_8001F2D0();
        break;
    case 1:
        drawDebugText(96, 96, 0, D_80010270);
        if (--TASK_C->w5c <= 0)
            TASK_C->w4c++;
        break;
    case 2:
        t->w48 = 1;
        t->w4a = 0;
        t->w4c = 0;
        t->w4e = 0;
        *(char *)0x1F8001D0 = 0;
        setTask(func_80018A6C_slps01144);
        break;
    }
}
#endif
