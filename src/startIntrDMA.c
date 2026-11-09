// FUNC 80068cd4 80 MAIN0
// MATCHING 80068cd4 80
// Ported from sotn-decomp src/main/psxsdk/libetc/intr_dma.c (MIT, (c) github.com/xeeynamo/sotn-decomp contributors), derived from Sony's PSY-Q libraries.
#define SKIP_ASM

#include "common.h"

typedef void (*Callback)();

Callback setIntrDMA(int index, Callback callback);
void trapIntrDMA(void);
void DMA_memclr(int* ptr, int size);

extern void InterruptCallback(int, void*);

/* port: the INTR_DMA.OBJ data is defined by src/trapIntrDMA.c (PAL names) */
extern volatile unsigned long* dma_dicr; /* DMA Interrupt Register pointer */
extern Callback dma_callbacks[8];
extern unsigned long* D_8009819C;

void* startIntrDMA(void) {
    DMA_memclr(&dma_callbacks, 8);
    *dma_dicr = 0;
    InterruptCallback(3, &trapIntrDMA);
    return &setIntrDMA;
}

void trapIntrDMA(void);

Callback setIntrDMA(int index, Callback callback);

void DMA_memclr(int* ptr, int size);
