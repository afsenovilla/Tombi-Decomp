// JPDEMO x00 8011CDB4 116
// Demo version of PAL x000 80126CA4 (FUN_80126ca4.c): clears scratchpad 0x1F80019E on a hit.
typedef struct A { char pad[2]; unsigned char b; char pad2[0x1c-3]; unsigned char c; } A;
typedef struct B { char pad[0x94]; int f; } B;
extern short FUN_8004886c(void);
extern short FUN_800482ec(void);
extern short D_1F80019E;
void FUN_80126ca4(A *a, B *b)
{
    short r;
    if (b->f == 0) {
        if (a->c == 2 && a->b == 2) r = FUN_8004886c();
        else r = FUN_800482ec();
        if (r) D_1F80019E = 0;
    }
}
