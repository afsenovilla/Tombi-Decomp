// FUNC 80026a10 492 MAIN0
// MATCHING 80026a10 492
// Portado de psx_tomba (inventory.c, addPlayerAP); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

void addPlayerAP(int arg0) {
    u_long* ptr;
	char *mytemp;
    int i = 0;
    
    do
    {
        if ((GAME.playerAP < D_8007C290[i] && (D_8007C290[i] <= (GAME.playerAP + arg0)))) {
            GAME.unk13 = (i + 2);
            GAME.playerLives += 3;
            if (GAME.playerLives > 99) {
                GAME.playerLives = 99;
            }
        }
        i++;
    } while(D_8007C290[i] != -1); 
    GAME.playerAP += arg0;
    mytemp = D_800B07AC;
    *mytemp++ = (GAME.playerAP / 10000000) % 10;
    *mytemp++ = (GAME.playerAP / 1000000 ) % 10;
    *mytemp++ = (GAME.playerAP / 100000  ) % 10;
    *mytemp++ = (GAME.playerAP / 10000   ) % 10;
    *mytemp++ = (GAME.playerAP / 1000    ) % 10;
    *mytemp++ = (GAME.playerAP / 100     ) % 10;
    *mytemp++ = (GAME.playerAP / 10      ) % 10;
    *mytemp++ = (GAME.playerAP / 1       ) % 10;
    return;
}
