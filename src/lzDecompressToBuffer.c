// FUNC 8003bc88 72 MAIN0
// MATCHING 8003bc88 72
// Ported from psx_tomba (scriptop.c, lzDecompressToBuffer); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void lzDecompressToBuffer(char* src, char* dst, char* len)
{
    bzero(dst, len);
    lzDecompress(src, dst);
}
