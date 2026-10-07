// FUNC 80028420 72 MAIN0
// MATCHING 80028420 72
typedef struct S { char pad[0x24]; int v; char pad2[0x71-0x28]; char a, b, c; } S;
int FUN_80028420(S *o)
{
if (o->v != 0) {
    if (o->v > 0) o->v -= 0x80; else o->v += 0x80;
    return 0;
}
o->a = 0; o->b = 0; o->c = 0;
return 1;
}
