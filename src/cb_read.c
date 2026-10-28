// FUNC 80066bc8 532 MAIN0
// MATCHING 80066bc8 532
// Ported from sotn-decomp src/main/psxsdk/libcd/cdread.c (MIT, (c) github.com/xeeynamo/sotn-decomp contributors), derived from Sony's PSY-Q libraries.
#define SKIP_ASM

#include "common.h"
#include "psyq/libcd.h"

typedef struct {
    int sectors;
    u_long* unk4;
    u_long* unk8;
    int mode;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    CdlCB sync;
    CdlCB ready;
} cdreadStruct;

static CdlCB CD_ReadCallbackFunc = NULL;
static volatile cdreadStruct D_80032DBC = {0};

void cb_read(u_char arg0, u_char* result) {
    int pos[3];

    if (arg0 == 1) {
        if (D_80032DBC.unk14 > 0) {
            if (D_80032DBC.unk10 == 0x200) {
                CdGetSector(pos, 3);
                if (CdPosToInt(pos) != D_80032DBC.unk20) {
                    puts("CdRead: sector error\n");
                    D_80032DBC.unk14 = -1;
                }
            }
            CdGetSector(D_80032DBC.unk8, D_80032DBC.unk10);
            D_80032DBC.unk8 += D_80032DBC.unk10;
            D_80032DBC.unk14--;
            D_80032DBC.unk20++;
        }
    } else {
        D_80032DBC.unk14 = -1;
    }
    D_80032DBC.unk18 = VSync(-1);
    if (D_80032DBC.unk14 < 0) {
        cd_read_retry(true);
    }
    if (VSync(-1) > D_80032DBC.unk1C + 1200) {
        D_80032DBC.unk14 = -1;
    }
    if (D_80032DBC.unk14 && VSync(-1) <= D_80032DBC.unk1C + 1200) {
        return;
    }
    CdSyncCallback(D_80032DBC.sync);
    CdReadyCallback(D_80032DBC.ready);
    CdControl(CdlPause, NULL, NULL);
    if (CD_ReadCallbackFunc != NULL) {
        CD_ReadCallbackFunc(
            D_80032DBC.unk14 == 0 ? CdlComplete : CdlDiskError, result);
    }
}

int cd_read_retry(int retry);

int CdRead(int sectors, u_long* buf, int mode);

int CdReadSync(int mode, u_char* result);

CdlCB CdReadCallback(CdlCB func);
