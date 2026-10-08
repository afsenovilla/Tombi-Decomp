// FUNC 800fe8f8 148 X006
// MATCHING 800fe8f8 148
#include "TOBJ.H"
typedef struct { char pad[8]; unsigned char b8; } P;
extern P *D_8009C330;
extern short D_8009C944;
extern short D_8009C946[];

void func_800FE8F8(TObj *o)
{
    o->h->raw += D_8009C944 << 8;
    o->y.raw += D_8009C946[0] << 8;
    if (D_8009C330->b8 == 0) {
        o->visible = *(unsigned char *)0x1F8001F8 & 1;
    }
    o->h->raw += o->velX << 8;
    o->y.raw += o->velY << 8;
}
