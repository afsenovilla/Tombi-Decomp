// FUNC 800f698c 448 I0NFO2
// MATCHING 800f698c 448
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE6C(TObj *o);
extern unsigned char D_8009CE56[];
extern int SPR_DATA[];
typedef struct { unsigned char b0, b1, b2, b3; unsigned short w1e; short spad; void **anims; } EntFF4;
extern EntFF4 D_800FEFF4[];

void topdownA19ActorInit(TObj *o)
{
    o->box0 = D_800FEFF4[o->subtype].b0;
    o->box1 = D_800FEFF4[o->subtype].b1;
    o->box2 = D_800FEFF4[o->subtype].b2;
    o->box3 = D_800FEFF4[o->subtype].b3;
    o->w1e = D_800FEFF4[o->subtype].w1e;
    o->d3c = SPR_DATA[D_800FEFF4[o->subtype].spad];
    o->anim = D_800FEFF4[o->subtype].anims[U16(o, 0xB4)];
    func_8001FE6C(o);
    o->d8c = 0;
    o->b0d = 0;
    if (U16(o, 0xB6) != 0) {
        o->b6b = 0;
    } else {
        o->b6b = 0x80;
    }
    switch (U16(o, 0xBA)) {
    case 8:
        o->b0d = 1;
        o->w08 = 0x7F48;
        if (D_8009CE56[0] == 0xFF) {
            o->b04 = 2;
        }
        break;
    case 9:
        o->b0d = 1;
        o->w08 = 0x7B93;
        break;
    }
    o->category |= 0x80;
    o->b04++;
}
