// FUNC 80066ff8 224 MAIN0
// MATCHING 80066ff8 224
// Portado de psx_tomba (psyq/libcd/cdread.c, CdRead); licencia MIT del proyecto original.
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
extern volatile ReadAttr_t CD_READ_ATTR;

void cb_read(u_char arg0, u_char* arg1);

int cd_read_retry(int retry);

void CdReadBreak(void);

int CdRead(int sectors, u_long* buf, int mode) {
    CD_READ_ATTR.mode = mode;
    switch (CD_READ_ATTR.mode & (CdlModeSize0 | CdlModeSize1)) {
    case 0:
        CD_READ_ATTR.secsize = 0x200;
        break;
    case CdlModeSize1:
        CD_READ_ATTR.secsize = 0x249;
        break;
    default:
        CD_READ_ATTR.secsize = 0x246;
        break;
    }
    CD_READ_ATTR.mode |= CdlModeSize1;
    CD_READ_ATTR.buf_start = buf;
    CD_READ_ATTR.nsectors = sectors;
    CD_READ_ATTR.cbsync = CdSyncCallback(NULL);
    CD_READ_ATTR.cbready = CdReadyCallback(NULL);
    CD_READ_ATTR.vb_start = VSync(-1);
    if (CdStatus() & (CdlStatPlay | CdlStatSeek | CdlStatRead)) {
        CdControlB(CdlPause, NULL, NULL);
    }
    return cd_read_retry(false) > 0;
}

int CdReadSync(int mode, u_char* result);

CdlCB CdReadCallback(CdlCB func);
