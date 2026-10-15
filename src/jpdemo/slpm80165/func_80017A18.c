// JPDEMO slpm80165 80017A18 204
// Demo version of PAL main0 80018DE0 (initSpriteSlots.c):  4 message slots of 6 bytes, only the first word reset.
extern char GLYPH_CACHE[];
extern char MESSAGE_SLOTS[];
void initSpriteSlots(void)
{
    int iVar1, iVar2, iVar3;
    char cVar4;
    int j;
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
    iVar1 = -1;
    j = 18;
    do {
        *(short *)(MESSAGE_SLOTS + j) = iVar1;
        j -= 6;
    } while (j >= 0);
}
