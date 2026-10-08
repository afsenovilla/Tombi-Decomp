// FUNC 80016984 192 MAIN0
extern unsigned short fr;
extern char *cur, *prev;
extern char base[];
extern void PutDispEnv(void *), PutDrawEnv(void *), f(void *), DrawOTag(void *), ClearOTagR(void *, int);
void FlipFrame(void)
{
    int n = 1 - fr;
    int idx = (short)n * 0xd10;
    fr = n;
    prev = cur;
    cur = base + idx;
    PutDispEnv(cur + 0xca0);
    PutDrawEnv(cur + 0xcb4);
    f(prev + 0xc9c);
    DrawOTag(prev + 0xc9c);
    ClearOTagR(cur, 0x328);
}
