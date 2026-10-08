// FUNC 80018de0 220 MAIN0
// MATCHING 80018de0 220
extern char DAT_800a5de0[];
extern char DAT_800a4648[];
void FUN_80018de0(void)
{
    int iVar1, iVar2, iVar3;
    char cVar4;
    iVar2 = 0;
    cVar4 = 'j';
    iVar3 = 0;
    do {
        iVar1 = iVar3 * 10;
        DAT_800a5de0[iVar1 + 4] = iVar2;
        iVar2 += 4;
        *(short *)(DAT_800a5de0 + iVar1) = -1;
        *(short *)(DAT_800a5de0 + iVar1 + 2) = 0;
        DAT_800a5de0[iVar1 + 5] = cVar4;
        DAT_800a5de0[iVar1 + 6] = 4;
        DAT_800a5de0[iVar1 + 7] = 0x10;
        DAT_800a5de0[iVar1 + 8] = 0;
        DAT_800a5de0[iVar1 + 9] = 0;
        if (0x3c < iVar2) {
            iVar2 = 0;
            cVar4 += 0x10;
        }
        iVar3++;
    } while (iVar3 < 0x3c);
    iVar3 = 0;
    do {
        iVar1 = iVar3 * 8;
        *(short *)(DAT_800a4648 + iVar1) = -1;
        *(short *)(DAT_800a4648 + iVar1 + 2) = -1;
        iVar3++;
    } while (iVar3 < 4);
}
