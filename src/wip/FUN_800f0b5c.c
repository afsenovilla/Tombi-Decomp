// FUNC 800f0b5c 204 X000
extern unsigned short a, b;
extern unsigned char c1, c2;
extern short **pp;
int FUN_800f0b5c(void)
{
    char c;
    int r;
    if (a == 1) {
        c = c1;
        if (b == 1) {
L10:
            r = 0;
            if (c == 0) goto L20;
        }
    } else if (a == 9 && b == 1) {
        if (c2 != 0) {
            c = (*pp)[1] < 0x72;
            goto L10;
        }
        r = 0;
        if ((*pp)[1] < 0x73) goto L20;
    }
    r = 0x10000;
L20:
    return r >> 16;
}
