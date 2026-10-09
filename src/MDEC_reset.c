// FUNC 8005d858 252 MAIN0
// MATCHING 8005d858 252
// Portado de psx_tomba (psyq/libpress/libpress.c, MDEC_reset); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

extern u_int volatile* d1_madr;
extern u_int volatile* d1_bcr;

extern u_int volatile* d0_madr;
extern u_int volatile* d0_bcr;
extern u_int volatile* d0_chcr;
extern u_int volatile* mdec0;
extern u_int volatile* d_pcr;

extern u_int volatile* d1_chcr;
static int timeout(char* arg0);
int timeout(char* arg0);

extern volatile u_long* mdec1;

typedef struct {
    u_char iq_y[64];
    u_char iq_c[64];
    short dct[64];
} DECDCTENV;
extern u32 mdec_iq[];
extern u32 mdec_coef[];
void MDEC_in(u_long* buf, int size);
int ResetCallback(void);
void MDEC_reset(int mode);
extern int DecDCTinCallback(void (*func)());
extern int DecDCToutCallback(void (*func)());
int DecDCToutCallback(void (*cb)());
int MDEC_in_sync(void);
u_long MDEC_status(void);
void MDEC_out(u_long* buf, int size);
int DecDCTinCallback(void (*cb)());
int MDEC_out_sync(void);

void DecDCTReset(int mode);

DECDCTENV* DecDCTGetEnv(DECDCTENV* env);

DECDCTENV* DecDCTPutEnv(DECDCTENV* env);

int DecDCTBufSize(u_long* bs);

void DecDCTin(u_long* buf, int mode);

void DecDCTout(u_long* buf, int size);

int DecDCTinSync(int mode);

int DecDCToutSync(int mode);

int DecDCTinCallback(void (*cb)());

int DecDCToutCallback(void (*cb)());

void MDEC_reset(int mode) {
    switch (mode) {
    case 0:
        *mdec1 = 0x80000000;
        *d0_chcr = 0;
        *d1_chcr = 0;
        *mdec1 = 0x60000000;
        MDEC_in((u_long*)mdec_iq, 32);
        MDEC_in((u_long*)mdec_coef, 32);
        return;
    case 1:
        *mdec1 = 0x80000000;
        *d0_chcr = 0;
        *d1_chcr = 0;
        *d1_chcr;
        *mdec1 = 0x60000000;
        return;
    default:
        printf("MDEC_rest:bad option(%d)\n", mode);
        return;
    }
}

void MDEC_in(u_long* buf, int size);

void MDEC_out(u_long* buf, int size);

int MDEC_in_sync(void);

int MDEC_out_sync(void);

u_long MDEC_status(void);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libpress/libpress", timeout);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libpress/libpress", func_8005D748);
