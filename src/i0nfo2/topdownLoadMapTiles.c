// FUNC 800eca74 420 I0NFO2
// MATCHING 800eca74 420
extern short GetTPage(int tp, int abr, int x, int y);
extern short GetClut(int x, int y);
extern short LoadClut(void *clut, int x, int y);
typedef struct {
    short tp0, clut0, tp1, clut1, tp2, clut2;
    unsigned short w, h, w4, h4;
    char *p;
    char *p2;
} TexInfo;
extern TexInfo D_800A4668;
extern unsigned short D_8009C960[];
extern unsigned char *D_800FECB0;

void topdownLoadMapTiles(void)
{
    unsigned short *src = *(unsigned short **)0x1F800390;
    unsigned char *s;
    int i;
    D_800A4668.w = src[0];
    D_800A4668.h = src[1];
    D_800A4668.w4 = src[0] >> 2;
    D_800A4668.h4 = src[1] >> 2;
    s = (unsigned char *)(src + 2);
    for (i = 0; i < 0x4000; i++) {
        D_800FECB0[i] = *s++;
    }
    D_800A4668.p = *(char **)0x1F800394;
    D_800A4668.p2 = D_800A4668.p + 0x220;
    D_800A4668.tp0 = GetTPage(1, 0, 0x300, 0);
    D_800A4668.clut0 = LoadClut(D_800A4668.p + 0x14, 0x80, 0x1FE);
    D_800A4668.tp1 = GetTPage(1, 0, 0x380, 0);
    D_800A4668.clut1 = GetClut(0x80, 0x1FE);
    if (D_8009C960[0] == 5) {
        D_800A4668.tp2 = GetTPage(1, 0, 0x40, 0x100);
        D_800A4668.clut2 = GetClut(0x80, 0x1FE);
    } else if (*(int *)D_8009C960 == 0x20013) {
        D_800A4668.tp2 = GetTPage(1, 0, 0x40, 0);
        D_800A4668.clut2 = GetClut(0x80, 0x1FE);
    } else {
        D_800A4668.tp2 = GetTPage(1, 0, 0, 0x100);
        D_800A4668.clut2 = GetClut(0x80, 0x1FE);
    }
}
