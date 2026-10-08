// FUNC 8011fad4 228 X003
// MATCHING 8011fad4 228
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern void FUN_8001f96c(int, int, int, int);

void func_8011FAD4(char *a, char *b)
{
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 0x2d) >= 0x5b)
        return;
    if ((unsigned short)((U16(PTR(a, 0x40), 2) - U16(PTR(b, 0x40), 2)) + (U16(b, 0x6c) + U16(a, 0x6c))) > S16(b, 0x6e) + S16(a, 0x6e))
        return;
    if ((unsigned short)((U16(a, 0x16) - U16(b, 0x16)) + (U16(a, 0x70) + U16(b, 0x70))) > S16(a, 0x72) + S16(b, 0x72))
        return;
    b[0] = 2;
    b[0x6a] = 1;
    FUN_8001f96c(1, S16(a, 0x12), S16(a, 0x16), S16(a, 0x1a));
}
