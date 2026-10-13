// FUNC 8001e1ec 60 MAIN0
// MATCHING 8001e1ec 60
extern unsigned char DAT_800780c4[];
extern char *SFX_BANKS[];
char *getSfxVabOffset(unsigned i){ int k = (i & 0xffff) * 2; return SFX_BANKS[DAT_800780c4[k]] + DAT_800780c4[k+1] * 8; }
