// FUNC 80069610 400 MAIN0
/* diff: game has sw D_80098214 in the delay slot of the first beqz (lui at before branch); rest likely ok */
typedef struct { unsigned char pad[0xf0]; } E;
typedef struct { unsigned char pad[0xa]; unsigned short a; unsigned short c; unsigned short e; } H;
extern H *D_80098210;
extern int D_80098214;
extern int D_800981FC;
extern int D_80098200;
extern int D_8009BF88[];
extern int D_8009BF8C[];
extern int D_800981E4;
extern E *D_800981E0;
extern int D_800981F0;
extern int D_800981EC;
extern int D_800981F4;
extern void (*D_800981AC)(int);
int func_800698C4(E *e);
void func_80069BF8(E *e);
int func_80069610(void)
{
    int t;
    if (D_80098210->a & 2) {
        D_80098210->a = 0;
        return 0;
    }
    t = D_800981FC;
    D_80098214 = 1;
    if (t != 0 && D_8009BF88[0] < 150) D_8009BF88[0]++;
    if (D_80098200 == 0 && D_8009BF8C[0] < 150) D_8009BF8C[0]++;
    if (D_800981E4 != 0 && D_800981FC <= D_80098200) {
        D_800981F0 = 0;
        D_800981EC = D_800981FC;
        if (func_800698C4(&D_800981E0[D_800981FC]) == 0) D_800981AC(0xffff);
        D_800981F4 = 0;
        while (D_800981EC <= D_80098200) {
            func_80069BF8(&D_800981E0[D_800981EC]);
        }
        D_80098210->e = 0x88;
    }
    return 0;
}
