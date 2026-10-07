// FUNC 8001f658 56 MAIN0
// MATCHING 8001f658 56
extern short DAT_8009bd10;
extern short DAT_8009bd0c;
extern short DAT_800a3428;
extern void SsSeqSetVol(int, int, int);

void MusicUnmute(void)
{
    DAT_8009bd0c = 0;
    SsSeqSetVol(DAT_800a3428, DAT_8009bd10, DAT_8009bd10);
}
