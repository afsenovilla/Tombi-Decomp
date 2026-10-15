// FUNC 80024e58 72 MAIN0
// MATCHING 80024e58 72
extern int modelDrawFT4();
extern void modelDrawG4Lit();

void modelDrawFT4WithG4Lit(int *a, int b, int c)
{
    int r;
    a++;
    r = modelDrawFT4(c, a, a + 1, b);
    modelDrawG4Lit(r + 0x14, r + 0x18, b);
}
