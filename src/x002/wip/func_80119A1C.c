/* score 6: only the last spawn loop (FUN_80018448): game fills the null-check beqz delay slot from the target (addiu v0,s1,1) and hoists lhu f->sub above the b0c/b0e stores; ours fills it with li v0,1. Tried store orders, raw stores, if-block instead of continue, t = f->sub temp (fixes the load order but not the slot). */
// FUNC 80119a1c 1316 X002
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

extern E D_8011C984[], D_8011CA9C[];
extern E8 D_8011C93C[];
extern TObj *D_8009F2D8[], *D_8009F378[], *D_8009F328[], *D_8009F2E0[];
extern unsigned char D_8009D2C3, D_8009CE41;
extern unsigned char D_8009CFDC[];
extern TObj *FUN_800183b8(void);
extern TObj *FUN_80018448(void);

void func_80119A1C(TObj *o)
{
    E *e;
    E8 *f;
    short i;
    TObj *p;
    TObj *q;

    for (i = 0, e = D_8011C984; e->flags != 0xff; e = &D_8011C984[++i]) {
        p = FUN_800183b8();
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
        D_8009F2D8[i] = p;
        p->d90 = (int)o;
        if (i == 0 && D_8009D2C3 != 0x7f) p->b04 = 2;
    }
    for (i = 0; i < 6; i++) {
        if (D_8009CFDC[i] == 0) {
            p = D_8009F2E0[i];
            p->y.p.whole -= 0x140;
        }
    }
    if (D_8009CE41 == 0xff) {
        for (i = 0; i < 6; i++) D_8009F2E0[i]->y.p.whole -= 0x140;
    }
    if (D_8009CE41 == 0) {
        for (i = 0, e = D_8011CA9C; e->flags != 0xff; e = &D_8011CA9C[++i]) {
            p = FUN_800183b8();
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
            D_8009F378[i] = p;
            p->d90 = (int)o;
            switch (i) {
            case 0:
                if (!(D_8009D2C3 & 1)) p->y.p.whole -= 0xf0;
                break;
            case 1:
                if (!(D_8009D2C3 & 2)) p->y.p.whole -= 0x12c;
                break;
            case 2:
                if (!(D_8009D2C3 & 4)) p->y.p.whole -= 0x140;
                break;
            case 3:
                if (!(D_8009D2C3 & 8)) p->y.p.whole -= 0x12c;
                break;
            case 4:
                if (!(D_8009D2C3 & 0x10)) p->y.p.whole -= 0x140;
                break;
            case 5:
                if (!(D_8009D2C3 & 0x40)) p->y.p.whole -= 0x12c;
                break;
            case 6:
                if (!(D_8009D2C3 & 0x20)) p->y.p.whole -= 0x12c;
                break;
            }
        }
    }
    for (i = 0, f = D_8011C93C; f->x != 0xff; f = &D_8011C93C[++i]) {
        q = FUN_80018448();
        if (q == 0) continue;
        q->active = 1;
        q->type = 0x4c;
        q->b0c = 0;
        q->_pad0e[0] = 0;
        q->subtype = f->sub;
        D_8009F328[i] = q;
        q->a.p.whole = f->x;
        q->y.p.whole = f->y;
        q->b.p.whole = f->z;
    }
}
