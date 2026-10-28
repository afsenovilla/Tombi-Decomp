// FUNC 8005be1c 16 MAIN0
// MATCHING 8005be1c 16
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libc2/rand.c, srand); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/rand.h"

extern u32 RAND_SEED;

int rand(void);

void srand(unsigned int seed) {
    RAND_SEED = seed;
}
