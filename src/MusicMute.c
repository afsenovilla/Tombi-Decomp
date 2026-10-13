// FUNC 8001f620 56 MAIN0
// MATCHING 8001f620 56
extern short BGM_SEQ_ID;
extern short BGM_MUTED;
extern void FUN_80070644(int, int, int);
void MusicMute(void)
{
BGM_MUTED = 1; FUN_80070644(BGM_SEQ_ID, 0, 0);
}
