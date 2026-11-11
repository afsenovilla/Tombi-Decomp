// VERSION BETA_980203 x00 800F9768 60
#if defined(VERSION_BETA_980203)
// Beta version: written from the disc. Calls the voice allocator 0x8001F72C (3, a), then copies byte 0xC of the object at 0x800A4BCC to byte 0xA of the one at 0x800A1E10.
extern void func_8001F72C(int, int);
extern unsigned char *D_800A4BCC;
extern unsigned char *D_800A1E10;

void func_800F9768_beta980203(short a)
{
    func_8001F72C(3, a);
    D_800A1E10[0xa] = D_800A4BCC[0xc];
}
#endif
