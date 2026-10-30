// near-twin of setSectionFlagBit (PAL), tools/jp_neartwin.py
// VERSION SLPS_01144 slps01144 80021B3C 180
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
// Demo version of PAL main0 80020C04 (func_80021B3C_slps01144.c): section sizes count double (PAL single).
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern unsigned short AREA_SECTION_COUNTS[];
void func_80021B3C_slps01144(int p)
{
    char pad;
    int i, s = 0, r;
    unsigned short *q;
    int *b;
    for (i = 0; i < (int)GAME; i++)
        s += AREA_SECTION_COUNTS[i] * 2;
    q = &GAME_SELECTED_SECTION;
    s += *q * 2;
    if (p >= 0x20) s++;
    r = p % 32;
    b = (int *)((char *)q + 0x232);
    b[s] |= 1 << r;
}
#else
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern unsigned short AREA_SECTION_COUNTS[];
void func_80021B3C_slps01144(int p)
{
    char pad;
    int i, s = 0, r;
    unsigned short *q;
    int *b;
    for (i = 0; i < (int)GAME; i++)
        s += AREA_SECTION_COUNTS[i];
    q = &GAME_SELECTED_SECTION;
    s += *q * 2;
    if (p >= 0x20) s++;
    r = p % 32;
    b = (int *)((char *)q + 0x232);
    b[s] |= 1 << r;
}
#endif
#endif
