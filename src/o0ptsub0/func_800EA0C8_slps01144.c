// VERSION SLPS_01144 optsub00 800EA0C8 212
#if defined(VERSION_SLPS_01144)
// JP titleLogoDraw (USA func_800E7DDC_scus94236.c, PAL o0ptsub0 800e8a94)
extern unsigned char D_800EAF80; /* bounce phase, also read as a halfword: */
extern unsigned short D_800EAF80_h;
asm(".set D_800EAF80_h, D_800EAF80");
extern short D_800EAF7C;         /* bounce amplitude */
extern unsigned short D_800EAF74; /* bounce offset */
extern char D_800EA8E8[], D_800EA918[];
extern short func_80020D84(int a, int b); /* MulNegSin */
extern void titleDrawSpriteRecs(void *list, int x, short y, int dfe);

void func_800EA0C8(int *paused)
{
    short v;
    if (*paused == 0) {
        v = func_80020D84(D_800EAF80, D_800EAF7C) >> 4;
        D_800EAF74 = v;
        if (v < 0) D_800EAF74 = -v;
        D_800EAF80_h += 2;
        if (D_800EAF7C > 0) D_800EAF7C -= 0x20;
    }
    titleDrawSpriteRecs(D_800EA8E8, 0, -D_800EAF74, 0);
    titleDrawSpriteRecs(D_800EA918, 0, 0, 0);
}
#endif
