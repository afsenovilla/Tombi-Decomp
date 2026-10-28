// FUNC 80065490 1416 MAIN0
// MATCHING 80065490 1416
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libcd/bios.c, getintr); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern u8 CD_COM_x[] asm("CD_COM");
extern u8 CD_POS_x[] asm("CD_POS");
extern u8 CD_STATUS_x[] asm("CD_STATUS");
extern u8 D_80015F34_x[] asm("D_80015F34");
extern u8 D_8001612C_x[] asm("D_8001612C");
extern u8 D_800962C8_x[] asm("D_80096F64");
extern u8 D_8009B2A8_x[] asm("D_8009BF40");

u32 CD_CBSYNC_d[1] asm("CD_CBSYNC") = {
    0x00000000,
};
u32 CD_CBREADY_d[2] asm("CD_CBREADY") = {
    0x00000000, 0x00000000,
};
u32 CD_DEBUG_d[1] asm("CD_DEBUG") = {
    0x00000000,
};
u32 CD_STATUS_d[1] asm("CD_STATUS") = {
    0x00000000,
};
u32 CD_STATUS1_d[1] asm("CD_STATUS1") = {
    0x00000000,
};
u32 D_80096004_d[1] asm("D_80096CA0") = {
    0x00000000,
};
u32 CD_POS_d[1] asm("CD_POS") = {
    0x00000002,
};
u32 CD_MODE_d[1] asm("CD_MODE") = {
    0x00000000,
};
u32 CD_COMSTR_d[32] asm("CD_COMSTR") = {
    (u32)D_80015F34_x + 0xF4, (u32)D_80015F34_x + 0xEC, (u32)D_80015F34_x + 0xE0, (u32)D_80015F34_x + 0xD8,
    (u32)D_80015F34_x + 0xCC, (u32)D_80015F34_x + 0xC0, (u32)D_80015F34_x + 0xB4, (u32)D_80015F34_x + 0xA8,
    (u32)D_80015F34_x + 0xA0, (u32)D_80015F34_x + 0x94, (u32)D_80015F34_x + 0x88, (u32)D_80015F34_x + 0x80,
    (u32)D_80015F34_x + 0x74, (u32)D_80015F34_x + 0x64, (u32)D_80015F34_x + 0x58, (u32)D_80015F34_x + 0x54,
    (u32)D_80015F34_x + 0x48, (u32)D_80015F34_x + 0x3C, (u32)D_80015F34_x + 0x54, (u32)D_80015F34_x + 0x30,
    (u32)D_80015F34_x + 0x24, (u32)D_80015F34_x + 0x18, (u32)D_80015F34_x + 0xC, (u32)D_80015F34_x + 0x54,
    (u32)D_80015F34_x + 0x54, (u32)D_80015F34_x + 0x54, (u32)D_80015F34_x + 0x54, (u32)D_80015F34_x,
    (u32)D_80015F34_x + 0x54, (u32)D_80015F34_x + 0x54, (u32)D_80015F34_x + 0x54, (u32)D_80015F34_x + 0x54,
};
u32 CD_INTSTR_d[8] asm("CD_INTSTR") = {
    (u32)D_80015F34_x + 0x134, (u32)D_80015F34_x + 0x128, (u32)D_80015F34_x + 0x11C, (u32)D_80015F34_x + 0x110,
    (u32)D_80015F34_x + 0x108, (u32)D_80015F34_x + 0xFC, (u32)D_80015F34_x + 0x54, (u32)D_80015F34_x + 0x54,
};
u32 D_800960B0_d[32] asm("D_80096D4C") = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0x00000000,
    0x00000000, 0x00000001, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
};
u32 D_80096130_d[32] asm("D_80096130") = {
    0x00000000, 0x00000000, 0x00000000, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000001, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
};
u32 D_800961B0_d[32] asm("D_80096E4C") = {
    0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000000, 0x00000000, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001,
};
u32 D_80096230_d[25] asm("D_80096230") = {
    0x00000000, 0x00000000, 0x00000003, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000002, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000001, 0x00000000,
    0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000,
};
u32 D_80096294_d[7] asm("D_80096294") = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000,
};
u32 D_800962B0_d[1] asm("D_80096F4C") = {
    0x1F801800,
};
u32 D_800962B4_d[1] asm("D_80096F50") = {
    0x1F801801,
};
u32 D_800962B8_d[1] asm("D_80096F54") = {
    0x1F801802,
};
u32 D_800962BC_d[1] asm("D_80096F58") = {
    0x1F801803,
};
u32 D_800962C0_d[1] asm("D_800962C0") = {
    0x1F801020,
};
u32 D_800962C4_d[1] asm("D_800962C4") = {
    0x1F801C00,
};
u32 D_800962C8_d[1] asm("D_80096F64") = {
    0x00000000,
};
u32 D_800962CC_d[6] asm("D_800962CC") = {
    (u32)D_800962C8_x, (u32)D_8009B2A8_x, (u32)CD_COM_x, (u32)CD_STATUS_x,
    (u32)CD_POS_x, (u32)D_8001612C_x,
};
u32 D_800962E4_d[1] asm("D_800962E4") = {
    0x1F801018,
};
u32 D_800962E8_d[1] asm("D_800962E8") = {
    0x1F8010F0,
};
u32 D_800962EC_d[1] asm("D_800962EC") = {
    0x1F8010B0,
};
u32 D_800962F0_d[1] asm("D_800962F0") = {
    0x1F8010B4,
};
u32 D_800962F4_d[1] asm("D_800962F4") = {
    0x1F8010B8,
};

