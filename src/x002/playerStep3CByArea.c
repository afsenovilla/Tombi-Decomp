// FUNC 80106f98 84 X002
// MATCHING 80106f98 84
extern unsigned short GAME;
extern void FUN_8011e1a4(void);
extern void FUN_8011dae0(void);
void playerStep3CByArea(void)
{
switch (GAME) {
case 3: FUN_8011e1a4(); break;
case 4: FUN_8011dae0(); break;
}
}
