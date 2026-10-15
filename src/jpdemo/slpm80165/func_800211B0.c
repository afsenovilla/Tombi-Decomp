// JPDEMO slpm80165 800211B0 84
// Demo-only helper: sets bit n of the current area's flag words (GAME + 0x34, 8 words per area).
typedef struct { unsigned short area; char p[0x32]; int flags[1][8]; } G;
extern G D_8009A810;
void func_800211B0(int n)
{
    int w = n / 32;
    int b = n - w * 32;
    D_8009A810.flags[D_8009A810.area][w] |= 1 << b;
}