asm(
    ".globl CD_COM\nCD_COM = CD_MODE + 1\n"
    ".globl D_800962C9\nD_800962C9 = D_80096F64 + 1\n"
    ".globl D_800962CA\nD_800962CA = D_80096F64 + 2\n"
);
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

INCLUDE_RODATA("nonmatchings/psyq/libcd/bios", D_80015F34);

extern int CD_DEBUG; // CD_DEBUG
extern u_char CD_COM;
extern u_char CD_MODE;
extern int CD_STATUS;
extern int CD_STATUS1;
extern int D_80096CA0;
extern CdlLOC CD_POS;
extern const char* CD_COMSTR[]; // CD_COMSTR
extern const char* CD_INTSTR[];
extern const char* D_80096D4C[];
extern int D_80096130[]; 
extern const char* D_80096E4C[];
extern int D_80096230[]; // CD_COMATTR
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
extern volatile u_char* D_80096F58;
extern char D_8009BF50[];
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

int getintr(void) {
    volatile char nReg;
    volatile char buf[8];
    int i, j;
    int bHasError;

    *D_80096F4C = 1;

    nReg = *D_80096F58 & 0x7;

    if (nReg == 0) {
        return 0;
    }

    bHasError = 0;

    while (nReg != (*D_80096F58 & 7)) {
        nReg = *D_80096F58 & 0x7;
    }

    for (i = 0; i < 8; i++) {
        if ((*D_80096F4C & 0x20) == 0) {
            break;
        }
        buf[i] = *D_80096F50;
    }
    for (j = i; j < 8; j++) {
        buf[j] = 0;
    }

    *D_80096F4C = 1;
    *D_80096F58 = 7;
    *D_80096F54 = 7;
    if (nReg != 3 || D_80096E4C[CD_COM]) {
        if (!(CD_STATUS & CdlStatShellOpen) && (buf[0] & CdlStatShellOpen)) {
            D_80096CA0++;
        }
        CD_STATUS = buf[0];
        CD_STATUS1 = buf[1];
        bHasError = CD_STATUS;
        bHasError &= (CdlStatError | CdlStatSeekError | CdlStop | CdlStatShellOpen);
    }
    if ((nReg == 5) && (0 < CD_DEBUG)) {
        printf("DiskError: ");
        if (CD_DEBUG > 0)
            printf("com=%s,code=(%02x:%02x)\n", CD_COMSTR[CD_COM], CD_STATUS, CD_STATUS1);
    }
    switch (nReg) {
    case 3:
        if (bHasError) {
            D_80096F64.sync = CdlDiskError;
            rescpy(&D_8009BF40, &buf);
            return 2;
        }
        if (D_80096D4C[CD_COM]) {
            D_80096F64.sync = CdlAcknowledge;
            rescpy(&D_8009BF40, &buf);
            return 1;
        }
        D_80096F64.sync = CdlComplete;
        rescpy(&D_8009BF40, &buf);
        return 2;
    case 2:
        D_80096F64.sync = bHasError ? CdlDiskError : CdlComplete;
        rescpy(&D_8009BF40, &buf);
        return 2;
    case 1:
        if (bHasError && i == 1) {
            bHasError = 0;
        }
        D_80096F64.ready = bHasError ? CdlDiskError : CdlDataReady;
        rescpy(&D_8009BF48, &buf);
        *D_80096F4C = 0;
        *D_80096F58 = 0;
        return 4;
    case 4:
        D_80096F64.ready = D_80096F64.c = CdlDataEnd;
        rescpy(&D_8009BF50, &buf);
        rescpy(&D_8009BF48, &buf);
        return 4;
    case 5:
        D_80096F64.sync = D_80096F64.ready = CdlDiskError;
        rescpy(&D_8009BF40, &buf);
        rescpy(&D_8009BF48, &buf);
        return 6;
    default:
        puts("CDROM: unknown intr");
        printf("(%d)\n", nReg);
        return 0;
    }
}

int CD_sync(int mode, unsigned char* result);

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
