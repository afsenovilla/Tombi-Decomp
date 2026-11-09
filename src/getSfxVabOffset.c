// FUNC 8001e1ec 60 MAIN0
// MATCHING 8001e1ec 60
extern unsigned char SFX_TONE_TABLE[];
extern char *SFX_BANKS[];
char *getSfxVabOffset(unsigned i){ int k = (i & 0xffff) * 2; return SFX_BANKS[SFX_TONE_TABLE[k]] + SFX_TONE_TABLE[k+1] * 8; }
