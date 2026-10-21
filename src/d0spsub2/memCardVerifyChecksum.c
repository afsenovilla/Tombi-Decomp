// FUNC 800ea118 72 D0SPSUB2
// MATCHING 800ea118 72
int memCardVerifyChecksum(unsigned char *p, int n)
{
    unsigned char x = 0;
    int i;
    for (i = 0; i < n - 1; i++) x ^= *p++;
    return *p == x;
}
