// VERSION PCPX_96084 tomba 80081220 148
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Sprite object init, animation from the table D_80012C54 by +3 (four copies: 80081220 by +3,
// 800812B4/80081348/800813DC by +0xC).
extern void *D_80012C54[];
extern int D_1F80017C;
extern void AnimLoadDuration(void *o);

void func_80081220_pcpx96084(unsigned char *o)
{
    *(short *)(o + 0x6C) = 8;
    *(short *)(o + 0x6E) = 0x10;
    *(short *)(o + 0x72) = 0x10;
    *(short *)(o + 0x1E) = 1;
    *(short *)(o + 0x70) = 0;
    *(int *)(o + 0x8C) = 0;
    o[0xD] = 0;
    *(int *)(o + 0x3C) = D_1F80017C;
    *(void **)(o + 0x24) = D_80012C54[o[3]];
    *(short *)(o + 0xA) = 2;
    AnimLoadDuration(o);
    o[0x1C] |= 0x80;
    o[4]++;
}
#endif
