// FUNC 80065144 324 MAIN0
// MATCHING 80065144 324
// Portado de psx_tomba (psyq/libcd/sys.c, CdControlB); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern int D_80096C00[];
extern CdlCB CD_CBSYNC;
extern s32 CD_DEBUG;
extern CdlCB CD_CBREADY;
extern u8 CD_STATUS;
extern CdlLOC CD_POS;
extern const char* CD_COMSTR[];
extern const char* CD_INTSTR[];
extern u8 CD_COM;
extern u8 CD_MODE;

int CdStatus(void);

int CdMode(void);

int CdLastCom(void);

CdlLOC* CdLastPos(void);

int CdReset(int mode);

void CdFlush(void);

s32 CdSetDebug(s32 level);

char* CdComstr(u_char com);

char* CdIntstr(u_char intr);

int CdSync(int mode, u_char* result);

int CdReady(int mode, u_char* result);

CdlCB CdSyncCallback(CdlCB func);

CdlCB CdReadyCallback(CdlCB func);

static inline int loop(u_char com, u_char* param, u_char* result, int arg3) {
    int i;
    CdlCB cbprev = CD_CBSYNC;
    for (i = 3; i != -1; --i) {
        CD_CBSYNC = 0;

        if (com != 1 && (CD_STATUS & CdlStatShellOpen) != 0) {
            CD_cw(CdlNop, 0, 0, 0);
        }

        if (param == 0 || D_80096C00[com] == 0 ||
            CD_cw(CdlSetloc, param, result, 0) == 0) {
            CD_CBSYNC = cbprev;
            if (CD_cw(com, param, result, arg3) == 0) {
                return 0;
            }
        }
    }

    CD_CBSYNC = cbprev;
    return -1;
}

int CdControl(u_char com, u_char* param, u_char* result);

int CdControlF(u_char com, u_char* param);

int CdControlB(u_char com, u_char* param, u_char* result)
{
    if (loop(com, param, result, 0) != 0) {
        return 0;
    }
    return CD_sync(0, result) == 2;
}

int CdMix(CdlATV* vol);

int CdGetSector(void* madr, int size);
void(*CdDataCallback(void (*func)()));

int CdDataSync(int mode);

CdlLOC* CdIntToPos(int i, CdlLOC* p);

int CdPosToInt(CdlLOC* p);
