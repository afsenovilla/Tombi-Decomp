// FUNC 80065a18 640 MAIN0
// MATCHING 80065a18 640
// Portado de psx_tomba (psyq/libcd/bios.c, CD_sync); licencia MIT del proyecto original.
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
extern volatile u_char* D_800962B8;
extern volatile int* D_800962C0;
extern void* D_800962C4;
extern volatile CdlIntr CD_INTR;
extern void* D_800962CC[];
extern volatile unsigned char* cd_reg0;
extern s32* D_800962E4;
extern s32* D_800962E8;
extern s32* D_800962EC;
extern s32* D_800962F0;
extern volatile s32* D_800962F4;
extern char cd_result_sync[];
extern char cd_result_ready[];
extern volatile u_char* D_800962BC;
extern char D_8009B2B8[];
extern int cd_alarm;
extern int cd_alarm_count; // timeout
extern char* cd_alarm_func[];

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
    if ((cd_alarm < VSync(-1)) || cd_alarm_count++ > 0x3C0000) {
        puts("CD timeout: ");
        
        printf("%s:(%s) Sync=%s, Ready=%s\n",
            cd_alarm_func[0],
            CD_COMSTR[CD_COM],
            CD_INTSTR[CD_INTR.sync],
            CD_INTSTR[CD_INTR.ready]);
        CD_flush();
        return -1;
    } 
    return 0;
}

int getintr(void);

int CD_sync(int mode, unsigned char* result) {
    int i;
    int intr;
    int sync;
    
    int temp_s1;
    u_char* src;
    u_char* dst;
    char com, s, r;
    char** name;

    cd_alarm = VSync(-1) + 0x3C0;
    cd_alarm_count = 0;
    cd_alarm_func[0] = "CD_sync";  

    while (true) {
        if (get_alarm() != 0) {
            return -1;
        }

        if (CheckCallback()) {
            temp_s1 = cd_reg0[0] & 3;

            while (intr = getintr()) {
                if (intr & 4 && CD_CBREADY != NULL) {
                    CD_CBREADY(CD_INTR.ready, cd_result_ready);
                }
                if (intr & 2 && CD_CBSYNC != NULL) {
                    CD_CBSYNC(CD_INTR.sync, cd_result_sync);
                }
            }
            cd_reg0[0] = temp_s1;
        }

        sync = CD_INTR.sync;
        if (sync == 2 || sync == 5) {
            CD_INTR.sync = 2;
            dst = result;
            src = cd_result_sync;
            if (dst != NULL) {
                for (i = 7; i != -1; i--) {
                    *dst++ = *src++;
                }
            }
            return sync;
        }

        if (mode != 0) {
            break;
        }
    }
    return 0;
}

int CD_ready(int arg0, u_char* arg1);

int CD_cw(u_char arg0, u_char* arg1, u_char* arg2, int arg3);

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
