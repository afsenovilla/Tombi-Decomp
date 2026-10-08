// FUNC 80024e58 72 MAIN0
// MATCHING 80024e58 72
extern int FUN_8002235c();
extern void FUN_80023084();

void FUN_80024e58(int *a, int b, int c)
{
    int r;
    a++;
    r = FUN_8002235c(c, a, a + 1, b);
    FUN_80023084(r + 0x14, r + 0x18, b);
}
