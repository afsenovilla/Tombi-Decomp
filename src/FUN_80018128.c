// FUNC 80018128 44 MAIN0
extern void memfill(void *p, int v, int n);
extern char DAT_800a4550[];

void FUN_80018128(void)
{
    memfill(DAT_800a4550, 0, 0x84);
}
