// FUNC 800175f0 136 MAIN0
// MATCHING 800175f0 136
// VERSION PCPX_96084 tomba 80019720 128
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 800175F0 (InitSubsystems.c): SLPS_01144 C of src/InitSubsystems.c (InitSubsystems, twin score 0.762), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
extern void clearPlayerObject(void);
extern void initObjectPoolLayer1(void);
extern void initObjectPool(void);
extern void initLayer1DrawList(void);
extern void initMainDrawList(void);
extern void initLayer4DrawList(void);
extern void initObjectPoolLayer8(void);
extern void initLayer5DrawList(void);
extern void clearOtBuffer(void);
extern void initObjectPoolLayer7(void);
extern void clearLayer1Buffer(void);
extern void initObjectPoolUnlayered(void);
extern void clearMenuState(void);
extern void clearMenuParams(void);
void InitSubsystems(void)
{
    initObjectPoolLayer1();
    initObjectPool();
    initLayer1DrawList();
    initMainDrawList();
    initLayer4DrawList();
    initObjectPoolLayer8();
    initLayer5DrawList();
    clearOtBuffer();
    initObjectPoolLayer7();
    clearLayer1Buffer();
    initObjectPoolUnlayered();
    clearMenuState();
    clearMenuParams();
}
#else
extern void clearPlayerObject(void);
extern void initObjectPoolLayer1(void);
extern void initObjectPool(void);
extern void initLayer1DrawList(void);
extern void initMainDrawList(void);
extern void initLayer4DrawList(void);
extern void initObjectPoolLayer8(void);
extern void initLayer5DrawList(void);
extern void clearOtBuffer(void);
extern void initObjectPoolLayer7(void);
extern void clearLayer1Buffer(void);
extern void initObjectPoolUnlayered(void);
extern void clearMenuState(void);
extern void clearMenuParams(void);
void InitSubsystems(void)
{
    clearPlayerObject();
    initObjectPoolLayer1();
    initObjectPool();
    initLayer1DrawList();
    initMainDrawList();
    initLayer4DrawList();
    initObjectPoolLayer8();
    initLayer5DrawList();
    clearOtBuffer();
    initObjectPoolLayer7();
    clearLayer1Buffer();
    initObjectPoolUnlayered();
    clearMenuState();
    clearMenuParams();
}
#endif
