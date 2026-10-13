// FUNC 8004f38c 48 MAIN0
// MATCHING 8004f38c 48
extern int WSS_LOAD_LISTS[];
extern void FUN_8004f538(int);
void loadWssFile(int a, int b)
{
    FUN_8004f538(WSS_LOAD_LISTS[b]);
}
