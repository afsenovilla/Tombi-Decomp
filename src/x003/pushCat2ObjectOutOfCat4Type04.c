// FUNC 8012032c 96 X003
// MATCHING 8012032c 96
typedef struct A { char pad[2]; unsigned char b; char pad2[0x1c-3]; unsigned char c; } A;
typedef struct B { char pad[0x94]; int f; } B;
extern void pushObjectOutOfObjectFacingB(void);
extern void pushObjectOutOfObjectBoxB(void);
void pushCat2ObjectOutOfCat4Type04(A *a, B *b)
{
if (b->f == 0) {
    if (a->c == 2 && a->b == 2) pushObjectOutOfObjectFacingB();
    else pushObjectOutOfObjectBoxB();
}
}
