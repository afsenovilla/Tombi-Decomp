// FUNC 8006ad48 96 MAIN0
// MATCHING 8006ad48 96
extern void (*D_800981B0)(void);

int func_8006AD48(unsigned char *p)
{
    if (p[0x53] != 0) {
        if (p[0x46] == 2) {
            return 1;
        }
        p[0x46] = 0xfe;
    } else {
        D_800981B0();
    }
    return 0;
}
