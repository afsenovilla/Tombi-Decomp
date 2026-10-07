// FUNC 80028b40 68 MAIN0
// MATCHING 80028b40 68
typedef struct S { char pad[0x20]; int v; char pad2[0x6e - 0x24]; char a; char b; } S;

int FUN_80028b40(S *s)
{
    int v = s->v;
    if (v != 0) {
        if (v > 0) {
            v -= 0x100;
        } else {
            v += 0x100;
        }
        s->v = v;
        return 0;
    } else {
        s->a = 0;
        s->b = 0;
        return 1;
    }
}
