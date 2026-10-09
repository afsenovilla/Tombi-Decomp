// FUNC 80068f50 44 MAIN0
// MATCHING 80068f50 44
// Portado de psx_tomba (psyq/libetc/intr_dma.c, DMA_memclr); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

typedef void (*Callback)();

Callback setIntrDMA(int index, Callback callback);
void trapIntrDMA(void);
void DMA_memclr(int* ptr, int size);

extern char* D_80016264[];
extern char* D_80016280[];
extern volatile unsigned long* D_800974DC;
extern Callback D_800974E0[8];
extern unsigned long* D_80097500;

void* startIntrDMA(void);

void trapIntrDMA(void);

Callback setIntrDMA(int index, Callback callback);

void DMA_memclr(int* ptr, int size) {
    while (size--) {
        *ptr++ = 0;
    }
}
