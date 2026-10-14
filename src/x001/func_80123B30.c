// FUNC 80123b30 800 X001
// MATCHING 80123b30 800
#include "spad.h"
#include "TOBJ.H"

extern int D_1F80018C;
extern unsigned short D_1F8001F8_h;
extern unsigned char D_800A4553;
extern unsigned char D_8007A7F0[];
extern void (*D_8007A890[])(TObj *);
extern void initItemObject(TObj *);
extern int ObjCullRegister(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern TObj *ObjAlloc(void);
extern int Rand(void);
extern void setSectionFlagBit(int);
extern void freeObjectLayer5(TObj *);

void func_80123B30(TObj *o)
{
    TObj *n;

    switch (o->b04) {
    case 0:
        initItemObject(o);
        o->velV = 0;
        if (o->velH) o->step = 2;
        else o->step = 0;
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->velV += 0x10;
            if (o->velV > 0x400) o->velV = 0x400;
            o->y.raw += o->velV << 8;
            D_1F80018C = o->h->raw;
            SPAD.i190 = o->y.raw;
            if (o->b69 || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8)) {
                o->timer = 0x1e;
                o->step++;
                break;
            }
            n = ObjAlloc();
            if (n == 0) break;
            n->active = 1;
            n->type = 0x31;
            n->subtype = 1;
            n->b0c = ((*(unsigned char*)&SPAD.s1f8) + SPAD.i198) & 3;
            n->h->raw = o->h->raw;
            n->y.raw = o->y.raw;
            n->d->raw = o->d->raw;
            if (D_1F8001F8_h & 1)
                n->h->raw += (Rand() & 0xf) << 16;
            else
                n->h->raw -= (Rand() & 0xf) << 16;
            break;
        case 1:
            if (--o->timer == 0) {
                D_800A4553 = 4;
                o->step++;
            }
            break;
        case 2:
            if (o->visible) {
                o->y.p.whole += 2;
                TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8);
            }
            break;
        }
        break;
    case 2:
        setSectionFlagBit(o->b6b);
        D_8007A890[D_8007A7F0[o->subtype]](o);
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
