// FUNC 8006f9b0 44 MAIN0
// MATCHING 8006f9b0 44
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_play.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
#include "common.h"

void _SsSeqPlay(s16, s16);

/* libsnd _SsSndPlay (PLAY.OBJ): the sequencer tick of one score */
void _SsSndPlay(s16 seq, s16 sep) { _SsSeqPlay(seq, sep); }
