// FUNC 80066ddc 460 MAIN0
// MATCHING 80066ddc 460
// Portado de psx_tomba (psyq/libcd/cdread.c, cd_read_retry); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

typedef struct {
/*0x00*/    int nsectors;
/*0x04*/    u_long* buf_start;
/*0x08*/    u_long* buf_cur;
/*0x0C*/    int mode;
/*0x10*/    int secsize;
/*0x14*/    int status;
/*0x18*/    int vb_attempt_start;
/*0x1C*/    int vb_start;
/*0x20*/    int pos;
/*0x24*/    CdlCB cbsync;
/*0x28*/    CdlCB cbready;
/*0x2C*/    CdlCB cbdata;
/*0x30*/    int block_mode; 
/*0x34*/    u_char* unk34;
/*0x38*/    int unk38;
} ReadAttr_t;

extern CdlCB D_80096300;
extern volatile ReadAttr_t D_80096FA0;

void cb_read(u_char arg0, u_char* arg1);

int cd_read_retry(int retry) {
    char mode;
    int mode2;
    CdSyncCallback(NULL);
    CdReadyCallback(NULL);
    if (CdStatus() & CdlStatShellOpen) {
        if ((VSync(-1) % 0x40) == 0) {
            puts("CdRead: Shell open...\n");
        }
        CdControlF(CdlNop, NULL);
        D_80096FA0.vb_start = VSync(-1);
        D_80096FA0.status = -1;
        return D_80096FA0.status;
    }
    if (retry) {
        puts("CdRead: retry...\n");
        CdControl(CdlPause, NULL, NULL);
        if (!CdControl(CdlSetloc, CdLastPos(), NULL)) {
            return D_80096FA0.status = -1;
        }
    }
    CdFlush();
    mode2 = D_80096FA0.mode;
    mode = mode2; // FAKE
    if ((char)mode2 != CdMode() || retry) {
        if (!CdControl(CdlSetmode, &mode, NULL)) {
            D_80096FA0.status = -1;
            return D_80096FA0.status;
        }
    }
    D_80096FA0.pos = CdPosToInt(CdLastPos());
    CdReadyCallback(&cb_read);
    D_80096FA0.buf_cur = D_80096FA0.buf_start;
    CdControlF(CdlReadN, NULL);
    D_80096FA0.status = D_80096FA0.nsectors;
    D_80096FA0.vb_attempt_start = VSync(-1);
    return D_80096FA0.status;
}

void CdReadBreak(void);

int CdRead(int sectors, u_long* buf, int mode);

int CdReadSync(int mode, u_char* result);

CdlCB CdReadCallback(CdlCB func);
