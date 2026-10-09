// FUNC 8006662c 492 MAIN0
// MATCHING 8006662c 492
// Portado de psx_tomba (psyq/libcd/bios.c, CD_init); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"
// #include "psyq/libetc.h"

typedef struct {                          
    unsigned char sync;  // sync state    
    unsigned char ready; // ready state   
    unsigned char c;                      
} CdlIntr;  

typedef struct Result_t {
    int unk0;
    int unk4;
} Result_t;

extern CdlCB (*CD_CBREADY)(u_char, u_char*); 
extern CdlCB (*CD_CBSYNC)(u_char, u_char*);  

INCLUDE_RODATA("asm/scus_942.36/nonmatchings/psyq/libcd/bios", D_80015F34);

extern int CD_DEBUG; // CD_DEBUG
extern u_char CD_COM;
extern u_char CD_MODE;
extern int CD_STATUS;
extern int CD_STATUS1;
extern int D_80096004;
extern CdlLOC CD_POS;
extern const char* CD_COMSTR[]; // CD_COMSTR
extern const char* CD_INTSTR[];
extern const char* D_800960B0[];
extern int D_80096130[]; 
extern const char* D_800961B0[];
extern int D_80096230[]; // CD_COMATTR
extern int D_80096294;
extern volatile u_char* D_800962B4;
extern volatile u_char* D_80096F54;
extern volatile int* D_80096F5C;
extern void* D_800962C4;
extern volatile CdlIntr D_80096F64; // CD_INTR
extern void* D_80096F68[];
extern volatile unsigned char* D_80096F4C;
extern s32* D_800962E4;
extern s32* D_800962E8;
extern s32* D_800962EC;
extern s32* D_800962F0;
extern volatile s32* D_800962F4;
extern char D_8009B2A8[];
extern char D_8009B2B0[];
extern volatile u_char* D_80096F58;
extern char D_8009B2B8[];
extern int D_8009B2C0;
extern int D_8009B2C4; // timeout
extern char* D_8009B2C8[];

void CD_flush(void);
void callback(void);

static inline void rescpy(void* _dst, void* _src) {
    char *pDst = (char*)_dst;
    char *pSrc = (char*)_src;
    u32 _size = sizeof(Result_t);

    if (pDst == 0) return;
    
    while (_size--) {
        *pDst++ = *pSrc++;
    }
}

static inline int get_alarm(void) {
    if ((D_8009B2C0 < VSync(-1)) || D_8009B2C4++ > 0x3C0000) {
        puts("CD timeout: ");
        
        printf("%s:(%s) Sync=%s, Ready=%s\n",
            D_8009B2C8[0],
            CD_COMSTR[CD_COM],
            CD_INTSTR[D_80096F64.sync],
            CD_INTSTR[D_80096F64.ready]);
        CD_flush();
        return -1;
    } 
    return 0;
}

int getintr(void);

int CD_sync(int mode, unsigned char* result);

int CD_ready(int arg0, u_char* arg1);

int CD_cw(u_char arg0, u_char* arg1, u_char* arg2, int arg3);

int CD_vol(CdlATV* vol);

void CD_flush(void);

int CD_initvol(void);

void CD_initintr(void);

const char D_8001612C[] = "$Id: bios.c,v 1.81 1996/12/16 06:24:14 makoto Exp $";

int CD_init(void) {
    puts("CD_init:");
    printf("addr=%08x\n", &D_80096F68);
    CD_COM = 0;
    CD_MODE = 0;
    CD_CBREADY = 0;
    CD_CBSYNC = 0;
    CD_STATUS1 = 0;
    CD_STATUS = 0;
    ResetCallback();
    InterruptCallback(2, &callback);
    
    *D_80096F4C = 1;
    while (*D_80096F58 & 7) {
        *D_80096F4C = 1;
        *D_80096F58 = 7;
        *D_80096F54 = 7;
    }
    
    D_80096F64.ready = D_80096F64.c = 0;
    D_80096F64.sync = 2;
    
    *D_80096F4C = 0;
    *D_80096F58 = 0;
    *D_80096F5C = 0x1325; // COM_DELAY
    
    CD_cw(1, 0, 0, 0);
    if (CD_STATUS & 0x10) {
        CD_cw(1, 0, 0, 0);
    }
    if (CD_cw(0xA, 0, 0, 0) != 0) {
        return -1;
    }

    if (CD_cw(0xC, 0, 0, 0) != 0) {
        return -1;
    }

    if (CD_sync(0, 0) != 2) {
        return -1;
    }
    return 0;
}

int CD_datasync(int arg0);

int CD_getsector(int arg0, int arg1);

void CD_set_test_parmnum(int arg0);

void callback(void);
