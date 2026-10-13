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
extern G *SCRIPT_CTX;
extern unsigned char *SCRIPT_CODE;
extern int SCRIPT_OBJECTS[64];
extern char D_80013650[];
extern int D_8009D69C;
extern int printf(char *, ...);

void func_80038B04(void)
{
    G *g = SCRIPT_CTX;
    unsigned char *t = SCRIPT_CODE;
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
        SCRIPT_OBJECTS[i] = 0;
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
