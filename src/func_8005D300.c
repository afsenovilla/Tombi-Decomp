// FUNC 8005d300 48 MAIN0
// MATCHING 8005d300 48
extern void StopCARD2(void);
extern void _patch_card2(void);
extern void func_8005D510(void);
void func_8005D300(void)
{
    StopCARD2();
    _patch_card2();
    func_8005D510();
}
