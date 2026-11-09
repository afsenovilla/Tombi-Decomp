// FUNC 8006ad48 96 MAIN0
// MATCHING 8006ad48 96
extern void (*_padFuncClrInfo)(void);

int _padChkMainModeDone(unsigned char *p)
{
    if (p[0x53] != 0) {
        if (p[0x46] == 2) {
            return 1;
        }
        p[0x46] = 0xfe;
    } else {
        _padFuncClrInfo();
    }
    return 0;
}
