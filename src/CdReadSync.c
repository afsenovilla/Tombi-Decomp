// FUNC 800670d8 204 MAIN0
// MATCHING 800670d8 204
// Portado de psx_tomba (psyq/libcd/cdread.c, CdReadSync); licencia MIT del proyecto original.
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

int cd_read_retry(int retry);

void CdReadBreak(void);

int CdRead(int sectors, u_long* buf, int mode);

int CdReadSync(int mode, u_char* result) {
    int var_s0;

    while (true) {
        var_s0 = -1;
        if (VSync(-1) <= D_80096FA0.vb_start + 1200) {
            if (D_80096FA0.status < 0 || VSync(-1) > D_80096FA0.vb_attempt_start + 60) {
                cd_read_retry(true);
                var_s0 = D_80096FA0.nsectors;
            } else {
                var_s0 = D_80096FA0.status;
            }
        }
        if (mode != 0 || var_s0 <= 0) {
            CdReady(1, result);
            return var_s0;
        }
    }
}

CdlCB CdReadCallback(CdlCB func);
