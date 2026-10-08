// FUNC 8004cb00 1240 MAIN0
/* score 20: only t/b register choice in the link code differs (game t=a0, b reuses the pointer reg v1; idx a2 in case 4). Tried: LINK macro forms, decl orders, types, q reuse as b (38), non-volatile t. */
#include "TOBJ.H"
typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5;
    short s6, s8, sa;
    unsigned char bc, bd, be, bf;
} Ent;
extern TObj *DAT_8009bd98;
extern TObj *DAT_8009bd9c;
extern int DAT_1f800334;
extern int DAT_1f800338;
extern TObj *allocObjectLayer2(void);
extern TObj *ObjAlloc(void);
extern TObj *allocObjectLayer4(void);
extern TObj *allocObjectLayer5(void);
extern TObj *allocObjectLayer1(void);
extern TObj *allocObjectLayer(void);


void FUN_8004cb00(Ent *e, int z)
{
    unsigned char kind;
    TObj *p;
    TObj *q2;
    unsigned char ty;
    unsigned char idx;
    volatile int *q;
    int t, b, i4;

    kind = e->b1 & 0x7f;
    switch (kind) {
    case 2:
        p = allocObjectLayer2();
        DAT_8009bd98 = p;
        if (p == 0)
            return;
        if (e->bc & 0x10) {
            q = &DAT_1f800334;
            goto link;
        }
        break;
    case 3:
        if ((DAT_8009bd98 = ObjAlloc()) == 0)
            return;
        break;
    case 4:
        p = allocObjectLayer4();
        DAT_8009bd98 = p;
        if (p == 0)
            return;
        idx = e->be;
        if (idx == 0xff) {
            p->da0 = 0;
            break;
        }
        if (e->bc & 0x10) {
            q = &DAT_1f800334;
            t = *q;
            b = *q;
            i4 = idx * 4;
            goto link2;
        }
        break;
    case 5:
        if ((DAT_8009bd98 = allocObjectLayer5()) == 0)
            return;
        break;
    case 8:
        p = allocObjectLayer1();
        DAT_8009bd98 = p;
        if (p == 0)
            return;
        q = &DAT_1f800338;
    link:
        t = *q;
        b = *q;
        i4 = e->be * 4;
    link2:
        b += *(int *)(t + i4 + 4);
        p->ba4 = 1;
        p->da0 = b;
        break;
    case 7:
        if ((DAT_8009bd98 = allocObjectLayer()) == 0)
            return;
        break;
    }
    if (z >= 0)
        DAT_8009bd98->b6b = z;
    else
        DAT_8009bd98->b6b = (e->bf >> 2) - 1;
    DAT_8009bd98->category |= e->b1 & 0x80;
    DAT_8009bd98->active = 1;
    DAT_8009bd98->b1d = e->b0;
    DAT_8009bd98->type = e->b2;
    DAT_8009bd98->subtype = e->b4;
    DAT_8009bd98->b0c = e->b3;
    DAT_8009bd98->b0a = e->bc;
    DAT_8009bd98->animFrame = e->b5;
    DAT_8009bd98->b0f = e->bd;
    DAT_8009bd98->a.raw = e->s6 << 16;
    DAT_8009bd98->y.raw = e->s8 << 16;
    DAT_8009bd98->b.raw = e->sa << 16;
    if (kind == 3) {
        ty = e->b2;
        if (ty == 0x18 || ty == 0x19 || ty == 0x24 || ty == 0x33)
            DAT_8009bd98->box0 = e->be;
    }
    if (kind == 6 || kind == 7)
        return;
    switch (e->bf & 3) {
    case 0:
        DAT_8009bd98->a.raw = e->s6 << 16;
        DAT_8009bd98->y.raw = e->s8 << 16;
        DAT_8009bd98->b.raw = e->sa << 16;
        DAT_8009bd98->d90 = 0;
        DAT_8009bd98->d94 = 0;
        break;
    case 1:
        DAT_8009bd98->a.raw = e->s6 << 16;
        DAT_8009bd98->y.raw = e->s8 << 16;
        DAT_8009bd98->b.raw = e->sa << 16;
        DAT_8009bd9c = DAT_8009bd98;
        DAT_8009bd98->d90 = 0;
        break;
    case 2:
        DAT_8009bd98->a.raw = DAT_8009bd9c->a.raw + (e->s6 << 16);
        DAT_8009bd98->y.raw = DAT_8009bd9c->y.raw + (e->s8 << 16);
        DAT_8009bd98->b.raw = DAT_8009bd9c->b.raw + (e->sa << 16);
        DAT_8009bd98->d30 = e->s6 << 16;
        DAT_8009bd98->d34 = e->s8 << 16;
        DAT_8009bd98->d38 = e->sa << 16;
        q2 = DAT_8009bd9c;
        DAT_8009bd9c = DAT_8009bd98;
        DAT_8009bd98->d90 = (int)q2;
        q2->d94 = (int)DAT_8009bd98;
        break;
    case 3:
        DAT_8009bd98->a.raw = DAT_8009bd9c->a.raw + (e->s6 << 16);
        DAT_8009bd98->y.raw = DAT_8009bd9c->y.raw + (e->s8 << 16);
        DAT_8009bd98->b.raw = DAT_8009bd9c->b.raw + (e->sa << 16);
        DAT_8009bd98->d30 = e->s6 << 16;
        DAT_8009bd98->d34 = e->s8 << 16;
        DAT_8009bd98->d38 = e->sa << 16;
        DAT_8009bd98->d90 = (int)DAT_8009bd9c;
        DAT_8009bd9c->d94 = (int)DAT_8009bd98;
        DAT_8009bd98->d94 = 0;
        break;
    }
}
