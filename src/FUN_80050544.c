// FUNC 80050544 172 MAIN0
// MATCHING 80050544 172
extern unsigned char DAT_800a6039[];
extern unsigned char DAT_800a603a[];
extern unsigned char DAT_800a6038[];
extern void FUN_800ea124(void *);
extern void FUN_800505f0(void *);
extern void FUN_800eb714(void *);
extern void FUN_800ead48(void *);
void FUN_80050544(void)
{
    unsigned char *p = DAT_800a6038;
    if (DAT_800a6039[0] != 0) {
        switch (DAT_800a603a[0]) {
        case 0:
            FUN_800505f0(p);
            break;
        case 1:
            FUN_800ea124(p);
            break;
        case 2:
            FUN_800eb714(p);
            break;
        case 3:
            FUN_800ead48(p);
            break;
        }
    }
}
