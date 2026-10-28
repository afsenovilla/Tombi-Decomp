// FUNC 8005dbc0 304 MAIN0
// MATCHING 8005dbc0 304
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libpress/libpress.c, timeout); licencia MIT del proyecto original.
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

void MDEC_reset(int mode);

void MDEC_in(u_long* buf, int size);

void MDEC_out(u_long* buf, int size);

int MDEC_in_sync(void);

int MDEC_out_sync(void);

u_long MDEC_status(void);

static inline void print_status(void) {
    u_long stat;

    stat = *mdec1;
    printf("	 DMA=(%d,%d), ADDR=(0x%08x->0x%08x)
", (*d0_chcr >> 24) & 1,
           (*d1_chcr >> 24) & 1, *d0_madr, *d1_madr);
    printf("	 FIFO=(%d,%d),BUSY=%d,DREQ=(%d,%d),RGB24=%d,STP=%d
",
           ~stat >> 31, (stat >> 30) & 1, (stat >> 29) & 1, (stat >> 28) & 1,
           (stat >> 27) & 1, (stat >> 25) & 1, (stat >> 23) & 1);
}

static int timeout(char* arg0) {
    printf("%s timeout:
", arg0);
    print_status();
    *mdec1 = 0x80000000;
    *d0_chcr = 0;
    *d1_chcr = 0;
    *d1_chcr;
    *mdec1 = 0x60000000;
    return 0;
}
