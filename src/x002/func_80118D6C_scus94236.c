// VERSION SCUS_94236 x02 80118D6C 1276
// FLAGS -O2 -G0
#if defined(VERSION_SCUS_94236)
// USA version of JP x02 80118CB8 (src/x002/spawnPigGateSceneNpcs.c, VERSION_SLPS_01144 branch): US globals and callee names.
#include "TOBJ.H"
typedef struct {
    short flags;
    short frame;
    short wb4;
    short w74, w76, w78, w7a;
    short wba, wb8;
    short x, y, z;
    short subtype;
    short b0c;
} E;
typedef struct { short x, y, z, sub; } E8;

extern E D_8011BCAC[], D_8011BDC4[];
extern E8 D_8011BC64[];
extern TObj *D_8009E640[], *D_8009E6E0[], *D_8009E690[], *D_8009E648[];
extern unsigned char D_8009C62B[], D_8009C344[], D_8009C1A9[];
extern TObj *func_80018354(void);
extern TObj *func_800183E4(void);

void func_80118D6C(TObj *o)
{
    E *e;
    E8 *f;
    short i;
    TObj *p;
    TObj *q;

    for (i = 0, e = D_8011BCAC; e->flags != 0xff; e = &D_8011BCAC[++i]) {
        p = func_80018354();
        if (p == 0) continue;
        p->active = 3;
        if (p->subtype) p->active = 2;
        p->type = 0x32;
        p->b0a = 0;
        p->animFrame = e->frame;
        p->subtype = e->subtype;
        p->b0c = e->b0c;
        p->a.p.whole = e->x;
        p->y.p.whole = e->y;
        p->b.p.whole = e->z;
        p->wb4 = e->wb4;
        p->wb8 = e->wb8;
        p->wba = e->wba;
        p->w74 = e->w74;
        p->w76 = e->w76;
        p->w78 = e->w78;
        p->w7a = e->w7a;
        p->b68 = 0;
        D_8009E640[i] = p;
        p->d90 = (int)o;
        if (i == 0 && D_8009C62B[0] != 0x7f) p->b04 = 2;
    }
    for (i = 0; i < 6; i++) {
        if (D_8009C344[i] == 0) {
            p = D_8009E648[i];
            p->y.p.whole -= 0x140;
        }
    }
    if (D_8009C1A9[0] == 0xff) {
        for (i = 0; i < 6; i++) D_8009E648[i]->y.p.whole -= 0x140;
    }
    if (D_8009C1A9[0] == 0) {
        for (i = 0, e = D_8011BDC4; e->flags != 0xff; e = &D_8011BDC4[++i]) {
            p = func_80018354();
            if (p == 0) continue;
            p->active = 2;
            p->type = 0x52;
            p->b0a = 0;
            p->animFrame = e->frame;
            p->subtype = e->subtype;
            p->b0c = e->b0c;
            p->a.p.whole = e->x;
            p->y.p.whole = e->y;
            p->b.p.whole = e->z;
            p->wb4 = e->wb4;
            p->wb8 = e->wb8;
            p->wba = e->wba;
            p->w74 = e->w74;
            p->w76 = e->w76;
            p->w78 = e->w78;
            p->w7a = e->w7a;
            D_8009E6E0[i] = p;
            p->d90 = (int)o;
            switch (i) {
            case 0:
                if (!(D_8009C62B[0] & 1)) p->y.p.whole -= 0xf0;
                break;
            case 1:
                if (!(D_8009C62B[0] & 2)) p->y.p.whole -= 0x12c;
                break;
            case 2:
                if (!(D_8009C62B[0] & 4)) p->y.p.whole -= 0x12c;
                break;
            case 3:
                if (!(D_8009C62B[0] & 8)) p->y.p.whole -= 0x12c;
                break;
            case 4:
                if (!(D_8009C62B[0] & 0x10)) p->y.p.whole -= 0x12c;
                break;
            case 5:
                if (!(D_8009C62B[0] & 0x40)) p->y.p.whole -= 0x12c;
                break;
            case 6:
                if (!(D_8009C62B[0] & 0x20)) p->y.p.whole -= 0x12c;
                break;
            }
        }
    }
    for (i = 0; (f = &D_8011BC64[i])->x != 0xff; i++) {
        unsigned short t;
        q = func_800183E4();
        if (q == 0) continue;
        q->active = 1;
        q->type = 0x4c;
        t = f->sub;
        q->b0c = 0;
        q->_pad0e[0] = 0;
        q->subtype = t;
        D_8009E690[i] = q;
        q->a.p.whole = f->x;
        q->y.p.whole = f->y;
        q->b.p.whole = f->z;
    }
}
#endif
