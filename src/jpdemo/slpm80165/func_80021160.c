// JPDEMO slpm80165 80021160 80
// Demo-only helper: tests bit n of the current area's flag words (GAME + 0x34, 8 words per area).
typedef struct { unsigned short area; char p[0x32]; int flags[1][8]; } G;
extern G D_8009A810;
int func_80021160(int n)
{
    int w = n / 32;
    int b = n - w * 32;
    return D_8009A810.flags[D_8009A810.area][w] & (1 << b);
}
