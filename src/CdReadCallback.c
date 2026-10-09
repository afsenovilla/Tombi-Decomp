// FUNC 800671a4 24 MAIN0
// MATCHING 800671a4 24
// Portado de psx_tomba (psyq/libcd/cdread.c, CdReadCallback); licencia MIT del proyecto original.
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

extern CdlCB D_80096F9C;
extern volatile ReadAttr_t D_80096304;

void cb_read(u_char arg0, u_char* arg1);

int cd_read_retry(int retry);

void CdReadBreak(void);

int CdRead(int sectors, u_long* buf, int mode);

int CdReadSync(int mode, u_char* result);

CdlCB CdReadCallback(CdlCB func) {
    CdlCB prevFunc = D_80096F9C;
    D_80096F9C = func;
    return prevFunc;
}
