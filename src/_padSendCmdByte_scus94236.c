// VERSION SCUS_94236 scus94236 8006A5E4 140
// CC gcc-2.8.1
#if defined(VERSION_SCUS_94236)
// USA version of PAL _padSendCmdByte.c (src/_padSendCmdByte.c): char pad0[0x37];=>char pad0[0x36];; char pad38[4];=>char pad38[5];; D_800981B4=>_padFuncGetTxd; _padSioExchangeByte=>func_8006979C_scus94236; return -4;=>return -9;.
typedef struct {
    char pad0[0x36];
    unsigned char b37;
    char pad38[5];
    unsigned char *p3c;
} S;
extern int (*_padFuncGetTxd)(S *, int);
extern int func_8006979C_scus94236(S *, int);

int _padSendCmdByte(S *s)
{
    int r;
    int a = 0;
    if ((*s->p3c >> 4) == 8) a = s->b37 == 0;
    r = func_8006979C_scus94236(s, (unsigned char)_padFuncGetTxd(s, a));
    if (r == 0x5a || r == 0 || r < 0) return r;
    return -9;
}
#endif
