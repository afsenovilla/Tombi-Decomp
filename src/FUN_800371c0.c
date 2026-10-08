// FUNC 800371c0 660 MAIN0
// MATCHING 800371c0 660
extern unsigned char *FUN_80037454(int, unsigned char *, int);

void FUN_800371c0(int a, int b, unsigned char *dst, int d)
{
    unsigned char c;
    int *q;
    unsigned char *p;
    int n, a4;
    q = (int *)(a + 4);
    q = (int *)((int)q + (b << 2));
    n = q[1] - *q;
    if (d != -1) {
        *(int *)dst = 0x10;
        *(int *)(dst + 4) = 0;
        *(int *)(dst + 8) = 1;
        *(int *)(dst + 0xc) = d;
        dst = dst + 0x10;
    }
    p = (unsigned char *)(a + *q);
    loop: {
        c = *p++;
        n = n - 1;
        if (n < 1)
            return;
        if (c & 1) {
            int x = *p++;
            int y = *p++;
            n = n - 2;
            dst = FUN_80037454(x, dst, y);
        } else {
            *dst = *p++;
            n = n - 1;
            dst = dst + 1;
        }
        if (c & 2) {
            int x = *p++;
            int y = *p++;
            n = n - 2;
            dst = FUN_80037454(x, dst, y);
        } else {
            *dst = *p++;
            n = n - 1;
            dst = dst + 1;
        }
        if (c & 4) {
            int x = *p++;
            int y = *p++;
            n = n - 2;
            dst = FUN_80037454(x, dst, y);
        } else {
            *dst = *p++;
            n = n - 1;
            dst = dst + 1;
        }
        if (c & 8) {
            int x = *p++;
            int y = *p++;
            n = n - 2;
            dst = FUN_80037454(x, dst, y);
        } else {
            *dst = *p++;
            n = n - 1;
            dst = dst + 1;
        }
        if (c & 16) {
            int x = *p++;
            int y = *p++;
            n = n - 2;
            dst = FUN_80037454(x, dst, y);
        } else {
            *dst = *p++;
            n = n - 1;
            dst = dst + 1;
        }
        if (c & 32) {
            int x = *p++;
            int y = *p++;
            n = n - 2;
            dst = FUN_80037454(x, dst, y);
        } else {
            *dst = *p++;
            n = n - 1;
            dst = dst + 1;
        }
        if (c & 64) {
            int x = *p++;
            int y = *p++;
            n = n - 2;
            dst = FUN_80037454(x, dst, y);
        } else {
            *dst = *p++;
            n = n - 1;
            dst = dst + 1;
        }
        if (c & 128) {
            int x = *p++;
            int y = *p++;
            n = n - 2;
            dst = FUN_80037454(x, dst, y);
        } else {
            *dst = *p++;
            n = n - 1;
            dst = dst + 1;
        }
    }
    goto loop;
}
