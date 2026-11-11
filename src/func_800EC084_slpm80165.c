// VERSION SLPM_80165 x00 800EC084 244
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only player helper: turns the tilt +0x8C by 4 a frame towards the facing side (folded into 0..0xFF); past
// 0x40 (facing right) or below 0xC0 (facing left) it loads anim D_800108BC at frame 3 and clears the tilt, which
// anim 0x55 also clears.
#include "TOBJ.H"
extern char D_800108BC[];
extern void func_8002057C(TObj *, int);

void func_800EC084(TObj *o)
{
    if (o->animFrame & 1) {
        if (o->d8c > 0x80)
            o->d8c = (0x100 - o->d8c) & 0xff;
        o->d8c = (o->d8c + 4) & 0xff;
        if (o->d8c > 0x40) {
            o->anim = D_800108BC;
            func_8002057C(o, 3);
            o->d8c = 0;
        }
    } else {
        if (o->d8c < 0x80)
            o->d8c = (0x100 - o->d8c) & 0xff;
        o->d8c = (o->d8c - 4) & 0xff;
        if (o->d8c < 0xc0) {
            o->anim = D_800108BC;
            func_8002057C(o, 3);
            o->d8c = 0;
        }
    }
    if (*(unsigned short *)o->anim == 0x55)
        o->d8c = 0;
}
#endif
