// FUNC 80068c7c 44 MAIN0
// MATCHING 80068c7c 44
// Portado de psx_tomba (psyq/libetc/intr_vb.c, setIntrVSync); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

extern void InterruptCallback(int, void (*)());
void trapIntrVSync(void);
extern void setIntrVSync(int, void (*)(void));
void VSync_memclr(s32*, int);
extern volatile int Vcount;
extern int* D_800974D8;
void setIntrVSync(int index, void (*callback)(void));

extern void (*D_80098150[8])(void);

void* startIntrVSync(void);

void trapIntrVSync(void);

void setIntrVSync(int index, void (*callback)(void))
{
    if (callback != D_80098150[index]) {
        D_80098150[index] = callback;
    }
}

void VSync_memclr(s32* mem, int len);
