// FUNC 8001e5f4 148 MAIN0
extern int SoundFindFreeVoice(void);
extern int FUN_8001f3fc(int, int);
extern short DAT_800a3cc8[];

int SfxPlay3(int a, int b)
{
    int v = SoundFindFreeVoice();
    if (v == -1 || FUN_8001f3fc((b & 0xff) | 0x8100, v) == -1)
        return -1;
    if (FUN_8001f3fc((a & 0x3ff) | 0x800, v) == -1)
        return v;
    DAT_800a3cc8[v] = a;
    return v;
}
