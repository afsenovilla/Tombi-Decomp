// FUNC 800eebc4 124 X002
// MATCHING 800eebc4 124
typedef struct S { char pad[0x24]; void *anim; char pad2[0x8c - 0x28]; int v; } S;
extern unsigned short DAT_8009c960;
extern short DAT_8009c944;
extern char DAT_800112e0[];
extern char DAT_80010ae8[];
extern void func_8001fe6c(S *s);
extern void func_8001fe94(S *s, int n);

void FUN_800eebc4(S *s)
{
    if (DAT_8009c960 == 3 && DAT_8009c944 != 0) {
        s->anim = DAT_800112e0;
        func_8001fe6c(s);
    } else {
        s->anim = DAT_80010ae8;
        func_8001fe94(s, 3);
    }
    s->v = 0;
}
