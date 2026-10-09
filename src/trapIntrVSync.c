// FUNC 80068c04 120 MAIN0
// MATCHING 80068c04 120
// Portado de psx_tomba (psyq/libetc/intr_vb.c, trapIntrVSync); licencia MIT del proyecto original.
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

void trapIntrVSync(void) {
    void (**cb)();
    s32 i;

    i = 0;
    cb = D_80098150;
    Vcount += 1;
    (void)Vcount;
    do {
        if (cb[i] != 0) {
            cb[i]();
        }
        i += 1;
    } while (i < 8);
}

void setIntrVSync(int index, void (*callback)(void));

void VSync_memclr(s32* mem, int len);
