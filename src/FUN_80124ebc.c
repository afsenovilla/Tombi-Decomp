// FUNC 80124ebc 64 X000
// MATCHING 80124ebc 64
typedef struct S { char pad[0x94]; int f; } S;
extern void FUN_8004306c(void);
extern void FUN_80043c74(void);
void FUN_80124ebc(int a, S *b)
{
if (b->f == 0) FUN_8004306c(); else FUN_80043c74();
}
