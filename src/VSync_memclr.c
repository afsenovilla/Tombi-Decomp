// FUNC 80068ca8 44 MAIN0
// MATCHING 80068ca8 44
// Portado de psx_tomba (psyq/libetc/intr_vb.c, VSync_memclr); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

extern void InterruptCallback(int, void (*)());
void trapIntrVSync(void);
extern void setIntrVSync(int, void (*)(void));
void VSync_memclr(s32*, int);
extern volatile int Vcount;
extern int* D_800974D8;
void setIntrVSync(int index, void (*callback)(void));

extern void (*D_800974B4[8])(void);

void* startIntrVSync(void);

void trapIntrVSync(void);

void setIntrVSync(int index, void (*callback)(void));

void VSync_memclr(s32* mem, int len) {
    int i;
    for (i = len - 1; i != -1; i--) {
        *mem++ = 0;
    }
}
