// FUNC 800f5014 100 X003
// MATCHING 800f5014 100
typedef struct { char p[0xa]; char b0a; char b0b; char b0c; } S;
extern S *PA, *PB;
extern void g(int a, int b, int c, int d);
extern void h(int a, int b);

void FUN_800f5014(short a)
{
    g(0, 0, 0xff, 2);
    h(3, a);
    PB->b0a = PA->b0c;
}
