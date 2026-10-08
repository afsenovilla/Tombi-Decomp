// FUNC 800ee25c 72 X003
// MATCHING 800ee25c 72
extern unsigned short D_8009C960;
extern void FUN_80122118(void);
extern void func_8011C5CC(void);

void func_800EE25C(void)
{
    if (D_8009C960 == 0) {
        FUN_80122118();
    } else if (D_8009C960 == 9) {
        func_8011C5CC();
    }
}
