// FUNC 80018154 44 MAIN0
// MATCHING 80018154 44
// VERSION SLPS_01144 slps01144 80017728 44
#if defined(VERSION_SLPS_01144)
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
