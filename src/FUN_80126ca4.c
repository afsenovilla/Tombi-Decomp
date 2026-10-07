// FUNC 80126ca4 96 X000
typedef struct A { char pad[2]; unsigned char b; char pad2[0x1c-3]; unsigned char c; } A;
typedef struct B { char pad[0x94]; int f; } B;
extern void FUN_8004886c(void);
extern void FUN_800482ec(void);
void FUN_80126ca4(A *a, B *b)
{
if (b->f == 0) {
    if (a->c == 2 && a->b == 2) FUN_8004886c();
    else FUN_800482ec();
}
}
