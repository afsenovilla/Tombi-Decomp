// FUNC 8011cd78 376 X006
// MATCHING 8011cd78 376
extern unsigned char D_8009C980[];
extern unsigned short *D_80012100[];
extern unsigned short D_800124F8;
extern void FUN_800594e4(void *, int, int, int);
extern void AddDrawMode(int, int);

void func_8011CD78(void *o, short x, short y)
{
    int n;
    if (D_8009C980[0] - 1 >= 10) {
        n = *D_80012100[(D_8009C980[0] - 1) / 10];
        FUN_800594e4(o, x - 5, y, n);
        n = *D_80012100[(D_8009C980[0] - 1) % 10];
        FUN_800594e4(o, x + 5, y, n);
    } else {
        n = *D_80012100[D_8009C980[0] - 1];
        FUN_800594e4(o, x, y, n);
    }

    FUN_800594e4(o, x, y - 4, D_800124F8);
    AddDrawMode(0x15, 1);
}
