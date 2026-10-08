// FUNC 8001e4f0 112 MAIN0
// w1: score 20; only s0/s1 swap (game id in s0, r in s1). Tried copies of id, unsigned short/short id, register, goto forms, != vs == forms: no change.
extern int FUN_8001e430(void);
extern int FUN_8001f3fc(int, int);
extern unsigned short DAT_800a3cc8[];
int SfxPlay(int id)
{
    int r;
    r = FUN_8001e430();
    if (r == -1)
        return -1;
    if (FUN_8001f3fc(id & 0x3ff, r) != -1)
        DAT_800a3cc8[r] = id;
    return r;
}
