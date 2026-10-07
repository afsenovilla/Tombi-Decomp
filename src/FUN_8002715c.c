// FUNC 8002715c 92 MAIN0
extern char *DAT_800a6078;

int FUN_8002715c(int t)
{
    unsigned d;
    int r;
    short s;
    unsigned short *p = (unsigned short *)(DAT_800a6078 + 2);
    d = *p - t + 3;
    r = 1;
    if ((unsigned short)d < 7) {
        *p = t;
    } else {
        if ((int)(d << 16) < 0)
            s = 3;
        else
            s = -3;
        *p = *p + s;
        r = 0;
    }
    return r;
}
