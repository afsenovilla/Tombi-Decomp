// FUNC 80038024 40 MAIN0
// MATCHING 80038024 40
extern int DAT_a[64];
typedef struct S { char pad[0x88]; char a; char pad2; short b; } S;
void FUN_80038024(S *o)
{
int i;
for (i = 63; i >= 0; i--) DAT_a[i] = 0;
o->a = 0;
o->b = 0;
}
