// FUNC 800175f0 136 MAIN0
// MATCHING 800175f0 136
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
