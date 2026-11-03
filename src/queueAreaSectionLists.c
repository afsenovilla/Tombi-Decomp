// FUNC 8004f490 168 MAIN0
// MATCHING 8004f490 168
// VERSION SLPS_01144 slps01144 8001FFFC 184
#if defined(VERSION_SLPS_01144)
#include "globals.h"
extern int LOAD_NEXT_ADDR, LOAD_SLOTS, g2b0;
extern int D_80085360[];
extern int *D_800855EC[];
extern unsigned short D_801FD8E0;
extern void SfxStopAll(int);
extern void queueLoadList(int);
extern void ThreadCreate(int, void *);
extern void func_8001F574(void);
void func_8001FFFC(int a, int b, int c)
{
    int *p;
    if (c == 0) {
        SfxStopAll(1);
    } else {
        queueLoadList(D_80085360[a]);
        LOAD_SLOTS = LOAD_NEXT_ADDR;
    }
    p = D_800855EC[a];
    queueLoadList(p[b]);
    g2b0 = LOAD_NEXT_ADDR;
    *(unsigned char *)0x1F8001CE = 0;
    if (D_801FD8E0 == 0) ThreadCreate(2, func_8001F574);
}
#else
#include "globals.h"
extern int LOAD_NEXT_ADDR, LOAD_SLOTS, g2b0;
extern void SfxStopAll(int);
extern void queueLoadList(int);
void queueAreaSectionLists(int a, int b, int c)
{
    int *p;
    if (c == 0) {
        SfxStopAll(1);
    } else {
        p = AREA_LOAD_LISTS[a];
        queueLoadList(p[0]);
        LOAD_SLOTS = LOAD_NEXT_ADDR;
    }
    p = AREA_LOAD_LISTS[a];
    queueLoadList(p[b + 1]);
    g2b0 = LOAD_NEXT_ADDR;
}
#endif
