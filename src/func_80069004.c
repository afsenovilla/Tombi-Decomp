// FUNC 80069004 76 MAIN0
// MATCHING 80069004 76
typedef struct { unsigned char pad[0xe8]; unsigned char e8; unsigned char pad2[0xf0 - 0xe9]; } E;
extern int D_800981F8;
extern E *D_800981E0;
int func_80069004(int a)
{
    if (D_800981F8 != 0) {
        return D_800981E0[a >> 4].e8 == 8;
    }
    return 0;
}
