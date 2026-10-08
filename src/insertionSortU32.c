// FUNC 80020cb0 112 MAIN0
// MATCHING 80020cb0 112
// Ported from psx_tomba (entity.c, insertionSortU32); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void insertionSortU32(s32 n, u32* arr)
{
    s32 i;
    s32 j;
    u32 key;

    for (i = 1; i < n; i++) {
        key = arr[i];
        for (j = i - 1; j >= 0 && key < arr[j]; j--) {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = key;
    }
}
