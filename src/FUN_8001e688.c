// FUNC 8001e688 228 MAIN0
// MATCHING 8001e688 228
extern unsigned short DAT_8009c960;
extern short DAT_800a3cc8[];
extern int SoundFindFreeVoice(int a);
extern int FUN_8001f3fc(int a, int b);

int FUN_8001e688(unsigned int a, unsigned int b, unsigned int c)
{
    int v;
    if ((DAT_8009c960 == 2 || DAT_8009c960 == 0x13) && a == 0x32) a = 0x61;
    v = SoundFindFreeVoice(a);
    if (v == -1 || FUN_8001f3fc((b & 0xff) | 0x8000, v) == -1 || FUN_8001f3fc((c & 0xff) | 0x8100, v) == -1)
        return -1;
    if (FUN_8001f3fc((a & 0x3ff) | 0xc00, v) != -1)
        DAT_800a3cc8[v] = a;
    return v;
}
