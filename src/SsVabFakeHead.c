// FUNC 80074760 48 MAIN0
// MATCHING 80074760 48
// Portado de psx_tomba (psyq/libsnd/vs_vh.c, SsVabFakeHead); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

#define LEN(x) ((s32)(sizeof(x) / sizeof(*(x))))
#define NUM_VAB 16
extern long SpuMalloc(long size);
int _spu_getInTransfer(void);

s16 SsVabOpenHead(u8* arg1, s16 vabid);

s16 SsVabOpenHeadSticky(u8* addr, s16 vabid, u32 sbaddr);

s16 SsVabFakeHead(u8* addr, s16 vabid, u32 sbaddr) {
    return SsVabOpenHeadWithMode(addr, vabid, 1, sbaddr);
}

short SsVabOpenHeadWithMode( u_char* addr, short vabid, short mode, u_long sbaddr);

void SpuInit(void);
