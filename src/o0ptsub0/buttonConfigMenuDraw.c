// FUNC 800e8be0 68 O0PTSUB0
// MATCHING 800e8be0 68
extern unsigned char D_1F8001CF;
extern short D_8009F3DC;
extern void buttonConfigDrawCursor(void);
extern void buttonConfigDrawText(int a);
extern void optionsDrawFrame(void);

void buttonConfigMenuDraw(void)
{
    D_1F8001CF = 1;
    buttonConfigDrawCursor();
    buttonConfigDrawText(D_8009F3DC);
    optionsDrawFrame();
}
