// FUNC 800310d0 388 MAIN0
// MATCHING 800310d0 388
typedef struct S { unsigned char a, p, b; } S;
extern S PLAYER;
extern unsigned char DAT_603a;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern unsigned char DAT_36c2;
extern void dispatchScreenEffect(void);
extern void updateObjectsLayer1(void);

void updateAreaScreenEffect(void)
{
    unsigned char v;
    S *p = &PLAYER;
    DAT_603a = 0;
    switch (GAME) {
    case 2:
        switch (GAME_SELECTED_SECTION) {
        case 0: DAT_603a = 1; break;
        case 3: DAT_603a = 3; break;
        }
        break;
    case 5:
    case 8:
        if (GAME_SELECTED_SECTION == 0 || GAME_SELECTED_SECTION == 2)
            p->b = 2;
        break;
    case 0x13:
        switch (GAME_SELECTED_SECTION) {
        case 0: DAT_603a = 1; break;
        case 2: DAT_603a = 2; break;
        }
        break;
    }
end:
    if ((GAME != 6 || GAME_SELECTED_SECTION != 0) && (GAME != 9 || GAME_SELECTED_SECTION < 6)) {
        if (p->a != 0)
            dispatchScreenEffect();
        if (GAME_SELECTED_SECTION != 7 || DAT_36c2 == 0)
            updateObjectsLayer1();
    }
}
