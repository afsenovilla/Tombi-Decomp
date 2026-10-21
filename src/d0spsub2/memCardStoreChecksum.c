// FUNC 800ea0d8 64 D0SPSUB2
// MATCHING 800ea0d8 64
int memCardStoreChecksum(unsigned char *p, int n)
{
    unsigned char x = 0;
    int i;
    for (i = 0; i < n - 1; i++) x ^= *p++;
    *p = x;
    return x;
}
