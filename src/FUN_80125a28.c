// FUNC 80125a28 132 X000
// MATCHING 80125a28 132
extern short boxesOverlapSideB();
extern int FUN_800428c0(int, int);

void FUN_80125a28(char *a, char *b)
{
    char *t;
    unsigned short u;
    if (boxesOverlapSideB() >= 0 && FUN_800428c0((int)a, (int)b) != 0) {
        t = b;
        if (*(unsigned short *)(b + 0x2c) != 0)
            t = *(char **)(b + 0x28);
        *(unsigned short *)(t + 0x2e) = *(unsigned short *)(a + 0x2e) & 1;
        u = *(unsigned short *)(b + 0x2c);
        t[0x68] = 1;
        t[0x6b] = u;
    }
}
