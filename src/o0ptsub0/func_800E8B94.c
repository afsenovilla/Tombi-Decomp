// FUNC 800e8b94 76 O0PTSUB0
// MATCHING 800e8b94 76
extern unsigned char D_1F8001CF;
extern unsigned char D_8009D67C, D_8009F0E8;
extern void func_800E8E44(void);
extern void func_800E90D0(int a, int b);
extern void func_800E96A4(void);

void func_800E8B94(void)
{
    D_1F8001CF = 1;
    func_800E8E44();
    func_800E90D0(D_8009D67C, D_8009F0E8);
    func_800E96A4();
}
