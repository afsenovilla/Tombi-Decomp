// FUNC 800eec40 512 X011
// MATCHING 800eec40 512
#include "TOBJ.H"
extern unsigned short D_8009C960;
extern short D_8009C944;
extern char D_800112E0[];
extern char D_80010AE8[];
void AnimLoadDuration(TObj *o);
void AnimJump(TObj *o, short n);

void func_800EEC40(TObj *o)
{
    switch (*(unsigned short *)o->anim) {
    case 0x47:
        break;
    case 0xfa:
    case 0xfb:
    case 0xfc:
        if (o->animFrame & 1) {
            if (o->d8c > 0)
                o->d8c = -o->d8c;
            o->d8c -= 4;
            if (o->d8c < -0x20)
                o->d8c = -0x20;
        } else {
            if (o->d8c < 0)
                o->d8c = -o->d8c;
            o->d8c += 4;
            if (o->d8c > 0x20)
                o->d8c = 0x20;
        }
        return;
    default:
        if (o->animFrame & 1) {
            if (o->d8c > 0x80)
                o->d8c = (0x100 - o->d8c) & 0xff;
            o->d8c = (o->d8c + 4) & 0xff;
            if (o->d8c <= 0x40)
                return;
            if (D_8009C960 == 3 && D_8009C944 != 0) {
                o->anim = D_800112E0;
                AnimLoadDuration(o);
            } else {
                o->anim = D_80010AE8;
                AnimJump(o, 3);
            }
        } else {
            if (o->d8c < 0x80)
                o->d8c = (0x100 - o->d8c) & 0xff;
            o->d8c = (o->d8c - 4) & 0xff;
            if (o->d8c >= 0xc0)
                return;
            if (D_8009C960 == 3 && D_8009C944 != 0) {
                o->anim = D_800112E0;
                AnimLoadDuration(o);
            } else {
                o->anim = D_80010AE8;
                AnimJump(o, 3);
            }
        }
        break;
    }
    o->d8c = 0;
}
