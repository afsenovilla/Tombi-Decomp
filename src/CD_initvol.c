// FUNC 800664e8 244 MAIN0
// MATCHING 800664e8 244
// Portado de psx_tomba (psyq/libcd/bios.c, CD_initvol); licencia MIT del proyecto original.
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
extern volatile u_char* D_80096F50;
extern volatile u_char* D_80096F54;
extern volatile int* D_800962C0;
extern void* D_80096F60;
extern volatile CdlIntr D_800962C8; // CD_INTR
extern void* D_800962CC[];
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
            CD_INTSTR[D_800962C8.sync],
            CD_INTSTR[D_800962C8.ready]);
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

int CD_initvol(void) {
    CdlATV vol;
    // 0x800962C4 + 0x1b8 = 0x8009647C (current main vol left).
    if (*((volatile u16*)((u32)D_80096F60 + 0x1b8)) == 0 &&
    *((volatile u16*)((u32)D_80096F60 + 0x1ba)) == 0) {
        // 0x800962C4 + 0x180 = 0x80096444 (main vol left/right).
        *((volatile u16*)((u32)D_80096F60 + 0x180)) = 0x3fff;
        *((volatile u16*)((u32)D_80096F60 + 0x182)) = 0x3fff;
    }
    // 0x800962C4 + 0x1b0 = 0x80096474 (CD volume left/right).
    *((volatile u16*)((u32)D_80096F60 + 0x1b0)) = 0x3fff;
    *((volatile u16*)((u32)D_80096F60 + 0x1b2)) = 0x3fff;
    // Enable spu, unmute spu and enable cd audio.
    *((volatile u16*)((u32)D_80096F60 + 0x1aa)) =
    (1 << 15) | (1 << 14) | (1 << 0);
    vol.val0 = vol.val2 = 0x80;
    vol.val1 = vol.val3 = 0;
    *D_80096F4C = 2;
    *D_80096F54 = vol.val0;
    *D_80096F58 = vol.val1;
    *D_80096F4C = 3;
    *D_80096F50 = vol.val2;
    *D_80096F54 = vol.val3;
    *D_80096F58 = 0x20;
    return 0;
}

void CD_initintr(void);

const char D_8001612C[] = "$Id: bios.c,v 1.81 1996/12/16 06:24:14 makoto Exp $";

int CD_init(void);

int CD_datasync(int arg0);

int CD_getsector(int arg0, int arg1);

void CD_set_test_parmnum(int arg0);

void callback(void);
