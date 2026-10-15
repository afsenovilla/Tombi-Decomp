// FUNC 800e8be0 68 O0PTSUB0
// MATCHING 800e8be0 68
extern unsigned char D_1F8001CF;
extern short D_8009F3DC;
extern void func_800E8FD8(void);
extern void func_800E9254(int a);
extern void func_800E96A4(void);

void func_800E8BE0(void)
{
    D_1F8001CF = 1;
    func_800E8FD8();
    func_800E9254(D_8009F3DC);
    func_800E96A4();
}
