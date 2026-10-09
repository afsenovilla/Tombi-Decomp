// FUNC 800712cc 16 MAIN0
// MATCHING 800712cc 16
// Portado de psx_tomba (psyq/libsnd/ut_rev.c, SsUtGetReverbType); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

s16 SsUtSetReverbType(s16 arg0);

s16 SsUtGetReverbType(void) { return _svm_rattr.mode; }
