// FUNC 80016fc0 124 MAIN0
void FUN_80016fc0(unsigned int a)
{
    unsigned n;
    unsigned int *p;
    unsigned int *r;

    n = 0x327;
    p = (unsigned int *)(a & 0xffffff);
    for (;;) {
        if (*p == (unsigned int)(p - 1)) {
            if (n-- == 0) return;
            r = p;
            p--;
            for (;;) {
                if (*p != (unsigned int)(p - 1)) break;
                if (n-- == 0) return;
                p = (unsigned int *)*p;
            }
            *r = (unsigned int)p;
            if (n-- == 0) return;
            p--;
        } else {
            if (n-- == 0) return;
            p--;
        }
    }
}
