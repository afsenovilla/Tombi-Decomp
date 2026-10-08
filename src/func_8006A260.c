// FUNC 8006a260 16 MAIN0
// MATCHING 8006a260 16
typedef struct { char pad[0x2c]; int d2c; char pad2[6]; unsigned char b36; unsigned char b37; } S;

void func_8006A260(S *p, int a, int b, int c)
{
    p->b37 = a;
    p->d2c = b;
    p->b36 = c;
}
