// FUNC 80018de0 220 MAIN0
// MATCHING 80018de0 220
extern char GLYPH_CACHE[];
extern char MESSAGE_SLOTS[];
void initSpriteSlots(void)
{
    int iVar1, iVar2, iVar3;
    char cVar4;
    iVar2 = 0;
    cVar4 = 'j';
    iVar3 = 0;
    do {
        iVar1 = iVar3 * 10;
        GLYPH_CACHE[iVar1 + 4] = iVar2;
        iVar2 += 4;
        *(short *)(GLYPH_CACHE + iVar1) = -1;
        *(short *)(GLYPH_CACHE + iVar1 + 2) = 0;
        GLYPH_CACHE[iVar1 + 5] = cVar4;
        GLYPH_CACHE[iVar1 + 6] = 4;
        GLYPH_CACHE[iVar1 + 7] = 0x10;
        GLYPH_CACHE[iVar1 + 8] = 0;
        GLYPH_CACHE[iVar1 + 9] = 0;
        if (0x3c < iVar2) {
            iVar2 = 0;
            cVar4 += 0x10;
        }
        iVar3++;
    } while (iVar3 < 0x3c);
    iVar3 = 0;
    do {
        iVar1 = iVar3 * 8;
        *(short *)(MESSAGE_SLOTS + iVar1) = -1;
        *(short *)(MESSAGE_SLOTS + iVar1 + 2) = -1;
        iVar3++;
    } while (iVar3 < 4);
}
