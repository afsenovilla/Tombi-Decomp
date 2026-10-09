// FUNC 80064c0c 144 MAIN0
// MATCHING 80064c0c 144
// Portado de psx_tomba (psyq/libcd/event.c, CdInit); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"
#include "psyq/kernel.h"

void def_cbsync(u_char intr, u_char* result);
void def_cbready(u_char intr, u_char* result);
void def_cbread(u_char intr, u_char* result);

int CdInit(void) {
    int status;
    int i;
    i = 4;
    while(1) {
        status = CdReset(1);
        i+=-1;
        if (status == 1) break;
        status = -1;
        if (i == status) {
           printf("CdInit: Init failed\n");
          return 0;
        }
    }
    CdSyncCallback(def_cbsync);
    CdReadyCallback(def_cbready);
    CdReadCallback(def_cbread);
    return 1;
}

void def_cbsync(u_char intr, u_char* result);

void def_cbready(u_char intr, u_char* result);

void def_cbread(u_char intr, u_char* result);
