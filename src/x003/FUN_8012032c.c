// FUNC 8012032c 96 X003
// MATCHING 8012032c 96
typedef struct A { char pad[2]; unsigned char b; char pad2[0x1c-3]; unsigned char c; } A;
typedef struct B { char pad[0x94]; int f; } B;
extern void FUN_8004886c(void);
extern void FUN_800482ec(void);
void FUN_8012032c(A *a, B *b)
{
if (b->f == 0) {
    if (a->c == 2 && a->b == 2) FUN_8004886c();
    else FUN_800482ec();
}
}
