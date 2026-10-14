// FUNC 80136aac 128 X001
// MATCHING 80136aac 128
#include "player.h"
extern int Rand(void);
extern void SfxPlay3(int, int);

void func_80136AAC(void)
{
    int a;
    if ((unsigned short)((*(unsigned short **)&PLAYER.obj.h)[1] - 0x8ae) < 0x200 && (Rand() & 0x3f) == 0) {
        if (PLAYER.obj.y.p.whole < -0x190) a = 0x7f;
        else if (PLAYER.obj.y.p.whole < -0xc8) a = 0x70;
        else a = 0x60;
        SfxPlay3(0x41, a);
    }
}
