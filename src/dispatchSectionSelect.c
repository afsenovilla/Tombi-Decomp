// FUNC 8002ab80 60 MAIN0
// MATCHING 8002ab80 60
extern unsigned short D_8009C962;
extern void func_800E9040(void);
extern void func_8011611C(void);

void dispatchSectionSelect(void)
{
    if (D_8009C962 == 0) {
        func_800E9040();
    } else {
        func_8011611C();
    }
}
