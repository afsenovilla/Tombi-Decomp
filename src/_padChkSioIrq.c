// FUNC 800695a8 104 MAIN0
// MATCHING 800695a8 104
extern int *PAD_INTR;
extern void (*_padFuncIntGun)(void);
int _padChkSioIrq(void)
{
    if (!(PAD_INTR[1] & 1)) return 0;
    if (!(PAD_INTR[0] & 1)) return 0;
    if (_padFuncIntGun != 0) _padFuncIntGun();
    return 1;
}
