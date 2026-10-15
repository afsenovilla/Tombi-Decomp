// FUNC 80126cc4 104 X001
// MATCHING 80126cc4 104

extern short pushObjectOutOfObjectBoxA(unsigned char *, unsigned char *);

void func_80126CC4(unsigned char *a, unsigned char *b)
{
    b[0x6a] = 0;
    if (pushObjectOutOfObjectBoxA(a, b) == 2 && a[0xc3] != 0) b[0x6a] = a[0xa6];
}
