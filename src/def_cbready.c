// FUNC 80064cc4 40 MAIN0
// MATCHING 80064cc4 40
// Portado de psx_tomba (psyq/libcd/event.c, def_cbready); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"
#include "psyq/kernel.h"

void def_cbsync(u_char intr, u_char* result);
void def_cbready(u_char intr, u_char* result);
void def_cbread(u_char intr, u_char* result);

int CdInit(void);

void def_cbsync(u_char intr, u_char* result);

void def_cbready(u_char intr, u_char* result) {
    DeliverEvent(HwCdRom, EvSpDR);
}

void def_cbread(u_char intr, u_char* result);
