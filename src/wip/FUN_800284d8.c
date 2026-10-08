// FUNC 800284d8 112 MAIN0
typedef struct S { char pad[0x20]; int v; char pad2[0x6d - 0x24]; signed char m; char pad3; signed char t; } S;

int FUN_800284d8(S *s)
{
    switch (s->m) {
    case 0:
        if ((s->t << 8) < s->v) {
            s->v = s->v - 0x100;
            return 0;
        }
        return 1;
    case 1:
        if (s->v < (s->t << 8)) {
            s->v = s->v + 0x100;
            return 0;
        }
        return 1;
    default:
        return 0;
    }
    return 1;
}
