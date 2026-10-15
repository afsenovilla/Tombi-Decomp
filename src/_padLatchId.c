// FUNC 8006b720 16 MAIN0
// MATCHING 8006b720 16
typedef struct S { char pad[0x37]; unsigned char a; unsigned char b; } S;

void _padLatchId(S *s)
{
    unsigned char t = s->a;
    s->a = 0;
    s->b = t;
}
