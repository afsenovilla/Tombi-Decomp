// FUNC 80030b5c 64 MAIN0
// MATCHING 80030b5c 64
// Ported from psx_tomba (effect.c, dispatchEffectByVariant); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
void updateScrollAnchoredEffect(u8* self);

extern u8 D_8009C1C0;

extern u8 D_8009C3F0;
extern u8 D_8009BCA6;

void func_800346A8(u8* self);

void dispatchEffectByVariant(ObjectVariantView* arg0)
{
    if (arg0->unk3 == 0) {
        func_8011B6BC();
        return;
    }
    func_8011FB10();
}
