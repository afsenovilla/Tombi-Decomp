// JPDEMO x00 8011BD00 160
// Demo version of PAL x000 80125A28 (FUN_80125a28.c): no second hit test; spawns a hit effect and plays sound 5.
extern short boxesOverlapSideB();
extern int FUN_800428c0(int, int);

extern void func_80020054(int, int, int, int);
extern void func_8001E3D8(int);
void FUN_80125a28(char *a, char *b)
{
    char *t;
    unsigned short u;
    if (boxesOverlapSideB(a, b) >= 0) {
        t = b;
        if (*(unsigned short *)(b + 0x2c) != 0)
            t = *(char **)(b + 0x28);
        *(unsigned short *)(t + 0x2e) = *(unsigned short *)(a + 0x2e) & 1;
        u = *(unsigned short *)(b + 0x2c);
        t[0x68] = 1;
        t[0x6b] = u;
        func_80020054(1, *(short *)(a + 0x12), *(short *)(a + 0x16), *(short *)(a + 0x1a));
        if (a[2] != 1)
            func_8001E3D8(5);
    }
}
