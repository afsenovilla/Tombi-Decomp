// FUNC 80106f98 84 X003
// MATCHING 80106f98 84
extern unsigned short DAT_8009c960;
extern void playerStepDizzy(void);
extern void FUN_8011dae0(void);
void playerStep3CByArea(void)
{
switch (DAT_8009c960) {
case 3: playerStepDizzy(); break;
case 4: FUN_8011dae0(); break;
}
}
