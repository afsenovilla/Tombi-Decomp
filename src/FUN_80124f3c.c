// FUNC 80124f3c 104 X000
// MATCHING 80124f3c 104
extern short pushObjectOutOfObjectBoxC(void);
void FUN_80124f3c(int a, char *o)
{
    short s;
    short v;
    if (*(int *)(o + 0x94) == 0 && (s = pushObjectOutOfObjectBoxC(), 0 < s)) {
        if (s == 1)
            v = 2;
        else if (s == 3)
            v = 3;
        else
            return;
        *(short *)(o + 0x2e) = v;
    }
}
