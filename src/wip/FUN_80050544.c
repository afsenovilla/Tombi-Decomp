// FUNC 80050544 172 MAIN0
extern unsigned char DAT_800a6039;
extern unsigned char DAT_800a603a;
extern unsigned char DAT_800a6038[];
extern void FUN_800ea124(void);
extern void FUN_800505f0(void *);
extern void FUN_800eb714(void);
extern void FUN_800ead48(void);
void FUN_80050544(void)
{
    if (DAT_800a6039 != 0) {
        switch (DAT_800a603a) {
        case 0:
            FUN_800505f0(DAT_800a6038);
            break;
        case 1:
            FUN_800ea124();
            break;
        case 2:
            FUN_800eb714();
            break;
        case 3:
            FUN_800ead48();
            break;
        }
    }
}
