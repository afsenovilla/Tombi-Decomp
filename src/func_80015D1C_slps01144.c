// VERSION SLPS_01144 slps01144 80015D1C 652
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
typedef unsigned char u8;
typedef unsigned short u16;
extern void SetDispMask(int); extern void SetVideoMode(int); extern void ResetCallback(void);
extern void func_8001606C(void); extern void func_80064B24(int); extern void ResetGraph(int);
extern void InitGeom(void); extern void CdInit(void); extern int CdControl(int, u8 *, u8 *);
extern void func_8001FE14_slps01144(void); extern void DecDCTReset(int); extern void initGraphics(void);
extern void setDrawSyncParams(int, int); extern void func_8001F3E0(void); extern void func_80018658(void);
extern void initTasks(void); extern void startTask(int, void (*)(void)); extern void func_80018818_slps01144(void);
extern void EnterCriticalSection(void); extern void ExitCriticalSection(void);
extern int OpenEvent(unsigned int, int, int, void (*)(void)); extern void vblankHandler(void);
extern void EnableEvent(int); extern void SetRCnt(unsigned int, int, int); extern void func_80058660(unsigned int);
extern void ClearOTagR(void *, int); extern void ThreadTableRun(void); extern void DrawSync(int);
extern void func_80015FA8(void); extern void tickTaskTimers(void);
extern char D_800A4070[]; extern char *DEBUG_NEXT_PRIM;
extern int SP_1D8, SP_1E0;
extern volatile u16 SP_1E8; extern u16 SP_1EA, SP_1EC, SP_1F0;
extern short SP_1F4;
extern u8 SP_1CC;
asm(".set SP_1D8,0x1f8001d8
.set SP_1E0,0x1f8001e0
.set SP_1E8,0x1f8001e8
.set SP_1EA,0x1f8001ea
.set SP_1EC,0x1f8001ec
.set SP_1F0,0x1f8001f0
.set SP_1F4,0x1f8001f4
.set SP_1CC,0x1f8001cc");

void main(void)
{
    u8 mode;
    u16 target;

    SetDispMask(0);
    SetVideoMode(0);
    ResetCallback();
    func_8001606C();
    func_80064B24(0);
    ResetGraph(0);
    InitGeom();
    CdInit();
    mode = 0x80;
    while (CdControl(0xE, &mode, 0) == 0);
    func_8001FE14_slps01144();
    DecDCTReset(0);
    initGraphics();
    setDrawSyncParams(1, 1);
    func_8001F3E0();
    func_80018658();
    initTasks();
    startTask(0, func_80018818_slps01144);
    EnterCriticalSection();
    SP_1D8 = OpenEvent(0xF2000003, 2, 0x1000, vblankHandler);
    ExitCriticalSection();
    EnableEvent(SP_1D8);
    SetRCnt(0xF2000003, 1, 0x1000);
    func_80058660(0xF2000003);
    SetDispMask(1);
    while (1) {
        SP_1E8 = 0;
        ClearOTagR((void *)SP_1E0, 0x328);
        if (SP_1F0 < 0x4001) {
            DEBUG_NEXT_PRIM = SP_1F4 * 0x780 + D_800A4070;
            ThreadTableRun();
        }
        if (SP_1EC != 0) {
            DrawSync(0);
        }
        target = SP_1EA;
        while (SP_1E8 < target);
        if (SP_1EC == 0) {
            ResetGraph(1);
        }
        switch (SP_1CC) {
        case 2:
            break;
        case 0:
        case 1:
            if (SP_1F0 < 0x4001) {
                func_80015FA8();
                tickTaskTimers();
            }
            break;
        case 3:
            SP_1CC = 2;
            if (SP_1F0 < 0x4001) {
                func_80015FA8();
                tickTaskTimers();
            }
            break;
        }
    }
}
#endif
