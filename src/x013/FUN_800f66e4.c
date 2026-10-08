// FUNC 800f66e4 216 X013
// MATCHING 800f66e4 216
#include "TOBJ.H"
typedef struct { char pad[8]; unsigned char b8; char pad2[0x2c - 9]; short cur; unsigned short prev; } Cam;
extern Cam *DAT_8009c330;
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);

void FUN_800f66e4(TObj *o)
{
    Cam *p = DAT_8009c330;
    p->cur = 4;
    if (p->prev != 4) {
        p->cur = 4;
        FUN_800efc04(o);
        FUN_8001fe94(o, 1);
        DAT_8009c330->prev = DAT_8009c330->cur;
    }
    *(unsigned char *)&o->wac = 1;
    o->b9c = 2;
    o->timer = 10;
    DAT_8009c330->b8 = 1;
    o->velY = 0;
    o->velV = 0;
    o->d84 = 0;
    o->d88 = (o->animFrame & 1) ? 0xf0 : 0x10;
    o->d8c = (o->animFrame & 1) ? 0x18 : 0xe8;
    o->step = 2;
    o->state = 3;
}
