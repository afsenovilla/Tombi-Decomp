// FUNC 800f183c 108 D0SPSUB
// MATCHING 800f183c 108
typedef struct { short w0; unsigned short w2; } Obj;
extern unsigned short D_8009C960, D_8009C962;
extern unsigned char D_8009CDE4;
extern Obj *D_800A6078;

int func_800F183C(void)
{
    int r = 0;
    if (D_8009C960 == 0x10 && D_8009C962 == 6 && D_8009CDE4 != 0 && D_8009CDE4 != 0xFF)
    {
        unsigned int t = D_800A6078->w2 - 0x64;
        r = t < 0x78;
    }
    return r;
}
