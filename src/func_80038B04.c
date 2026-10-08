// FUNC 80038b04 460 MAIN0
// MATCHING 80038b04 460
typedef struct { short m[4]; short w[64]; } F;
typedef struct {
    short w[64];
    F *p80;
    F *p84;
    unsigned char b88;
    char b89;
    unsigned short w8a;
} G;
extern G *D_8009F0F0;
extern unsigned char *D_8009D60C;
extern int D_8009F2D8[64];
extern char D_80013650[];
extern int D_8009D69C;
extern int printf(char *, ...);

void func_80038B04(void)
{
    G *g = D_8009F0F0;
    unsigned char *t = D_8009D60C;
    char buf[80];
    F s;
    F *src;
    F *ps;
    int i, n, j;

    n = t[g->w8a + 1];
    for (i = 0; i < n; i++) {
        buf[i] = t[i + g->w8a + 2];
    }
    buf[i] = 0;
    for (i = 63; i >= 0; i--) {
        D_8009F2D8[i] = 0;
    }
    src = g->p80;
    g->b88 = 0;
    g->w8a = 0;
    ps = &s;
    s = *src;
    if (*(int *)s.m != 0x530057) {
        printf(D_80013650);
        D_8009D69C = 0;
        return;
    }
    for (j = 0; j < 64; j++) {
        g->w[j] = ps->w[j];
    }
    g->p80 = src;
    g->p84 = src + 1;
    g->b88 = 0;
}
