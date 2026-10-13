// FUNC 8002dcc8 40 MAIN0
// MATCHING 8002dcc8 40
extern void openMessageBox();

void showMessageBoxDirect(int a, int b, int c)
{
    openMessageBox(a, b, c, 1, -1);
}
