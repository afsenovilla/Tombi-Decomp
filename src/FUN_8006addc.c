// FUNC 8006addc 32 MAIN0
// MATCHING 8006addc 32
typedef struct S { char pad[0x24]; unsigned char c; char pad2[7]; void *p; char pad3[6]; unsigned char b; unsigned char a; } S;
/* layout: 0x24 c, 0x2c p, 0x36 b, 0x37 a */
void FUN_8006addc(S *s, int c)
{
    s->a = 0x4c;
    s->p = &s->c;
    s->c = c;
    s->b = 1;
}
