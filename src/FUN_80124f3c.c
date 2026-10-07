// FUNC 80124f3c 104 X000
extern short FUN_800439f4(void);
void FUN_80124f3c(int a, char *o)
{
    short s;
    short v;
    if (*(int *)(o + 0x94) == 0 && (s = FUN_800439f4(), 0 < s)) {
        if (s == 1)
            v = 2;
        else if (s == 3)
            v = 3;
        else
            return;
        *(short *)(o + 0x2e) = v;
    }
}
