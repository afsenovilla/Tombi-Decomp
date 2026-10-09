// FUNC 80067250 32 MAIN0
// MATCHING 80067250 32
// Portado de psx_tomba (psyq/libcd/cdread2.c, StCdInterrupt2); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern s32 D_8009BC7C;

void StCdInterrupt2(u_char intr, u_char* result);
void data_ready_callback();

int CdRead2(long mode);

void StCdInterrupt2(u_char intr, u_char* result) {
    StCdInterrupt();
}
