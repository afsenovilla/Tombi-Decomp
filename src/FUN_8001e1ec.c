// FUNC 8001e1ec 60 MAIN0
// MATCHING 8001e1ec 60
extern unsigned char DAT_800780c4[];
extern char *PTR_DAT_800782cc[];
char *FUN_8001e1ec(unsigned i){ int k = (i & 0xffff) * 2; return PTR_DAT_800782cc[DAT_800780c4[k]] + DAT_800780c4[k+1] * 8; }
