// FUNC 8001f620 56 MAIN0
extern short DAT_80093428;
extern short DAT_8009d30c;
extern void FUN_80070644(int, int, int);
void MusicMute(void)
{
    DAT_8009d30c = 1;
    FUN_80070644(DAT_80093428, 0, 0);
}
