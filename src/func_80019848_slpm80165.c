// VERSION SLPM_80165 slpm80165 80019848 440
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of gameLoadSaveState (src/gameLoadSaveState_beta980203.c): the primitive pointer is set every frame
// before the switch, the display is masked during the load (no sound / pad calls), the load list is
// func_8001FEDC(5) after func_8001EC84, and the error case returns to the front end func_80018BBC in mode 2.
typedef struct { char pad[0x48]; unsigned short w48, w4a, w4c, w4e; } S1d4;
extern S1d4 *D_1F8001D4;
extern unsigned char D_1F8001CE, D_1F8001D0;
extern short D_1F8001F4;
extern int D_1F800164;
extern char D_800B3498[];
extern short D_800A3D7A;
extern unsigned char D_8009A824, D_8009A817;
extern void SetDispMask(int);
extern void func_8001EC84(void);
extern void func_8001FEDC(int);
extern void clearMenuParams(void);
extern int func_800E90DC(void);
extern void func_800E9A30(void);
extern void func_8001DA00(void);
extern void setTask(void (*)(void));
extern void func_80018BBC(void);

void func_80019848(void)
{
    int r;
    D_1F800164 = (int)(D_800B3498 + D_1F8001F4 * 0xc000) & 0xffffff;
    switch (D_1F8001D4->w4a) {
    case 0:
        SetDispMask(0);
        D_1F8001CE = 0;
        func_8001EC84();
        func_8001FEDC(5);
        D_1F8001D4->w4a++;
        break;
    case 1:
        if (D_1F8001CE == 0)
            break;
        clearMenuParams();
        D_800A3D7A = 5;
        SetDispMask(1);
        D_1F8001D4->w4a++;
        break;
    case 2:
        r = func_800E90DC();
        if (r == 1)
            D_1F8001D4->w4a++;
        else if (r == -1)
            D_1F8001D4->w4a = 4;
        func_800E9A30();
        func_8001DA00();
        break;
    case 3:
        D_8009A824 = 0;
        D_8009A817 = 0;
        D_1F8001D4->w48 = 1;
        D_1F8001D4->w4a = 1;
        D_1F8001D4->w4c = 0;
        D_1F8001D4->w4e = 0;
        break;
    case 4:
        D_1F8001D0 = 0;
        D_1F8001D4->w48 = 2;
        D_1F8001D4->w4a = 0;
        D_1F8001D4->w4c = 0;
        D_1F8001D4->w4e = 0;
        setTask(func_80018BBC);
        break;
    }
}
#endif
