// FUNC 800e8b94 76 O0PTSUB0
// MATCHING 800e8b94 76
extern unsigned char D_1F8001CF;
extern unsigned char D_8009D67C, D_8009F0E8;
extern void optionsMenuDrawCursor(void);
extern void optionsMenuDrawText(int a, int b);
extern void optionsDrawFrame(void);

void optionsMenuDraw(void)
{
    D_1F8001CF = 1;
    optionsMenuDrawCursor();
    optionsMenuDrawText(D_8009D67C, D_8009F0E8);
    optionsDrawFrame();
}
