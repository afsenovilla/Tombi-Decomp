// FUNC 8002dcf0 44 MAIN0
// MATCHING 8002dcf0 44
extern void openMessageBox(int a, int b, int c, int d, int e);

void showMessageBoxDirectTimed(int a, int b, int c, short d)
{
    openMessageBox(a, b, c, 1, d);
}
