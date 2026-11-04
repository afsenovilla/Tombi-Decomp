// FUNC 80018154 44 MAIN0
// MATCHING 80018154 44
// VERSION SLPS_01144 slps01144 80017728 44
// VERSION BETA_980203 beta980203 80017D4C 44
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL main0 80018154 (clearOtBuffer.c): 364 -> 268.
extern char AREA_MODEL_DRAW_LIST[];
extern void *memset(void *, int, int);
void clearOtBuffer(void)
{
    memset(AREA_MODEL_DRAW_LIST, 0, 0x10c);
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 80018154 (clearOtBuffer.c): SLPS_01144 branch of src/clearOtBuffer.c [-O2 -G0] (tools/demo_sweep.py)
// Japanese version of PAL main0 80018154 (clearOtBuffer.c): 364 -> 268.
extern char AREA_MODEL_DRAW_LIST[];
extern void *memset(void *, int, int);
void clearOtBuffer(void)
{
    memset(AREA_MODEL_DRAW_LIST, 0, 0x10c);
}
#else
extern char AREA_MODEL_DRAW_LIST[];
extern void *memset(void *, int, int);
void clearOtBuffer(void)
{
    memset(AREA_MODEL_DRAW_LIST, 0, 0x16c);
}
#endif
