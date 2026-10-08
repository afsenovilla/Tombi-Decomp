// FUNC 8003a7e4 184 MAIN0
// MATCHING 8003a7e4 184
// Ported from psx_tomba (scriptop.c, scriptOpSetAreaConfig); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetAreaConfig(void)
{
    unkstruct_8009E458* p = D_8009E458;
    s32 a = *(s32*)((u8*)p + 0x1190);
    s32 b;

    D_800A539C = a;
    b = *(s32*)((u8*)p + 0x1194);
    D_800A539E = 0;
    D_800A539D = b;
    switch ((u8)a) {
    case 1:
        D_8009BCA7 = 0;
        D_800A5398[0] = 1;
        D_800A5436 = 0;
        break;
    case 4:
        switch ((u8)b) {
        case 2:
            D_800A544A = *(s32*)((u8*)p + 0x11A0);
        case 1:
            D_800A53C6 = *(s32*)((u8*)p + 0x1198);
            D_800A53B8 = *(s32*)((u8*)p + 0x119C);
            break;
        }
        D_8009BCA7 = 1;
        break;
    }
}
