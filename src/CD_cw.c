// FUNC 80065f64 1052 MAIN0
// MATCHING 80065f64 1052
// Portado de psx_tomba (psyq/libcd/bios.c, CD_cw); licencia MIT del proyecto original.
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
extern int D_80096DCC[]; 
extern const char* D_800961B0[];
extern int D_80096ECC[]; // CD_COMATTR
extern int D_80096294;
extern volatile u_char* D_80096F50;
extern volatile u_char* D_80096F54;
extern volatile int* D_800962C0;
extern void* D_800962C4;
extern volatile CdlIntr D_80096F64; // CD_INTR
extern void* D_800962CC[];
extern volatile unsigned char* D_80096F4C;
extern s32* D_800962E4;
extern s32* D_800962E8;
extern s32* D_800962EC;
extern s32* D_800962F0;
extern volatile s32* D_800962F4;
extern char D_8009BF40[];
extern char D_8009BF48[];
extern volatile u_char* D_800962BC;
extern char D_8009B2B8[];
extern int D_8009BF58;
extern int D_8009BF5C; // timeout
extern char* D_8009BF60[];

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
    if ((D_8009BF58 < VSync(-1)) || D_8009BF5C++ > 0x3C0000) {
        puts("CD timeout: ");
        
        printf("%s:(%s) Sync=%s, Ready=%s\n",
            D_8009BF60[0],
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

int CD_cw(u_char arg0, u_char* arg1, u_char* arg2, int arg3)
{
    s32 i;
    s32 intr;
    s32 sync;
    s32 temp_s1;
    u8* src;
    u8* dst;
    
    if (CD_DEBUG > 1) {
        printf("%s...\n", CD_COMSTR[arg0]);
    }
    
    if ((D_80096ECC[arg0] != 0) && (arg1 == 0)) {
        if (CD_DEBUG > 0) {
            printf("%s: no param\n", CD_COMSTR[arg0]);
        }
        
        return -2;
    }
    
    CD_sync(0, 0);
    if (arg0 == 0x2) {
        for (i = 0; i < 4; i++) {
            ((u8*)&CD_POS)[i] = ((u8*)arg1)[i];
        }
    }
    if (arg0 == 0xE) {
        CD_MODE = *arg1;
    }
    
    D_80096F64.sync = 0;
    if (D_80096DCC[arg0] != 0) {
        D_80096F64.ready = 0;
    }
    
    *D_80096F4C = 0;
    for (i = 0; i < D_80096DCC[0x40 + arg0]; i++) {
        *D_80096F54 = arg1[i];
    }
    
    CD_COM = arg0;
    *D_80096F50 = arg0;   
    if (arg3 != 0) return 0;
    D_8009BF58 = VSync(-1) + 0x3C0;
    D_8009BF5C = 0;
    D_8009BF60[0] = "CD_cw";
    
    while ( D_80096F64.sync == 0) {
        if (get_alarm() != 0) {
            return -1;
        }
        
        if (CheckCallback()) {
            temp_s1 = D_80096F4C[0] & 3;
            while (intr = getintr()) {
                if (intr & 4 && CD_CBREADY != NULL) {
                    CD_CBREADY(D_80096F64.ready, D_8009BF48);
                }
                if (intr & 2 && CD_CBSYNC != NULL) {
                    CD_CBSYNC(D_80096F64.sync, D_8009BF40);
                }
            }
            D_80096F4C[0] = temp_s1;
        }
    }

    rescpy(arg2, D_8009BF40);

    return D_80096F64.sync == 5 ? -1 : 0;
}

int CD_vol(CdlATV* vol);

void CD_flush(void);

int CD_initvol(void);

void CD_initintr(void);

const char D_8001612C[] = "$Id: bios.c,v 1.81 1996/12/16 06:24:14 makoto Exp $";

int CD_init(void);

int CD_datasync(int arg0);

int CD_getsector(int arg0, int arg1);

void CD_set_test_parmnum(int arg0);

void callback(void);
