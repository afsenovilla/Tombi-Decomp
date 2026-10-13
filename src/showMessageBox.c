// FUNC 8002dc50 56 MAIN0
// MATCHING 8002dc50 56
extern void openMessageBox(int a, int b, short *c, int d, int e);

void showMessageBox(int a, int b, short c, short d)
{
    short s[6];
    s[1] = c;
    s[3] = d;
    s[5] = 0;
    openMessageBox(a, b, s, 0, -1);
}
