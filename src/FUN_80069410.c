// FUNC 80069410 72 MAIN0
/* score 3: only the epilogue differs (game: jr $ra with addiu $sp in the delay slot after s-reg restores; library code from another toolchain). Tried -O1/-O3/-fno-delayed-branch/-fomit-frame-pointer: no change or worse. Unreproducible with CC1PSX 4.3/ASPSX 2.86 per guide. */
extern int (*DAT_800981c0)(void);
extern int FUN_8006a254(int a, int b, int c);

int FUN_80069410(int a, int b, int c)
{
    return FUN_8006a254(DAT_800981c0(), b, c);
}
