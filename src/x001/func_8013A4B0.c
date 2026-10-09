// FUNC 8013a4b0 436 X001
// MATCHING 8013a4b0 436
#include "TOBJ.H"

extern unsigned char D_8009CE59;
extern unsigned char D_8009CEDE;
extern unsigned char D_8009CEDF;
extern TObj *FUN_800183b8(void);

void func_8013A4B0(TObj *o)
{
    TObj *e;
    TObj **p;
    unsigned char *q;

    o->b04 = 3;
    p = (TObj **)&o->category;
    switch (D_8009CE59) {
    case 1:
    case 3:
        q = &D_8009CEDE;
        if (*q == 1) {
            e = FUN_800183b8();
            if (e) {
                e->active = 1;
                e->active = 1;
            e->type = 0xd;
                e->animFrame = 1;
                e->b0a = 2;
                e->a.p.whole = 0x105b;
                e->y.p.whole = -0x178;
                e->b.p.whole = 0x26a;
                e->subtype = 0;
                e->b0c = 0;
                *p = e;
                D_8009CE59 = 3;
                *q = 0;
                D_8009CEDF = 0;
                o->b04 = 1;
            }
        } else {
            e = FUN_800183b8();
            if (e) {
                e->active = 1;
                e->active = 1;
            e->type = 0xd;
                e->animFrame = 1;
                e->b0a = 2;
                e->a.p.whole = 0x105b;
                e->y.p.whole = -0x178;
                e->subtype = 0;
                e->b0c = 0;
                e->b.p.whole = 0x26a;
                *p = e;
                o->b04 = 1;
            }
        }
        break;
    case 2:
        e = FUN_800183b8();
        if (e) {
            e->active = 1;
            e->type = 0xd;
            e->animFrame = 1;
            e->b0a = 2;
            e->a.p.whole = 0x105b;
            e->y.p.whole = -0x6d4;
            e->subtype = 0;
            e->b0c = 0;
            e->b.p.whole = 0x53c;
            *(TObj **)&o->category = e;
            o->b04 = 1;
        }
        break;
    }
}
