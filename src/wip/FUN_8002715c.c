// FUNC 8002715c 92 MAIN0
typedef struct S { short pad; unsigned short v; } S;
extern S *DAT_800a6078;

int FUN_8002715c(int t)
{
    S *s = DAT_800a6078;
    unsigned cur = s->v;
    unsigned d = cur - t + 3;
    int r = 1;
    if ((d & 0xffff) < 7) {
        s->v = t;
    } else {
        short k = (int)(d << 16) < 0 ? 3 : -3;
        s->v = cur + k;
        r = 0;
    }
    return r;
}
