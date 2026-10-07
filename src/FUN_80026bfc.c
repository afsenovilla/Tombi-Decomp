// FUNC 80026bfc 84 MAIN0
// MATCHING 80026bfc 84
extern char DAT_800b146c[];
extern char *FUN_80018448(void);
void FUN_80026bfc(int idx, char val)
{
char *p;
DAT_800b146c[idx] = val;
p = FUN_80018448();
if (p != 0) {
    p[0] = 1;
    p[2] = 0x20;
    p[0xc] = idx;
}
}
