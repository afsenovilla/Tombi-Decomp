// FUNC 80069390 56 MAIN0
/* score 3: epilogue only (jr ra; addiu sp in delay slot with s0 saved). Bytes MATCH with Psy-Q 4.4 CC1PSX (CC1_WINE=/opt/psyq/cc44/CC1PSX.EXE matchcheck) and with ncheck OLDGCC=gcc-2.8.1-psx: library-range code built with a newer compiler, not reproducible with CC1PSX 4.3. */
extern void *(*DAT_800981c0)(void);
extern int FUN_8006ab10(void *, int);
int FUN_80069390(int a, int b)
{
    return FUN_8006ab10((*DAT_800981c0)(), b);
}
