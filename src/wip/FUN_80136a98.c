// FUNC 80136a98 296 X000
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);
extern unsigned char DAT_8009cda9;
extern unsigned DAT_8009c984;

void FUN_80136a98(TObj *o)
{
    char *p = (char *)o + 0x1c;
    TObj *n;
    unsigned f;
    unsigned char v;
    if (DAT_8009cda9 == 0) {
        n = FUN_800183b8();
        if (n) {
            n->active = 1;
            n->type = 0x14;
            n->animFrame = 0;
            n->subtype = 0;
            *(short *)((char *)n + 0x1a) = -0x28;
            *(short *)((char *)n + 0x16) = -0x124;
            *(short *)((char *)n + 0x12) = 0xa38;
            *(TObj **)((char *)o + 0x20) = n;
        }
        v = 1;
    } else {
        v = 3;
        if (DAT_8009cda9 != 0xff) {
        n = FUN_800183b8();
        if (n) {
            *(short *)((char *)n + 0x12) = 0xa38;
            *(short *)((char *)n + 0x16) = -0x124;
            n->active = 1;
            n->type = 0x14;
            n->animFrame = 0;
            n->subtype = 0;
            *(short *)((char *)n + 0x1a) = -0x28;
            f = DAT_8009c984;
            if ((f & 8) && (f & 0x10)) {
                *(short *)((char *)n + 0x12) = 0xc29;
                *(short *)((char *)n + 0x16) = -0x2e3;
                n->active = 1;
                n->type = 0x14;
                n->animFrame = 0;
                n->subtype = 1;
                *(short *)((char *)n + 0x1a) = 0x5a;
            }
            *(TObj **)(p + 4) = n;
        }
        v = 1;
        }
    }
    o->b04 = v;
}
