// FUNC 800e8c24 52 O0PTSUB0
// MATCHING 800e8c24 52
extern unsigned char D_1F8001CF;
extern void screenAdjustDrawText(void);
extern void optionsDrawFrame(void);

void screenAdjustMenuDraw(void)
{
    D_1F8001CF = 1;
    screenAdjustDrawText();
    optionsDrawFrame();
}
