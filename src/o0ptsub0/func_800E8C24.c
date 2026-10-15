// FUNC 800e8c24 52 O0PTSUB0
// MATCHING 800e8c24 52
extern unsigned char D_1F8001CF;
extern void func_800E950C(void);
extern void func_800E96A4(void);

void func_800E8C24(void)
{
    D_1F8001CF = 1;
    func_800E950C();
    func_800E96A4();
}
