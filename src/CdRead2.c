// FUNC 800671bc 148 MAIN0
// MATCHING 800671bc 148
// Portado de psx_tomba (psyq/libcd/cdread2.c, CdRead2); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern s32 D_8009C914;

void StCdInterrupt2(u_char intr, u_char* result);
void data_ready_callback();

int CdRead2(long mode) {
    u8 param = mode;
    CdControl(CdlSetmode, &param, NULL);
    if (mode & CdlModeStream) {
        if (mode & CdlModeSize1) {
            D_8009C914 = 0;
        } else {
            D_8009C914 = 1;
        }
        CdDataCallback(data_ready_callback);
        CdReadyCallback(StCdInterrupt2);
    }
    return CdControl(CdlReadS, NULL, NULL);
}

void StCdInterrupt2(u_char intr, u_char* result);
