// VERSION PCPX_96084 tomba 8009270C 52
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
// Byte copy of the Psy-Q library (the shape of GPU_memset, src/GPU_memset.c); the empty array gives the 8-byte frame.
void func_8009270C_pcpx96084(unsigned char *d, unsigned char *s, int n)
{
    char pad[0];
    int i;
    for (i = n - 1; i != -1; i--) {
        *d++ = *s++;
    }
}
#endif
