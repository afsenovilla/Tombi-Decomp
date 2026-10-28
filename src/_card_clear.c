// FUNC 8005d21c 56 MAIN0
// MATCHING 8005d21c 56
// Ported from sotn-decomp src/main/psxsdk/libcard/card.c (MIT, (c) github.com/xeeynamo/sotn-decomp contributors), derived from Sony's PSY-Q libraries.
#define SKIP_ASM

#include "common.h"
#include "psyq/kernel.h"
long _card_write(long chan, long block, unsigned char *buf);
void _new_card(void);

long _card_clear(long chan) {
    _new_card();
    return _card_write(chan, 0x3F, 0);
}
