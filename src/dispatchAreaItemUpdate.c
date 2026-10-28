// FUNC 8003e918 72 MAIN0
// MATCHING 8003e918 72
extern unsigned short GAME;
extern void a00SpawnL5Type7Child(void);
extern void FUN_8011e4ec(void);

void dispatchAreaItemUpdate(void)
{
    if (GAME == 0) {
        a00SpawnL5Type7Child();
    } else if (GAME == 4) {
        FUN_8011e4ec();
    }
}
