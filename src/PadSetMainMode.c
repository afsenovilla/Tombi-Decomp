// FUNC 800693c8 72 MAIN0
// MATCHING 800693c8 72
// CC gcc-2.8.1
extern int (*D_800981C0)(void);
extern int _padSetMainMode(int, unsigned char, unsigned char);
int PadSetMainMode(int a, unsigned char b, unsigned char c)
{
    return _padSetMainMode(D_800981C0(), b, c);
}
