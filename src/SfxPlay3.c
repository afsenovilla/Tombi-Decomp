// FUNC 8001e5f4 148 MAIN0
// MATCHING 8001e5f4 148
extern int SoundFindFreeVoice(int);
extern int FUN_8001f3fc(int, int);
extern unsigned short DAT_800a3cc8[];

int SfxPlay3(int a, int b)
{
    int v;
    v = SoundFindFreeVoice(a);
    if (v == -1) return -1;
    if (FUN_8001f3fc((b & 0xff) | 0x8100, v) == -1) return -1;
    if (FUN_8001f3fc((a & 0x3ff) | 0x800, v) != -1)
        DAT_800a3cc8[v] = a;
    return v;
}
