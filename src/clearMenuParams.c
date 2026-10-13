// FUNC 80018280 44 MAIN0
// MATCHING 80018280 44
extern char DAT_800a45d8[];
extern void memset();

void clearMenuParams(void)
{
    memset(DAT_800a45d8, 0, 0x70);
}
