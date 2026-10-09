// FUNC 80039150 96 MAIN0
// MATCHING 80039150 96
// Ported from psx_tomba (scriptexec.c, scriptRunOpcode); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern u8 D_8009C119;
extern u8 D_8009C120;
extern u8 D_8009C121;
extern u8 D_8009C245;

void scriptRunOpcode(void)
{
    ScriptContext* p = SCRIPT_CTX;
    u8* script = SCRIPT_CODE;
    u8  op = script[p->pc];

    if (op < 0x80) {
        execCoreOpcode(op);
    } else {
        execGameOpcode(op);
    }
}
