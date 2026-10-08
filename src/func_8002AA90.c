// FUNC 8002aa90 120 MAIN0
// MATCHING 8002aa90 120
extern unsigned short D_8009C962;
extern void func_800E8C98(void);
extern void func_80115EA8(void);

void func_8002AA90(void)
{
    switch (D_8009C962) {
    case 0:
    case 2:
        func_800E8C98();
        break;
    case 1:
    case 3:
        func_80115EA8();
        break;
    }
}
