// FUNC 80068d24 388 MAIN0
// MATCHING 80068d24 388
// Portado de psx_tomba (psyq/libetc/intr_dma.c, trapIntrDMA); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

typedef void (*Callback)();

Callback setIntrDMA(int index, Callback callback);
void trapIntrDMA(void);
void DMA_memclr(int* ptr, int size);

extern char* D_800162A8[];
extern char* D_800162C4[];
extern volatile unsigned long* D_80098178;
extern Callback D_8009817C[8];
extern unsigned long* D_8009819C;

void* startIntrDMA(void);

void trapIntrDMA(void) {
    u32 mask;
    int i;

    while((mask = ((u32) *D_80098178 >> 24) & 0x7f) != 0) {
        for (i = 0; mask != 0 && i < 7; i++, mask >>= 1) {
            if (mask & 1) {
                *D_80098178 &= (0xffffff | (1 << (i + 24)));
                if (D_8009817C[i] != 0) {
                     D_8009817C[i]();
                }
            }
        }
    }
    if (((*D_80098178 & 0xFF000000) == 0x80000000) || (*D_80098178 & 0x8000)) {
        printf(D_800162A8, *D_80098178);
        for (i = 0; i < 7; i++) {
            printf(D_800162C4, i, D_8009819C[4 * i]);
        }
    }
}

Callback setIntrDMA(int index, Callback callback);

void DMA_memclr(int* ptr, int size);
