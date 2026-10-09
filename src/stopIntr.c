// FUNC 8006897c 172 MAIN0
// MATCHING 8006897c 172
// Portado de psx_tomba (psyq/libetc/intr.c, stopIntr); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

#define JB_SP 1
extern int setjmp(jmp_buf);
long long startIntrDMA();
void* startIntrVSync();
void trapIntr();
static void memclr(s32* mem, int len);
void memclr(s32* mem, int len);

typedef int jmp_buf[12];
struct Intr {
    u16 unk0;
    u16 isCbContext;
    s16 D_800B5FFC[24];
    s32 D_800B602C;
    jmp_buf env;
    s32 stack[0x400];
};
extern struct Intr D_800970B4;
extern int VSyncCallback(void (*f)());
struct intr {
    const char* ver;
    void (*cb)();
    void (*set)(int arg0, void (*cb)(void));
    int (*start)();
    int (*stop)();
    int (*unk14)(int, void (*f)());
    int (*restart)();
    short* unk1C;
};
extern struct intr* D_800974A0;
int VSyncCallback(void (*func)());
extern volatile u16* D_80098144;
extern volatile s32* D_80098148;
void HookEntryInt(jmp_buf env);
extern volatile u16* D_80098140;

int ResetCallback(void);

void InterruptCallback(int arg0, void (*cb)(void));

void DMACallback(void);

int VSyncCallback(void (*func)());

int VSyncCallbacks(int n, void (*func)());

int StopCallback(void);

int RestartCallback(void);

int CheckCallback();

int GetIntrMask();

int SetIntrMask(int mask);

void* startIntr(void);

INCLUDE_RODATA("asm/scus_942.36/nonmatchings/psyq/libetc/intr", D_800161F8);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/intr", trapIntr);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/intr", setIntr);

u16* stopIntr(void) {
    if (D_800970B4.unk0) {
        EnterCriticalSection();
        D_800970B4.D_800B5FFC[0x17] = *D_80098144;
        D_800970B4.D_800B602C = *D_80098148;
        *D_80098140 = *D_80098144 = 0;
        *D_80098148 &= 0x77777777;
        ResetEntryInt();
        if (D_80098148 && D_80098148) {
        }
        D_800970B4.unk0 = 0;
        return &D_800970B4.unk0;
    }
    return NULL;
}

u16* restartIntr(void);

void memclr(s32* mem, int len);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/intr", func_80068534);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/intr", _96_remove);

INCLUDE_RODATA("asm/scus_942.36/nonmatchings/psyq/libetc/intr", D_80016264);

INCLUDE_RODATA("asm/scus_942.36/nonmatchings/psyq/libetc/intr", D_80016280);

INCLUDE_RODATA("asm/scus_942.36/nonmatchings/psyq/libetc/intr", jtbl_80016290);
