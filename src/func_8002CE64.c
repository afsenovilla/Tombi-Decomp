// FUNC 8002ce64 220 MAIN0
// MATCHING 8002ce64 220
extern unsigned char D_800A603A;
extern unsigned char D_8009D00F;
extern unsigned short D_8009C960;
extern short D_800A6066;
void func_800EEA7C(unsigned char *p, int a, int b);
void func_800E8A98(unsigned char *p, int a, int b);
void func_800E96E8(unsigned char *p, int a, int b);
void func_800E940C(unsigned char *p, int a, int b);
void func_8002CE64(void)
{
    unsigned char *p = &D_800A603A;
    D_8009D00F = 0;
    switch (*p) {
    case 0:
        if (D_8009C960 == 9) func_800EEA7C(p - 2, 0, 0);
        break;
    case 1:
        func_800E8A98(p - 2, 0, D_800A6066);
        break;
    case 2:
        func_800E96E8(p - 2, 0, D_800A6066);
        break;
    case 3:
        func_800E940C(p - 2, 0, D_800A6066);
        break;
    }
}
