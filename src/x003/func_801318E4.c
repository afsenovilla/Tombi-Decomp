// FUNC 801318e4 1376 X003
// MATCHING 801318e4 1376
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe, f10, f12, f14, f16, f18, f1a;
} E28;
extern E28 D_80135FFC[], D_80135F54[], D_80135FC4[];
extern TObj *SCRIPT_OBJECTS[];
extern TObj *D_8009F3D4[];
extern TObj *allocObjectLayer2(void);
extern TObj *allocObjectLayer4(void);

void func_801318E4(TObj *o)
{
    E28 *p;
    short i;
    TObj *n;
    unsigned short v = GAME.selectedSection;

    if (v == 2) {
        if (GAME.purifiedAreas & 4) {
            o->b04 = 3;
            return;
        }
        i = 0;
        p = D_80135FFC;
        for (; p->f0 != 0xff; p = &D_80135FFC[++i]) {
            n = allocObjectLayer2();
            if (n == 0) continue;
            {
                n->active = 3;
                n->type = 0x32;
                n->b0a = 0;
                n->animFrame = p->f2;
                n->subtype = p->f18;
                n->b0c = p->f1a;
                n->a.p.whole = p->f12;
                n->y.p.whole = p->f14;
                n->b.p.whole = p->f16;
                n->wb4 = p->f4;
                n->wb8 = p->f10;
                n->wba = p->fe;
                n->w74 = p->f6;
                n->w76 = p->f8;
                n->w78 = p->fa;
                n->w7a = p->fc;
                n->d90 = (int)o;
                SCRIPT_OBJECTS[i] = n;
                n->b68 = 0;
            }
        }
        return;
    }
    switch (GCD.bcdc7) {
    case 0:
        if (v) break;
        i = 0;
        p = D_80135F54;
        for (; p->f0 != 0xff; p = &D_80135F54[++i]) {
            if (*(volatile unsigned short *)&p->f0 & 0x80) {
                n = allocObjectLayer4();
                if (n == 0) continue;
                {
                    n->active = 1;
                    n->type = 0x1a;
                    n->b0a = 0x10;
                    n->animFrame = p->f2;
                    n->subtype = p->f18;
                    n->b0c = p->f1a;
                    n->a.p.whole = p->f12;
                    n->y.p.whole = p->f14;
                    n->b.p.whole = p->f16;
                }
            } else {
                n = allocObjectLayer2();
                if (n) {
                    n->active = 3;
                    n->type = 0x32;
                    n->b0a = 0;
                    n->animFrame = p->f2;
                    n->subtype = p->f18;
                    n->b0c = p->f1a;
                    n->a.p.whole = p->f12;
                    n->y.p.whole = p->f14;
                    n->b.p.whole = p->f16;
                    n->wb4 = p->f4;
                    n->wb8 = p->f10;
                    n->wba = p->fe;
                    n->w74 = p->f6;
                    n->w76 = p->f8;
                    n->w78 = p->fa;
                    n->w7a = p->fc;
                    n->d90 = (int)o;
                    SCRIPT_OBJECTS[i] = n;
                }
                if (p->fe == 3 && GCD.bcdc1) n->b04 = 3;
            }
        }
        return;
    case 1:
        if (v != 1) break;
        i = 0;
        p = D_80135FC4;
        for (; p->f0 != 0xff; p = &D_80135FC4[++i]) {
            if (*(volatile unsigned short *)&p->f0 & 0x80) {
                n = allocObjectLayer4();
                if (n == 0) continue;
                {
                    n->active = 1;
                    n->type = 0x1a;
                    n->b0a = 0x10;
                    n->animFrame = p->f2;
                    n->subtype = p->f18;
                    n->b0c = p->f1a;
                    n->a.p.whole = p->f12;
                    n->y.p.whole = p->f14;
                    n->b.p.whole = p->f16;
                }
            } else {
                n = allocObjectLayer2();
                if (n == 0) continue;
                {
                    n->active = 2;
                    n->type = 0x32;
                    n->b0a = 0;
                    n->animFrame = p->f2;
                    n->subtype = p->f18;
                    n->b0c = p->f1a;
                    n->a.p.whole = p->f12;
                    n->y.p.whole = p->f14;
                    n->b.p.whole = p->f16;
                    n->wb4 = p->f4;
                    n->wb8 = p->f10;
                    n->wba = p->fe;
                    n->w74 = p->f6;
                    n->w76 = p->f8;
                    n->w78 = p->fa;
                    n->w7a = p->fc;
                    n->d90 = (int)o;
                    D_8009F3D4[0] = n;
                }
            }
        }
        return;
    }
    o->b04 = 3;
}
