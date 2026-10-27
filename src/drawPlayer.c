// FUNC 80050544 172 MAIN0
// MATCHING 80050544 172
extern unsigned char PLAYER_VISIBLE[];
extern unsigned char PLAYER_TYPE[];
extern unsigned char PLAYER[];
extern void FUN_800ea124(void *);
extern void drawPlayerSpriteDefault(void *);
extern void FUN_800eb714(void *);
extern void FUN_800ead48(void *);
void drawPlayer(void)
{
    unsigned char *p = PLAYER;
    if (PLAYER_VISIBLE[0] != 0) {
        switch (PLAYER_TYPE[0]) {
        case 0:
            drawPlayerSpriteDefault(p);
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
