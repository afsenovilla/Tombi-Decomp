// FUNC 8010d888 644 X002
// MATCHING 8010d888 644
#include "TOBJ.H"
typedef struct { TObj t; char pad[10]; unsigned char bca; } PObj;
extern TObj *D_8009C330;
extern unsigned short D_1F8001FC, D_1F8003C4, D_1F8003C6;
extern void FUN_800eea7c(TObj *, int, int);
extern int AnimAdvance(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_8010d280(TObj *);

void func_8010D888(TObj *o)
{
    unsigned short d;
    switch (o->state) {
    case 0:
        FUN_800eea7c(o, 0x44, 0);
        o->timer = 10;
        ((PObj *)o)->bca = 1;
        D_8009C330->animFrame = 0xff;
        o->state++;
    case 1:
        if (AnimAdvance(o) && (unsigned short)(*(unsigned short *)o->anim - 0x136) >= 4)
            PlayerSetAnimIfChanged(o, 0x44);
        d = o->w76 & 7;
        switch (d) {
        case 0: o->w7a = 0; break;
        case 1: o->w7a = 0x20; break;
        case 2: o->w7a = 0x40; break;
        case 3: o->w7a = 0x60; break;
        case 4: o->w7a = 0x80; break;
        case 5: o->w7a = 0xa0; break;
        case 6: o->w7a = 0xc0; break;
        case 7: o->w7a = 0xe0; break;
        }
        if (o->w76 & 8)
            o->w74 = 0;
        else
            o->w74 = 0x200;
        if ((d = (o->w7a - o->wb6) & 0xff)) {
            if (d < 0x80)
                o->wb6 = o->wb6 + 4;
            else
                o->wb6 = o->wb6 - 4;
        }
        o->wb6 = (unsigned char)o->wb6;
        if (((o->wb6 - 0x40) & 0xff) < 0x80) {
            o->animFrame = 1;
            o->d8c = (o->wb6 + 0x80) & 0xff;
        } else {
            o->animFrame = 0;
            o->d8c = o->wb6;
        }
        if ((short)(o->w74 - o->wb2) > 0)
            o->wb2 += 8;
        else
            o->wb2 -= 8;
        if (D_1F8001FC & D_1F8003C4) {
            if ((unsigned short)(*(unsigned short *)o->anim - 0x136) < 4) {
                PlayerSetAnimIfChanged(o, 0x45);
                o->wb2 = 0x400;
            }
        }
        FUN_8010d280(o);
        if (D_1F8001FC & D_1F8003C6) {
            ((PObj *)o)->bca = 0;
            o->step = 0x44;
            o->state = 0;
        }
        if (o->y.p.whole < o->w56 + 8) {
            o->step = 0x3d;
            o->state = 0;
        }
        break;
    }
}
