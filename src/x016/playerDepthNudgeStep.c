// FUNC 80116e60 144 X016
// MATCHING 80116e60 144
// VERSION SLPS_01144 slps01144 8001BFB4 160
#if defined(VERSION_SLPS_01144)
typedef struct { char p0[0x48]; short w48, w4a, w4c, w4e, w50, w52, w54, w56, w58, w5a, w5c, w5e; } Tsk;
extern Tsk *D_1F8001D4;
#define TASK_C D_1F8001D4

void func_8001BFB4(void)
{
    Tsk *t = TASK_C;
    switch ((unsigned short)t->w4e) {
    case 0:
        *(char *)0x1F8001CF = 1;
        t->w5e = 1;
        t->w4e++;
        break;
    case 1:
        if (--t->w5e <= 0)
            t->w4e++;
        break;
    case 2:
        t->w4a = 3;
        t->w4c = 0;
        break;
    }
}
#else
#include "TOBJ.H"

extern unsigned char D_800A60E0;
extern TObj PLAYER;

void playerDepthNudgeStep(TObj *o)
{
    char pad[16];

    switch (o->state) {
    case 0:
        D_800A60E0 = 8;
        break;
    case 1:
        PLAYER.d->p.whole--;
        break;
    case 2:
        PLAYER.d->p.whole++;
        break;
    }
}
#endif
