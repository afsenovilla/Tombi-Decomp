// FUNC 80126ca4 96 X000
// MATCHING 80126ca4 96
// VERSION SLPM_80165 x00 8011CDB4 116
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 80126CA4 (pushCat2ObjectOutOfCat4Type00.c): clears scratchpad 0x1F80019E on a hit.
typedef struct A { char pad[2]; unsigned char b; char pad2[0x1c-3]; unsigned char c; } A;
typedef struct B { char pad[0x94]; int f; } B;
extern short pushObjectOutOfObjectFacingB(void);
extern short FUN_800482ec(void);
extern short D_1F80019E;
void pushCat2ObjectOutOfCat4Type00(A *a, B *b)
{
    short r;
    if (b->f == 0) {
        if (a->c == 2 && a->b == 2) r = pushObjectOutOfObjectFacingB();
        else r = FUN_800482ec();
        if (r) D_1F80019E = 0;
    }
}
#else
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
#endif
