// FUNC 80068ea8 168 MAIN0
// MATCHING 80068ea8 168
// Portado de psx_tomba (psyq/libetc/intr_dma.c, setIntrDMA); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

typedef void (*Callback)();

Callback setIntrDMA(int index, Callback callback);
void trapIntrDMA(void);
void DMA_memclr(int* ptr, int size);

extern char* D_80016264[];
extern char* D_80016280[];
extern volatile unsigned long* D_80098178;
extern Callback D_8009817C[8];
extern unsigned long* D_80097500;

void* startIntrDMA(void);

void trapIntrDMA(void);

Callback setIntrDMA(int index, Callback callback) {
    Callback prev = D_8009817C[index];
    if (callback != prev) {
        if (callback != 0) {
            D_8009817C[index] = callback;
            *D_80098178 = (*D_80098178 & 0xFFFFFF) | 0x800000 | ((1 << (index + 0x10)));
        } else {
            D_8009817C[index] = 0;
            *D_80098178 = ((*D_80098178 & 0xFFFFFF) | 0x800000) & ~(1 << (index + 0x10));
        }
    }
    return prev;
}

void DMA_memclr(int* ptr, int size);
