// FUNC 80102a08 84 X016
// MATCHING 80102a08 84
// FLAGS -O2 -G0
extern unsigned short GAME;
extern void FUN_80122f54(void);
extern void FUN_8011ccd8(void);

void playerStep2EByArea(void)
{
    switch (GAME) {
    case 1:
        FUN_80122f54();
        break;
    case 10:
        FUN_8011ccd8();
        break;
    }
}
