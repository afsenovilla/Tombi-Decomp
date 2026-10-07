// FUNC 8001f620 56 MAIN0
extern short DAT_a;
extern short DAT_b;
extern void FUN_80070644(int, int, int);
void MusicMute(void)
{
DAT_b = 1; FUN_80070644(DAT_a, 0, 0);
}
