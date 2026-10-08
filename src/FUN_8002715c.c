// FUNC 8002715c 92 MAIN0
// MATCHING 8002715c 92
typedef struct S { short pad; unsigned short v; } S;
extern S *DAT_800a6078;

int FUN_8002715c(int t)
{
    S *s = DAT_800a6078;
    unsigned short cur = s->v;
    unsigned short d = cur - t + 3;
    if (d >= 7) {
        if ((short)d < 0)
            s->v = cur + 3;
        else
            s->v = cur - 3;
        return 0;
    }
    s->v = t;
    return 1;
}
